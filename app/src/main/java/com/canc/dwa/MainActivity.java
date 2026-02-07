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
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
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

    private View setupContainer, gameContainer;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private boolean isSurfaceReady = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);

        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        Button selectRomBtn = findViewById(R.id.btn_select_rom);
        selectRomBtn.setOnClickListener(v -> openFilePicker());

        // New: Hook up a long press or a specific button for the Native Debug Menu
        Button debugBtn = findViewById(R.id.btn_debug);
        if (debugBtn != null) {
            debugBtn.setOnClickListener(v -> toggleDebugMenu());
        }
    }

    /**
     * CRITICAL: Called by Native C++ via JNI.
     * This allows the background engine thread to send messages to the user.
     */
    public void showToast(final String message) {
        runOnUiThread(() -> {
            Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show();
        });
    }

    // ... [openFilePicker and onActivityResult remain the same] ...

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

            // This calls the native-lib.cpp version we just updated
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            
            if ("Success".equals(result)) {
                // The native side now also triggers a Toast, but we'll keep this as a backup
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
        if (isEngineRunning) return;
        
        runOnUiThread(() -> {
            if (setupContainer != null) setupContainer.setVisibility(View.GONE);
            if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
            setupTouchControls();
        });

        nativeInitEngine(getFilesDir().getAbsolutePath());
        isEngineRunning = true;
        if (isSurfaceReady) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning || !isSurfaceReady) return;

        // Pull current NES frame buffer from C++
        nativeUpdateSurface(screenBitmap);

        SurfaceHolder holder = gameSurface.getHolder();
        Canvas canvas = holder.lockCanvas();
        if (canvas != null) {
            try {
                // Fit 256x240 to screen while maintaining aspect ratio
                float scale = Math.min((float) gameSurface.getWidth() / 256f, 
                                       (float) gameSurface.getHeight() / 240f);
                
                int sw = (int) (256 * scale);
                int sh = (int) (240 * scale);
                int left = (gameSurface.getWidth() - sw) / 2;
                int top = (gameSurface.getHeight() - sh) / 2;

                destRect.set(left, top, left + sw, top + sh);
                canvas.drawColor(0xFF000000); // Black bars
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally {
                holder.unlockCanvasAndPost(canvas);
            }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    // ... [Surface callbacks and bindButton remain the same] ...

    // Native methods
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
    public native void nativePauseEngine();
    public native void nativeResumeEngine();
    public native void toggleDebugMenu(); // Matches the new C++ export
}
