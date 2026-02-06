package com.canc.dwa;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Choreographer.FrameCallback {

    static { System.loadLibrary("dwa"); }

    private static final int PICK_ROM_REQUEST = 1;
    private static final String PREFS_NAME = "ControllerPrefs";
    
    private View gameContainer, setupContainer, menuOverlay;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private boolean isSurfaceReady = false; 
    private boolean isEditMode = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        
        // New UI Elements for the Menu
        menuOverlay = findViewById(R.id.menu_overlay); // Ensure this exists in your XML

        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());

        if (isRomExtracted()) {
            showGameLayout();
        }
    }

    // --- Menu and Orientation Logic ---

    @Override
    public void onBackPressed() {
        if (isEditMode) {
            exitEditMode();
        } else if (gameContainer.getVisibility() == View.VISIBLE) {
            showSettingsMenu();
        } else {
            super.onBackPressed();
        }
    }

    private void showSettingsMenu() {
        nativePauseEngine();
        
        String[] options = {"Toggle Orientation", "Move Buttons", "Reset Layout", "Resume"};
        new AlertDialog.Builder(this)
                .setTitle("Game Menu")
                .setItems(options, (dialog, which) -> {
                    switch (which) {
                        case 0: toggleOrientation(); break;
                        case 1: enterEditMode(); break;
                        case 2: resetPositions(); break;
                        case 3: resumeGame(); break;
                    }
                })
                .setOnCancelListener(dialog -> resumeGame())
                .show();
    }

    private void toggleOrientation() {
        int current = getResources().getConfiguration().orientation;
        if (current == Configuration.ORIENTATION_LANDSCAPE) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
        resumeGame();
    }

    private void resumeGame() {
        nativeResumeEngine();
    }

    // --- Draggable Button Logic ---

    private void enterEditMode() {
        isEditMode = true;
        Toast.makeText(this, "Drag buttons to move. Press BACK to save.", Toast.LENGTH_LONG).show();
        setupTouchControls(); // Re-bind with Edit Mode logic
    }

    private void exitEditMode() {
        isEditMode = false;
        Toast.makeText(this, "Layout Saved", Toast.LENGTH_SHORT).show();
        setupTouchControls(); // Re-bind with Input Mode logic
        resumeGame();
    }

    private void savePosition(String key, float x, float y) {
        SharedPreferences.Editor editor = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit();
        editor.putFloat(key + "_x", x);
        editor.putFloat(key + "_y", y);
        editor.apply();
    }

    private void loadPosition(View v, String key) {
        SharedPreferences prefs = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        float x = prefs.getFloat(key + "_x", -1);
        float y = prefs.getFloat(key + "_y", -1);
        if (x != -1 && y != -1) {
            v.setX(x);
            v.setY(y);
        }
    }

    private void resetPositions() {
        getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit().clear().apply();
        recreate(); // Simple way to snap back to XML defaults
    }

    // --- Engine & Surface Core ---

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady) return;

        nativeUpdateSurface(screenBitmap);

        SurfaceHolder holder = gameSurface.getHolder();
        Canvas canvas = holder.lockCanvas();
        if (canvas != null) {
            try {
                // Rendering code remains the same...
                float scale = Math.min((float)gameSurface.getWidth() / 256, (float)gameSurface.getHeight() / 240);
                int w = (int)(256 * scale);
                int h = (int)(240 * scale);
                int left = (gameSurface.getWidth() - w) / 2;
                int top = (gameSurface.getHeight() - h) / 2;
                destRect.set(left, top, left + w, top + h);
                canvas.drawColor(0xFF000000); 
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally {
                holder.unlockCanvasAndPost(canvas);
            }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        bindButton(R.id.btn_up,    0x08, "up"); 
        bindButton(R.id.btn_down,  0x04, "down");
        bindButton(R.id.btn_left,  0x02, "left"); 
        bindButton(R.id.btn_right, 0x01, "right");
        bindButton(R.id.btn_a,     0x80, "a"); 
        bindButton(R.id.btn_b,     0x40, "b");
        bindButton(R.id.btn_start, 0x10, "start"); 
        bindButton(R.id.btn_select,0x20, "select");
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int resId, int bitmask, String prefKey) {
        View v = findViewById(resId);
        if (v == null) return;

        loadPosition(v, prefKey);

        v.setOnTouchListener((v1, event) -> {
            if (isEditMode) {
                // DRAG LOGIC
                if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    v1.setX(event.getRawX() - (v1.getWidth() / 2f));
                    v1.setY(event.getRawY() - (v1.getHeight() / 2f));
                    savePosition(prefKey, v1.getX(), v1.getY());
                }
                return true;
            } else {
                // INPUT LOGIC
                int action = event.getAction();
                if (action == MotionEvent.ACTION_DOWN) {
                    injectInput(bitmask, true);
                    v1.setPressed(true);
                } else if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
                    injectInput(bitmask, false);
                    v1.setPressed(false);
                }
                return true;
            }
        });
    }

    // --- Lifecycle and ROM extraction helpers ---

    private void showGameLayout() {
        if (setupContainer != null) setupContainer.setVisibility(View.GONE);
        if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
        setupTouchControls();
    }

    private void startNativeEngine() {
        if (isEngineRunning) return; 
        showGameLayout();
        String filesPath = getFilesDir().getAbsolutePath();
        nativeInitEngine(filesPath);
        isEngineRunning = true;
        if (isSurfaceReady) Choreographer.getInstance().postFrameCallback(this);
    }

    private boolean isRomExtracted() {
        File chrFile = new File(getFilesDir(), "chr_rom.bin");
        File prgFile = new File(getFilesDir(), "prg_bank_0.bin");
        return chrFile.exists() && prgFile.exists() && chrFile.length() > 0;
    }

    private void openFilePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*"); 
        startActivityForResult(intent, PICK_ROM_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_ROM_REQUEST && resultCode == Activity.RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) copyAndExtract(uri);
        }
    }

    private void copyAndExtract(Uri uri) {
        try {
            InputStream is = getContentResolver().openInputStream(uri);
            if (is == null) return;
            File internalRom = new File(getFilesDir(), "base.nes");
            FileOutputStream os = new FileOutputStream(internalRom);
            byte[] buffer = new byte[16384];
            int length;
            while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);
            os.close(); is.close();
            String outPath = getFilesDir().getAbsolutePath();
            String result = nativeExtractRom(internalRom.getAbsolutePath(), outPath);
            if ("Success".equals(result)) startNativeEngine();
            else Toast.makeText(this, "Extraction failed: " + result, Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            Log.e("DWA", "File error", e);
        }
    }

    @Override 
    public void surfaceCreated(@NonNull SurfaceHolder h) {
        isSurfaceReady = true;
        if (isRomExtracted()) {
            startNativeEngine();
            Choreographer.getInstance().removeFrameCallback(this);
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isSurfaceReady = false; }

    // --- Native Methods ---
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void nativePauseEngine();
    public native void nativeResumeEngine();
}
