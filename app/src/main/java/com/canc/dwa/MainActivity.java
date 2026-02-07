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

/**
 * MainActivity for Dragon Warrior Android.
 * Handles the UI, ROM extraction, and synchronizes the Java Surface with the Native PPU.
 */
public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Choreographer.FrameCallback {

    // Ensure this matches the name defined in your CMakeLists.txt (usually native-lib)
    static {
        System.loadLibrary("native-lib");
    }

    private static final int PICK_ROM_REQUEST = 1;
    private static final String PREFS_NAME = "ControllerPrefs";
    private static final String TAG = "DWA_Main";

    private View gameContainer, setupContainer;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;

    private boolean isEngineInitialized = false; 
    private boolean isSurfaceReady = false; 
    private boolean isPaused = false;
    private boolean isEditMode = false;
    private boolean isRomLoadedInNative = false;

    private final Rect destRect = new Rect();
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

        // Standard NES resolution: 256x240
        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        SharedPreferences prefs = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        gameScreenYOffset = prefs.getFloat("screen_y_offset", 0);
        gameScaleFactor = prefs.getFloat("screen_scale", 1.0f);

        setupScaleDetector();
        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());

        // Check if a ROM was previously extracted
        File internalRom = new File(getFilesDir(), "base.nes");
        if (internalRom.exists()) {
            new Thread(() -> {
                String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
                if ("Success".equals(result)) {
                    isRomLoadedInNative = true;
                    runOnUiThread(this::startNativeEngine);
                }
            }).start();
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (isPaused || !isSurfaceReady || !isRomLoadedInNative) return;

        // Pull current PPU screen buffer from Native to Java Bitmap
        nativeUpdateSurface(screenBitmap);

        SurfaceHolder holder = gameSurface.getHolder();
        Canvas canvas = holder.lockCanvas();
        if (canvas != null) {
            try {
                canvas.drawColor(0xFF000000); // Black background
                
                // Calculate aspect-ratio correct scaling
                float baseScale = Math.min((float)gameSurface.getWidth() / 256f, (float)gameSurface.getHeight() / 240f);
                float finalScale = baseScale * gameScaleFactor;

                int w = (int)(256 * finalScale);
                int h = (int)(240 * finalScale);
                int left = (gameSurface.getWidth() - w) / 2;
                int top = ((gameSurface.getHeight() - h) / 2) + (int)gameScreenYOffset;

                destRect.set(left, top, left + w, top + h);
                canvas.drawBitmap(screenBitmap, null, destRect, null);
            } finally { 
                holder.unlockCanvasAndPost(canvas); 
            }
        }
        
        // Request next frame
        Choreographer.getInstance().postFrameCallback(this);
    }

    private void startNativeEngine() {
        if (!isEngineInitialized && isRomLoadedInNative) {
            nativeInitEngine(getFilesDir().getAbsolutePath());
            isEngineInitialized = true;
        }

        showGameLayout();

        if (isSurfaceReady && !isPaused) {
            Choreographer.getInstance().removeFrameCallback(this);
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        isPaused = true;
        if (isEngineInitialized) nativePauseEngine();
    }

    @Override
    protected void onResume() {
        super.onResume();
        isPaused = false;
        if (isEngineInitialized) {
            nativeResumeEngine();
            if (isSurfaceReady) {
                Choreographer.getInstance().removeFrameCallback(this);
                Choreographer.getInstance().postFrameCallback(this);
            }
        }
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder h) { 
        isSurfaceReady = true; 
        if (isRomLoadedInNative) startNativeEngine(); 
    }

    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { 
        isSurfaceReady = false; 
        Choreographer.getInstance().removeFrameCallback(this);
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
                new Thread(() -> copyAndExtract(uri)).start();
            }
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

            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if ("Success".equals(result)) {
                isRomLoadedInNative = true;
                runOnUiThread(this::startNativeEngine);
            } else {
                runOnUiThread(() -> Toast.makeText(this, "ROM Error: " + result, Toast.LENGTH_SHORT).show());
            }
        } catch (Exception e) { 
            Log.e(TAG, "File error", e); 
        }
    }

    private void showGameLayout() {
        if (setupContainer != null) setupContainer.setVisibility(View.GONE);
        if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
        setupTouchControls();
    }

    private void setupScaleDetector() {
        scaleDetector = new ScaleGestureDetector(this, new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override public boolean onScale(ScaleGestureDetector d) {
                if (isEditMode) {
                    gameScaleFactor = Math.max(0.5f, Math.min(gameScaleFactor * d.getScaleFactor(), 3.0f));
                    saveLayout();
                    return true;
                }
                return false;
            }
        });
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        bindButton(R.id.btn_up, 0x08, "up");
        bindButton(R.id.btn_down, 0x04, "down");
        bindButton(R.id.btn_left, 0x02, "left");
        bindButton(R.id.btn_right, 0x01, "right");
        bindButton(R.id.btn_a, 0x80, "a");
        bindButton(R.id.btn_b, 0x40, "b");
        bindButton(R.id.btn_start, 0x10, "start");
        bindButton(R.id.btn_select, 0x20, "select");

        gameSurface.setOnTouchListener((v, event) -> {
            scaleDetector.onTouchEvent(event);
            if (isEditMode && event.getAction() == MotionEvent.ACTION_MOVE && !scaleDetector.isInProgress()) {
                gameScreenYOffset = event.getRawY() - (gameSurface.getHeight() / 2f); 
                return true;
            }
            return isEditMode;
        });
    }

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
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                nativeInjectInput(mask, true);
                v1.setPressed(true);
            } else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                nativeInjectInput(mask, false);
                v1.setPressed(false);
            }
            return true;
        });
    }

    private void saveLayout() { getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit().putFloat("screen_y_offset", gameScreenYOffset).putFloat("screen_scale", gameScaleFactor).apply(); }
    private void savePosition(String k, float x, float y) { getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit().putFloat(k + "_x", x).putFloat(k + "_y", y).apply(); }
    private void loadPosition(View v, String k) { SharedPreferences p = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE); float x = p.getFloat(k + "_x", -1); float y = p.getFloat(k + "_y", -1); if (x != -1 && y != -1) { v.setX(x); v.setY(y); } }

    private void resumeGame() {
        isEditMode = false;
        if (!isPaused) nativeResumeEngine();
    }

    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}

    // --- Native JNI Interface (Must match native-lib.cpp exactly) ---
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void nativeInjectInput(int buttonBit, boolean isPressed);
    public native void nativePauseEngine();
    public native void nativeResumeEngine();
}
