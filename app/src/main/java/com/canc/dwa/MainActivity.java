package com.canc.dwa;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
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

    private static final int PICK_ROM_REQUEST = 1;
    private static final int BUTTON_A = 0x80;
    private static final int BUTTON_B = 0x40;
    private static final int BUTTON_SELECT = 0x20;
    private static final int BUTTON_START = 0x10;
    private static final int BUTTON_UP = 0x08;
    private static final int BUTTON_DOWN = 0x04;
    private static final int BUTTON_LEFT = 0x02;
    private static final int BUTTON_RIGHT = 0x01;

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

        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());

        if (isRomExtracted()) {
            startNativeEngine();
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!isEngineRunning) return;

        nativeUpdateSurface(screenBitmap);

        Canvas canvas = gameSurface.getHolder().lockCanvas();
        if (canvas != null) {
            float scale = Math.min((float)gameSurface.getWidth() / 256, (float)gameSurface.getHeight() / 240);
            int w = (int)(256 * scale), h = (int)(240 * scale);
            destRect.set((gameSurface.getWidth()-w)/2, (gameSurface.getHeight()-h)/2, (gameSurface.getWidth()+w)/2, (gameSurface.getHeight()+h)/2);

            canvas.drawColor(0xFF000000);
            canvas.drawBitmap(screenBitmap, null, destRect, null);
            gameSurface.getHolder().unlockCanvasAndPost(canvas);
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    private void startNativeEngine() {
        if (setupContainer != null) setupContainer.setVisibility(View.GONE);
        if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);
        setupTouchControls();
        nativeInitEngine(getFilesDir().getAbsolutePath());
        isEngineRunning = true;
        Choreographer.getInstance().postFrameCallback(this);
    }

    private boolean isRomExtracted() {
        return new File(getFilesDir(), "chr_rom.bin").exists();
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
            byte[] buffer = new byte[8192];
            int length;
            while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);
            os.close(); is.close();

            nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            startNativeEngine();
        } catch (Exception e) {
            Toast.makeText(this, "Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        safeBind(R.id.btn_up, BUTTON_UP);
        safeBind(R.id.btn_down, BUTTON_DOWN);
        safeBind(R.id.btn_left, BUTTON_LEFT);
        safeBind(R.id.btn_right, BUTTON_RIGHT);
        safeBind(R.id.btn_a, BUTTON_A);
        safeBind(R.id.btn_b, BUTTON_B);
        safeBind(R.id.btn_start, BUTTON_START);
        safeBind(R.id.btn_select, BUTTON_SELECT);
    }

    private void safeBind(int resId, int bitmask) {
        View v = findViewById(resId);
        if (v != null) {
            v.setOnTouchListener((v1, event) -> {
                if (event.getAction() == MotionEvent.ACTION_DOWN) injectInput(bitmask, true);
                if (event.getAction() == MotionEvent.ACTION_UP) injectInput(bitmask, false);
                return true;
            });
        }
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder h) {}
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isEngineRunning = false; }

    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
}
