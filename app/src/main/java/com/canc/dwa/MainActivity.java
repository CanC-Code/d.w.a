package com.canc.dwa;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
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

    private View setupContainer, gameContainer, debugOverlay;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private boolean isSurfaceReady = false;
    private boolean isDebugVisible = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Link UI components to XML IDs
        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        debugOverlay = findViewById(R.id.debug_overlay_bg); 

        // Standard NES Resolution Bitmap (256x240)
        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        
        if (gameSurface != null) {
            gameSurface.getHolder().addCallback(this);
        }

        View selectRomBtn = findViewById(R.id.btn_select_rom);
        if (selectRomBtn != null) {
            selectRomBtn.setOnClickListener(v -> openFilePicker());
        }

        View debugBtn = findViewById(R.id.btn_debug);
        if (debugBtn != null) {
            debugBtn.setOnClickListener(v -> toggleDebugUI());
        }
    }

    /**
     * RESTORES THE MENU BEHAVIOR:
     * 1. If Debug is open, close it.
     * 2. If in-game, return to the Setup/ROM picker (the "Back Button Menu").
     * 3. Only exits the app if already on the Setup screen.
     */
    @Override
    public void onBackPressed() {
        if (isDebugVisible) {
            toggleDebugUI(); 
        } else if (isEngineRunning && gameContainer != null && gameContainer.getVisibility() == View.VISIBLE) {
            // Act as a "Menu" button to return to ROM selection
            gameContainer.setVisibility(View.GONE);
            if (setupContainer != null) setupContainer.setVisibility(View.VISIBLE);
        } else {
            super.onBackPressed();
        }
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
            if (uri != null) {
                copyAndExtractRom(uri);
            }
        }
    }

    private void copyAndExtractRom(Uri uri) {
        try {
            InputStream is = getContentResolver().openInputStream(uri);
            File internalRom = new File(getFilesDir(), "base.nes");
            FileOutputStream os = new FileOutputStream(internalRom);

            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            os.close();
            is.close();

            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if ("Success".equals(result)) {
                startNativeEngine();
            } else {
                showToast("Error: " + result);
            }
        } catch (Exception e) {
            Log.e(TAG, "ROM load error", e);
            showToast("Failed to load ROM: " + e.getMessage());
        }
    }

    private void startNativeEngine() {
        // UI transition must happen on the main thread
        runOnUiThread(() -> {
            if (setupContainer != null) setupContainer.setVisibility(View.GONE);
            if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
            setupTouchControls(); 
        });

        if (!isEngineRunning) {
            nativeInitEngine(getFilesDir().getAbsolutePath());
            isEngineRunning = true;
        }

        if (isSurfaceReady) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        // Bind NES Controller bitmasks to UI buttons
        bindButton(R.id.btn_up, 0x08);
        bindButton(R.id.btn_down, 0x04);
        bindButton(R.id.btn_left, 0x02);
        bindButton(R.id.btn_right, 0x01);
        bindButton(R.id.btn_a, 0x80);
        bindButton(R.id.btn_b, 0x40);
        bindButton(R.id.btn_start, 0x10);
        bindButton(R.id.btn_select, 0x20);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int resId, int mask) {
        View button = findViewById(resId);
        if (button == null) return;
        button.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                injectInput(mask, true);
                v.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP ||
                       event.getAction() == MotionEvent.ACTION_CANCEL) {
                injectInput(mask, false);
                v.setPressed(false);
            }
            return true;
        });
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady || gameSurface == null) return;

        // 1. Pull the native pixel buffer into the Bitmap
        nativeUpdateSurface(screenBitmap);

        // 2. Lock the Surface Canvas and render the scaled Bitmap
        SurfaceHolder holder = gameSurface.getHolder();
        Canvas canvas = holder.lockCanvas();
        if (canvas != null) {
            try {
                canvas.drawColor(0xFF000000); // Letterbox background

                float viewWidth = gameSurface.getWidth();
                float viewHeight = gameSurface.getHeight();
                
                // Aspect Ratio Logic (4:3 for Dragon Warrior)
                float scale = Math.min(viewWidth / 256f, viewHeight / 240f);
                int sw = (int) (256 * scale);
                int sh = (int) (240 * scale);
                int left = (int) (viewWidth - sw) / 2;
                int top = (int) (viewHeight - sh) / 2;

                destRect.set(left, top, left + sw, top + sh);
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally {
                holder.unlockCanvasAndPost(canvas);
            }
        }

        // 3. Keep the loop running
        Choreographer.getInstance().postFrameCallback(this);
    }

    private void toggleDebugUI() {
        isDebugVisible = !isDebugVisible;
        toggleDebugMenu(); // NOTIFIES C++ ENGINE
        if (debugOverlay != null) {
            debugOverlay.setVisibility(isDebugVisible ? View.VISIBLE : View.GONE);
        }
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        isSurfaceReady = true;
        if (isEngineRunning) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {}

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        isSurfaceReady = false;
        Choreographer.getInstance().removeFrameCallback(this);
    }

    public void showToast(final String message) {
        runOnUiThread(() -> Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show());
    }

    // --- Native JNI Interface ---
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void toggleDebugMenu();
}
