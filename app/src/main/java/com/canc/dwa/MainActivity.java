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
import android.view.ScaleGestureDetector;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
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

    // Layout State
    private float gameScreenYOffset = 0;
    private float gameScaleFactor = 1.0f;
    private ScaleGestureDetector scaleDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        menuOverlay = findViewById(R.id.menu_overlay);

        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        // Load screen settings
        SharedPreferences prefs = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        gameScreenYOffset = prefs.getFloat("screen_y_offset", 0);
        gameScaleFactor = prefs.getFloat("screen_scale", 1.0f);

        setupScaleDetector();

        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());

        if (isRomExtracted()) {
            showGameLayout();
        }
    }

    private void setupScaleDetector() {
        scaleDetector = new ScaleGestureDetector(this, new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override
            public boolean onScale(ScaleGestureDetector detector) {
                if (isEditMode) {
                    gameScaleFactor *= detector.getScaleFactor();
                    gameScaleFactor = Math.max(0.5f, Math.min(gameScaleFactor, 2.5f));
                    saveLayout();
                    return true;
                }
                return false;
            }
        });
    }

    @Override
    public void onBackPressed() {
        if (isEditMode) exitEditMode();
        else if (gameContainer != null && gameContainer.getVisibility() == View.VISIBLE) showSettingsMenu();
        else super.onBackPressed();
    }

    private void showSettingsMenu() {
        nativePauseEngine();
        if (menuOverlay != null) menuOverlay.setVisibility(View.VISIBLE);
        String[] options = {"Toggle Orientation", "Edit Layout (Drag/Pinch)", "Reset Layout", "Resume"};
        new AlertDialog.Builder(this)
                .setTitle("Settings")
                .setItems(options, (dialog, which) -> {
                    if (which == 0) toggleOrientation();
                    else if (which == 1) enterEditMode();
                    else if (which == 2) resetPositions();
                    else resumeGame();
                })
                .setOnCancelListener(dialog -> resumeGame())
                .show();
    }

    private void toggleOrientation() {
        int current = getResources().getConfiguration().orientation;
        setRequestedOrientation(current == Configuration.ORIENTATION_LANDSCAPE ? 
                ActivityInfo.SCREEN_ORIENTATION_PORTRAIT : ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        resumeGame();
    }

    private void resumeGame() {
        if (menuOverlay != null) menuOverlay.setVisibility(View.GONE);
        nativeResumeEngine();
    }

    private void enterEditMode() {
        isEditMode = true;
        if (menuOverlay != null) menuOverlay.setVisibility(View.GONE);
        Toast.makeText(this, "Drag buttons/screen. Pinch screen to scale.", Toast.LENGTH_LONG).show();
        setupTouchControls(); 
    }

    private void exitEditMode() {
        isEditMode = false;
        saveLayout();
        setupTouchControls(); 
        resumeGame();
    }

    private void saveLayout() {
        getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit()
                .putFloat("screen_y_offset", gameScreenYOffset)
                .putFloat("screen_scale", gameScaleFactor)
                .apply();
    }

    private void savePosition(String key, float x, float y) {
        getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit()
                .putFloat(key + "_x", x).putFloat(key + "_y", y).apply();
    }

    private void loadPosition(View v, String key) {
        SharedPreferences prefs = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        float x = prefs.getFloat(key + "_x", -1);
        float y = prefs.getFloat(key + "_y", -1);
        if (x != -1 && y != -1) { v.setX(x); v.setY(y); }
    }

    private void resetPositions() {
        getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit().clear().apply();
        recreate(); 
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady) return;
        nativeUpdateSurface(screenBitmap);

        SurfaceHolder holder = gameSurface.getHolder();
        Canvas canvas = holder.lockCanvas();
        if (canvas != null) {
            try {
                canvas.drawColor(0xFF000000);
                float baseScale = Math.min((float)gameSurface.getWidth() / 256f, (float)gameSurface.getHeight() / 240f);
                float finalScale = baseScale * gameScaleFactor;
                
                int w = (int)(256 * finalScale);
                int h = (int)(240 * finalScale);
                int left = (gameSurface.getWidth() - w) / 2;
                int top = ((gameSurface.getHeight() - h) / 2) + (int)gameScreenYOffset;

                destRect.set(left, top, left + w, top + h);
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally { holder.unlockCanvasAndPost(canvas); }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        // Grouped D-Pad
        View[] dpad = { findViewById(R.id.btn_up), findViewById(R.id.btn_down), 
                        findViewById(R.id.btn_left), findViewById(R.id.btn_right) };
        String[] keys = {"up", "down", "left", "right"};
        int[] masks = {0x08, 0x04, 0x02, 0x01};

        for (int i = 0; i < dpad.length; i++) {
            if (dpad[i] != null) bindDpadGroup(dpad[i], masks[i], keys[i], dpad, keys);
        }

        bindButton(R.id.btn_a, 0x80, "a"); 
        bindButton(R.id.btn_b, 0x40, "b");
        bindButton(R.id.btn_start, 0x10, "start"); 
        bindButton(R.id.btn_select, 0x20, "select");

        // Screen Adjustments
        gameSurface.setOnTouchListener((v, event) -> {
            scaleDetector.onTouchEvent(event);
            if (isEditMode && event.getAction() == MotionEvent.ACTION_MOVE && !scaleDetector.isInProgress()) {
                gameScreenYOffset = event.getRawY() - (gameSurface.getHeight() / 2f);
                return true;
            }
            return isEditMode;
        });
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindDpadGroup(View v, int mask, String key, View[] group, String[] keys) {
        loadPosition(v, key);
        v.setOnTouchListener((v1, event) -> {
            if (isEditMode && event.getAction() == MotionEvent.ACTION_MOVE) {
                float dx = event.getRawX() - (v1.getWidth() / 2f) - v1.getX();
                float dy = event.getRawY() - (v1.getHeight() / 2f) - v1.getY();
                for (int i = 0; i < group.length; i++) {
                    group[i].setX(group[i].getX() + dx);
                    group[i].setY(group[i].getY() + dy);
                    savePosition(keys[i], group[i].getX(), group[i].getY());
                }
                return true;
            }
            return handleInput(v1, event, mask);
        });
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int resId, int mask, String key) {
        View v = findViewById(resId);
        if (v == null) return;
        loadPosition(v, key);
        v.setOnTouchListener((v1, event) -> {
            if (isEditMode && event.getAction() == MotionEvent.ACTION_MOVE) {
                v1.setX(event.getRawX() - (v1.getWidth() / 2f));
                v1.setY(event.getRawY() - (v1.getHeight() / 2f));
                savePosition(key, v1.getX(), v1.getY());
                return true;
            }
            return handleInput(v1, event, mask);
        });
    }

    private boolean handleInput(View v, MotionEvent e, int mask) {
        if (e.getAction() == MotionEvent.ACTION_DOWN) { injectInput(mask, true); v.setPressed(true); }
        else if (e.getAction() == MotionEvent.ACTION_UP || e.getAction() == MotionEvent.ACTION_CANCEL) { 
            injectInput(mask, false); v.setPressed(false); 
        }
        return true;
    }

    private void showGameLayout() {
        if (setupContainer != null) setupContainer.setVisibility(View.GONE);
        if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
        setupTouchControls();
    }

    private void startNativeEngine() {
        if (isEngineRunning) return;
        showGameLayout();
        nativeInitEngine(getFilesDir().getAbsolutePath());
        isEngineRunning = true;
        if (isSurfaceReady) Choreographer.getInstance().postFrameCallback(this);
    }

    private boolean isRomExtracted() {
        return new File(getFilesDir(), "chr_rom.bin").exists() && new File(getFilesDir(), "prg_bank_0.bin").exists();
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
            File internalRom = new File(getFilesDir(), "base.nes");
            FileOutputStream os = new FileOutputStream(internalRom);
            byte[] buf = new byte[16384]; int len;
            while ((len = is.read(buf)) > 0) os.write(buf, 0, len);
            os.close(); is.close();
            if ("Success".equals(nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath()))) startNativeEngine();
        } catch (Exception e) { Log.e("DWA", "File error", e); }
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder h) { 
        isSurfaceReady = true; 
        if (isRomExtracted()) { startNativeEngine(); Choreographer.getInstance().postFrameCallback(this); }
    }
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isSurfaceReady = false; }

    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void nativePauseEngine();
    public native void nativeResumeEngine();
}
