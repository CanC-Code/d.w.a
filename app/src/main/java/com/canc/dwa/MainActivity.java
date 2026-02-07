package com.canc.dwa;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.view.Choreographer;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Choreographer.FrameCallback {

    static {
        System.loadLibrary("dwa");
    }

    private static final int PICK_ROM_REQUEST = 1;
    private static final String ROM_FILENAME = "base.nes";

    // UI Components
    private View setupContainer, gameContainer, debugOverlay;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private final Rect destRect = new Rect();
    private final Paint bitmapPaint = new Paint(Paint.FILTER_BITMAP_FLAG);

    // Engine State
    private boolean isEngineRunning = false;
    private boolean isSurfaceReady = false;
    private boolean isDebugVisible = false;
    private boolean isTurboEnabled = false;
    private boolean isEditMode = false;

    private Vibrator vibrator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        debugOverlay = findViewById(R.id.debug_overlay_bg);

        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        if (gameSurface != null) gameSurface.getHolder().addCallback(this);

        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());
        findViewById(R.id.btn_debug).setOnClickListener(v -> toggleDebugUI());
        View turboBtn = findViewById(R.id.btn_turbo);
        if (turboBtn != null) turboBtn.setOnClickListener(v -> toggleTurbo());

        // Check for existing ROM on launch
        checkExistingRom();
    }

    private void checkExistingRom() {
        File romFile = new File(getFilesDir(), ROM_FILENAME);
        if (romFile.exists()) {
            // Automatically boot if ROM exists
            initializeGameSession(romFile.getAbsolutePath());
        }
    }

    // ============================================================================
    // MENU SYSTEM & ROM OFFLOADING
    // ============================================================================

    @Override
    public void onBackPressed() {
        if (gameContainer.getVisibility() == View.VISIBLE) {
            showGameMenu();
        } else {
            super.onBackPressed();
        }
    }

    private void showGameMenu() {
        String[] options = {
            isEditMode ? "Lock Controller Layout" : "Move Controller Layout",
            "Reset Game",
            "ROM Management...",
            "Close App"
        };

        new AlertDialog.Builder(this, android.R.style.Theme_DeviceDefault_Dialog_Alert)
            .setTitle("Main Menu")
            .setItems(options, (dialog, which) -> {
                switch (which) {
                    case 0: isEditMode = !isEditMode; break;
                    case 1: nativeInitEngine(getFilesDir().getAbsolutePath()); break;
                    case 2: showRomSubmenu(); break;
                    case 3: finish(); break;
                }
            })
            .setNegativeButton("Resume", null)
            .show();
    }

    private void showRomSubmenu() {
        String[] options = {"Offload (Delete) Saved ROM", "Back"};
        new AlertDialog.Builder(this, android.R.style.Theme_DeviceDefault_Dialog_Alert)
            .setTitle("ROM Management")
            .setItems(options, (dialog, which) -> {
                if (which == 0) confirmRomDeletion();
            })
            .show();
    }

    private void confirmRomDeletion() {
        new AlertDialog.Builder(this)
            .setTitle("Delete ROM?")
            .setMessage("This will remove the local copy of the ROM and return you to the setup screen.")
            .setPositiveButton("Delete", (dialog, which) -> deleteLocalRom())
            .setNegativeButton("Cancel", null)
            .show();
    }

    private void deleteLocalRom() {
        File romFile = new File(getFilesDir(), ROM_FILENAME);
        if (romFile.exists() && romFile.delete()) {
            showToast("ROM Removed");
            returnToSetup();
        }
    }

    private void returnToSetup() {
        isEngineRunning = false;
        Choreographer.getInstance().removeFrameCallback(this);
        runOnUiThread(() -> {
            gameContainer.setVisibility(View.GONE);
            setupContainer.setVisibility(View.VISIBLE);
        });
    }

    // ============================================================================
    // ROM HANDLING & INITIALIZATION
    // ============================================================================

    private void openFilePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, PICK_ROM_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_ROM_REQUEST && resultCode == RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) copyAndExtractRom(uri);
        }
    }

    private void copyAndExtractRom(Uri uri) {
        try (InputStream is = getContentResolver().openInputStream(uri)) {
            File internalRom = new File(getFilesDir(), ROM_FILENAME);
            try (FileOutputStream os = new FileOutputStream(internalRom)) {
                byte[] buffer = new byte[8192];
                int read;
                while ((read = is.read(buffer)) != -1) os.write(buffer, 0, read);
            }

            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if ("Success".equals(result)) {
                initializeGameSession(internalRom.getAbsolutePath());
            } else {
                showToast("Extraction Failed: " + result);
            }
        } catch (Exception e) {
            showToast("Load Failed: " + e.getMessage());
        }
    }

    private void initializeGameSession(String romPath) {
        runOnUiThread(() -> {
            setupContainer.setVisibility(View.GONE);
            gameContainer.setVisibility(View.VISIBLE);
            setupTouchControls();
            
            nativeInitEngine(getFilesDir().getAbsolutePath());
            isEngineRunning = true;
            
            if (isSurfaceReady) {
                Choreographer.getInstance().postFrameCallback(this);
            }
        });
    }

    // ============================================================================
    // RENDERING, INPUT & UTILS
    // ============================================================================

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady) return;
        nativeUpdateSurface(screenBitmap);
        Canvas canvas = gameSurface.getHolder().lockCanvas();
        if (canvas != null) {
            try {
                canvas.drawColor(0xFF000000);
                float scale = Math.min((float)gameSurface.getWidth() / 256f, (float)gameSurface.getHeight() / 240f);
                int sw = (int) (256 * scale), sh = (int) (240 * scale);
                int left = (gameSurface.getWidth() - sw) / 2, top = (gameSurface.getHeight() - sh) / 2;
                destRect.set(left, top, left + sw, top + sh);
                canvas.drawBitmap(screenBitmap, null, destRect, bitmapPaint);
            } finally {
                gameSurface.getHolder().unlockCanvasAndPost(canvas);
            }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    private float dX, dY;
    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int resId, int mask) {
        View button = findViewById(resId);
        if (button == null) return;
        button.setOnTouchListener((v, event) -> {
            if (isEditMode) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    dX = v.getX() - event.getRawX(); dY = v.getY() - event.getRawY();
                } else if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    v.setX(event.getRawX() + dX); v.setY(event.getRawY() + dY);
                }
                return true;
            } else {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    performHapticFeedback(); injectInput(mask, true); v.setPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                    injectInput(mask, false); v.setPressed(false);
                }
                return true;
            }
        });
    }

    private void setupTouchControls() {
        int[][] btns = {{R.id.btn_up, 0x08}, {R.id.btn_down, 0x04}, {R.id.btn_left, 0x02}, {R.id.btn_right, 0x01},
                        {R.id.btn_a, 0x80}, {R.id.btn_b, 0x40}, {R.id.btn_start, 0x10}, {R.id.btn_select, 0x20}};
        for (int[] b : btns) bindButton(b[0], b[1]);
        
        View dpad = findViewById(R.id.dpad_container);
        if (dpad != null) dpad.setOnTouchListener((v, event) -> {
            if (isEditMode) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    dX = v.getX() - event.getRawX(); dY = v.getY() - event.getRawY();
                } else if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    v.setX(event.getRawX() + dX); v.setY(event.getRawY() + dY);
                }
                return true;
            }
            return false;
        });
    }

    private void hideSystemUI() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            final WindowInsetsController c = getWindow().getInsetsController();
            if (c != null) {
                c.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                c.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        }
    }

    private void performHapticFeedback() {
        if (vibrator != null && vibrator.hasVibrator()) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) vibrator.vibrate(VibrationEffect.createOneShot(15, VibrationEffect.DEFAULT_AMPLITUDE));
            else vibrator.vibrate(15);
        }
    }

    private void toggleTurbo() { isTurboEnabled = !isTurboEnabled; nativeSetTurbo(isTurboEnabled); showToast("Turbo: " + (isTurboEnabled ? "ON" : "OFF")); }
    private void toggleDebugUI() { isDebugVisible = !isDebugVisible; toggleDebugMenu(); if (debugOverlay != null) debugOverlay.setVisibility(isDebugVisible ? View.VISIBLE : View.GONE); }
    @Override public void surfaceCreated(@NonNull SurfaceHolder h) { isSurfaceReady = true; if (isEngineRunning) Choreographer.getInstance().postFrameCallback(this); }
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isSurfaceReady = false; }
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override protected void onResume() { super.onResume(); hideSystemUI(); }
    public void showToast(String msg) { runOnUiThread(() -> Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()); }

    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void toggleDebugMenu();
    public native void nativeSetTurbo(boolean enabled);
}
