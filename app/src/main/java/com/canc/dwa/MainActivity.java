package com.canc.dwa;

import android.annotation.SuppressLint;
import android.app.Activity;
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
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Choreographer.FrameCallback {

    static {
        System.loadLibrary("dwa");
    }

    private static final String TAG = "DWA_MainActivity";
    private static final int PICK_ROM_REQUEST = 1;

    // UI & Rendering
    private View setupContainer, gameContainer, debugOverlay;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private final Rect destRect = new Rect();
    private final Paint bitmapPaint = new Paint(Paint.FILTER_BITMAP_FLAG); // Smooth scaling

    // State
    private boolean isEngineRunning = false;
    private boolean isSurfaceReady = false;
    private boolean isDebugVisible = false;
    private boolean isTurboEnabled = false;
    
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

        // UI Event Listeners
        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());
        findViewById(R.id.btn_debug).setOnClickListener(v -> toggleDebugUI());
        
        // Potential QOL Button Hooks (If you add these to your XML)
        View turboBtn = findViewById(R.id.btn_turbo);
        if (turboBtn != null) turboBtn.setOnClickListener(v -> toggleTurbo());
    }

    @Override
    protected void onResume() {
        super.onResume();
        hideSystemUI(); // Keep it fullscreen
        if (isEngineRunning && isSurfaceReady) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        Choreographer.getInstance().removeFrameCallback(this);
    }

    // ============================================================================
    // QOL: IMMERSIVE MODE & HAPTICS
    // ============================================================================

    private void hideSystemUI() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            final WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            // Legacy support for older Android versions
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_FULLSCREEN | 
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            );
        }
    }

    private void performHapticFeedback() {
        if (vibrator != null && vibrator.hasVibrator()) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                vibrator.vibrate(VibrationEffect.createOneShot(15, VibrationEffect.DEFAULT_AMPLITUDE));
            } else {
                vibrator.vibrate(15);
            }
        }
    }

    // ============================================================================
    // GAME ENGINE INTEGRATION
    // ============================================================================

    private void toggleTurbo() {
        isTurboEnabled = !isTurboEnabled;
        nativeSetTurbo(isTurboEnabled);
        showToast("Turbo: " + (isTurboEnabled ? "ON" : "OFF"));
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int resId, int mask) {
        View button = findViewById(resId);
        if (button == null) return;
        button.setOnTouchListener((v, event) -> {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    performHapticFeedback();
                    injectInput(mask, true);
                    v.setPressed(true);
                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    injectInput(mask, false);
                    v.setPressed(false);
                    break;
            }
            return true;
        });
    }

    private void copyAndExtractRom(Uri uri) {
        try (InputStream is = getContentResolver().openInputStream(uri)) {
            File internalRom = new File(getFilesDir(), "base.nes");
            try (FileOutputStream os = new FileOutputStream(internalRom)) {
                byte[] buffer = new byte[8192];
                int read;
                while ((read = is.read(buffer)) != -1) os.write(buffer, 0, read);
            }
            
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if ("Success".equals(result)) {
                setupTouchControls();
                setupContainer.setVisibility(View.GONE);
                gameContainer.setVisibility(View.VISIBLE);
                
                if (!isEngineRunning) {
                    nativeInitEngine(getFilesDir().getAbsolutePath());
                    isEngineRunning = true;
                }
                Choreographer.getInstance().postFrameCallback(this);
            } else {
                showToast(result);
            }
        } catch (Exception e) {
            showToast("Load Failed: " + e.getMessage());
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady) return;

        nativeUpdateSurface(screenBitmap);

        Canvas canvas = gameSurface.getHolder().lockCanvas();
        if (canvas != null) {
            try {
                canvas.drawColor(0xFF000000); // Background for letterboxing
                
                // Calculate 4:3 Aspect Ratio for Dragon Warrior
                float vW = gameSurface.getWidth();
                float vH = gameSurface.getHeight();
                float scale = Math.min(vW / 256f, vH / 240f);
                
                int sw = (int) (256 * scale);
                int sh = (int) (240 * scale);
                int left = (int) (vW - sw) / 2;
                int top = (int) (vH - sh) / 2;

                destRect.set(left, top, left + sw, top + sh);
                canvas.drawBitmap(screenBitmap, null, destRect, bitmapPaint);
            } finally {
                gameSurface.getHolder().unlockCanvasAndPost(canvas);
            }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    // ============================================================================
    // UTILS & JNI
    // ============================================================================

    private void toggleDebugUI() {
        isDebugVisible = !isDebugVisible;
        toggleDebugMenu();
        if (debugOverlay != null) debugOverlay.setVisibility(isDebugVisible ? View.VISIBLE : View.GONE);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder h) { isSurfaceReady = true; }
    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder h) { isSurfaceReady = false; }
    @Override
    public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}

    public void showToast(String msg) {
        runOnUiThread(() -> Toast.makeText(this, msg, Toast.LENGTH_SHORT).show());
    }

    private void openFilePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, PICK_ROM_REQUEST);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        bindButton(R.id.btn_up, 0x08);
        bindButton(R.id.btn_down, 0x04);
        bindButton(R.id.btn_left, 0x02);
        bindButton(R.id.btn_right, 0x01);
        bindButton(R.id.btn_a, 0x80);
        bindButton(R.id.btn_b, 0x40);
        bindButton(R.id.btn_start, 0x10);
        bindButton(R.id.btn_select, 0x20);
    }

    // JNI Declarations
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void toggleDebugMenu();
    public native void nativeSetTurbo(boolean enabled);
    public native void nativeSaveState();
    public native void nativeLoadState();
}
