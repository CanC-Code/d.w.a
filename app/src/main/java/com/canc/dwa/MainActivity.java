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

    static { System.loadLibrary("dwa"); }

    private static final int PICK_ROM_REQUEST = 1;
    private View gameContainer, setupContainer;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        
        // Ensure bitmap is mutable for JNI access
        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());
        
        // Auto-start if a ROM was already extracted in a previous session
        if (isRomExtracted()) {
            startNativeEngine();
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning) return;

        // 1. Get latest frame from C++
        nativeUpdateSurface(screenBitmap);

        // 2. Draw to SurfaceView
        Canvas canvas = gameSurface.getHolder().lockCanvas();
        if (canvas != null) {
            try {
                // Maintain NES 4:3 Aspect Ratio within the surface
                float scale = Math.min((float)gameSurface.getWidth() / 256, (float)gameSurface.getHeight() / 240);
                int w = (int)(256 * scale);
                int h = (int)(240 * scale);
                int left = (gameSurface.getWidth() - w) / 2;
                int top = (gameSurface.getHeight() - h) / 2;
                
                destRect.set(left, top, left + w, top + h);
                
                canvas.drawColor(0xFF000000); // Black background
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally {
                gameSurface.getHolder().unlockCanvasAndPost(canvas);
            }
        }
        
        // Request next frame
        Choreographer.getInstance().postFrameCallback(this);
    }

    private void startNativeEngine() {
        if (isEngineRunning) return; // Prevent double threads

        if (setupContainer != null) setupContainer.setVisibility(View.GONE);
        if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
        
        setupTouchControls();
        
        // Initialize the C++ side
        nativeInitEngine(getFilesDir().getAbsolutePath());
        
        isEngineRunning = true;
        Choreographer.getInstance().postFrameCallback(this);
    }

    private boolean isRomExtracted() {
        // We consider the ROM "ready" if the CHR bank exists
        return new File(getFilesDir(), "chr_rom.bin").exists();
    }

    private void openFilePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*"); // Some NES files aren't MIME-typed correctly
        startActivityForResult(intent, PICK_ROM_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_ROM_REQUEST && resultCode == Activity.RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) {
                copyAndExtract(uri);
            }
        }
    }

    private void copyAndExtract(Uri uri) {
        try {
            // Open stream from Scoped Storage
            InputStream is = getContentResolver().openInputStream(uri);
            if (is == null) return;

            File internalRom = new File(getFilesDir(), "base.nes");
            FileOutputStream os = new FileOutputStream(internalRom);
            
            byte[] buffer = new byte[16384];
            int length;
            while ((length = is.read(buffer)) > 0) {
                os.write(buffer, 0, length);
            }
            os.close();
            is.close();

            // Extract the .nes file into the bank files the C++ engine expects
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            
            if ("Success".equals(result)) {
                startNativeEngine();
            } else {
                Toast.makeText(this, "Extraction failed: " + result, Toast.LENGTH_LONG).show();
            }
        } catch (Exception e) {
            Log.e("DWA", "File error", e);
            Toast.makeText(this, "Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        // Bitmask values must match the controller_state logic in native-lib.cpp
        safeBind(R.id.btn_up,    0x08); 
        safeBind(R.id.btn_down,  0x04);
        safeBind(R.id.btn_left,  0x02); 
        safeBind(R.id.btn_right, 0x01);
        safeBind(R.id.btn_a,     0x80); 
        safeBind(R.id.btn_b,     0x40);
        safeBind(R.id.btn_start, 0x10); 
        safeBind(R.id.btn_select,0x20);
    }

    private void safeBind(int resId, int bitmask) {
        View v = findViewById(resId);
        if (v != null) {
            v.setOnTouchListener((v1, event) -> {
                int action = event.getAction();
                if (action == MotionEvent.ACTION_DOWN) {
                    injectInput(bitmask, true);
                    v1.setPressed(true);
                } else if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
                    injectInput(bitmask, false);
                    v1.setPressed(false);
                }
                return true;
            });
        }
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder h) {}
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) {
        // Stop the loop when the user exits the app to save battery/prevent crashes
        isEngineRunning = false; 
    }

    // JNI Declarations
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
}
