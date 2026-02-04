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
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.Button;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Choreographer.FrameCallback {

    static {
        [span_8](start_span)System.loadLibrary("dwa");[span_8](end_span)
    }

    private static final int PICK_ROM_REQUEST = 1;
    [span_9](start_span)private static final int BUTTON_A = 0x80;[span_9](end_span)
    [span_10](start_span)private static final int BUTTON_B = 0x40;[span_10](end_span)
    [span_11](start_span)private static final int BUTTON_SELECT = 0x20;[span_11](end_span)
    [span_12](start_span)private static final int BUTTON_START = 0x10;[span_12](end_span)
    [span_13](start_span)private static final int BUTTON_UP = 0x08;[span_13](end_span)
    [span_14](start_span)private static final int BUTTON_DOWN = 0x04;[span_14](end_span)
    [span_15](start_span)private static final int BUTTON_LEFT = 0x02;[span_15](end_span)
    [span_16](start_span)private static final int BUTTON_RIGHT = 0x01;[span_16](end_span)

    private View gameContainer, setupContainer;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        [span_17](start_span)setContentView(R.layout.activity_main);[span_17](end_span)

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);

        [span_18](start_span)screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);[span_18](end_span)
        [span_19](start_span)gameSurface.getHolder().addCallback(this);[span_19](end_span)

        [span_20](start_span)findViewById(R.id.btn_select_rom).setOnClickListener(v -> openFilePicker());[span_20](end_span)

        if (isRomExtracted()) {
            [span_21](start_span)startNativeEngine();[span_21](end_span)
        }
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        [span_22](start_span)if (!isEngineRunning) return;[span_22](end_span)

        [span_23](start_span)nativeUpdateSurface(screenBitmap);[span_23](end_span)

        Canvas canvas = gameSurface.getHolder().lockCanvas();
        if (canvas != null) {
            [span_24](start_span)float scale = Math.min((float)gameSurface.getWidth() / 256, (float)gameSurface.getHeight() / 240);[span_24](end_span)
            int w = (int)(256 * scale), h = (int)(240 * scale);
            [span_25](start_span)destRect.set((gameSurface.getWidth()-w)/2, (gameSurface.getHeight()-h)/2, (gameSurface.getWidth()+w)/2, (gameSurface.getHeight()+h)/2);[span_25](end_span)

            [span_26](start_span)canvas.drawColor(0xFF000000);[span_26](end_span)
            [span_27](start_span)canvas.drawBitmap(screenBitmap, null, destRect, null);[span_27](end_span)
            [span_28](start_span)gameSurface.getHolder().unlockCanvasAndPost(canvas);[span_28](end_span)
        }
        [span_29](start_span)Choreographer.getInstance().postFrameCallback(this);[span_29](end_span)
    }

    private void startNativeEngine() {
        [span_30](start_span)if (setupContainer != null) setupContainer.setVisibility(View.GONE);[span_30](end_span)
        [span_31](start_span)if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);[span_31](end_span)
        [span_32](start_span)setupTouchControls();[span_32](end_span)
        [span_33](start_span)nativeInitEngine(getFilesDir().getAbsolutePath());[span_33](end_span)
        isEngineRunning = true;
        [span_34](start_span)Choreographer.getInstance().postFrameCallback(this);[span_34](end_span)
    }

    private boolean isRomExtracted() {
        // Aligned with native-lib.cpp loading logic
        [span_35](start_span)return new File(getFilesDir(), "prg_bank_0.bin").exists();[span_35](end_span)
    }

    private void openFilePicker() {
        [span_36](start_span)Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);[span_36](end_span)
        [span_37](start_span)intent.addCategory(Intent.CATEGORY_OPENABLE);[span_37](end_span)
        intent.setType("*/*"); 
        [span_38](start_span)startActivityForResult(intent, PICK_ROM_REQUEST);[span_38](end_span)
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_ROM_REQUEST && resultCode == Activity.RESULT_OK && data != null) {
            [span_39](start_span)Uri uri = data.getData();[span_39](end_span)
            [span_40](start_span)if (uri != null) copyAndExtract(uri);[span_40](end_span)
        }
    }

    private void copyAndExtract(Uri uri) {
        try {
            [span_41](start_span)InputStream is = getContentResolver().openInputStream(uri);[span_41](end_span)
            [span_42](start_span)File internalRom = new File(getFilesDir(), "base.nes");[span_42](end_span)
            [span_43](start_span)FileOutputStream os = new FileOutputStream(internalRom);[span_43](end_span)
            byte[] buffer = new byte[8192];
            int length;
            [span_44](start_span)while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);[span_44](end_span)
            os.close(); is.close();
            
            // Extract using the native helper to create prg_bank_X.bin files
            [span_45](start_span)nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());[span_45](end_span)
            startNativeEngine();
        } catch (Exception e) {
            [span_46](start_span)Toast.makeText(this, "Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();[span_46](end_span)
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        [span_47](start_span)safeBind(R.id.btn_up, BUTTON_UP);[span_47](end_span)
        [span_48](start_span)safeBind(R.id.btn_down, BUTTON_DOWN);[span_48](end_span)
        [span_49](start_span)safeBind(R.id.btn_left, BUTTON_LEFT);[span_49](end_span)
        [span_50](start_span)safeBind(R.id.btn_right, BUTTON_RIGHT);[span_50](end_span)
        [span_51](start_span)safeBind(R.id.btn_a, BUTTON_A);[span_51](end_span)
        [span_52](start_span)safeBind(R.id.btn_b, BUTTON_B);[span_52](end_span)
        [span_53](start_span)safeBind(R.id.btn_start, BUTTON_START);[span_53](end_span)
        [span_54](start_span)safeBind(R.id.btn_select, BUTTON_SELECT);[span_54](end_span)
    }

    private void safeBind(int resId, int bitmask) {
        [span_55](start_span)View v = findViewById(resId);[span_55](end_span)
        if (v != null) {
            v.setOnTouchListener((v1, event) -> {
                [span_56](start_span)if (event.getAction() == MotionEvent.ACTION_DOWN) injectInput(bitmask, true);[span_56](end_span)
                [span_57](start_span)if (event.getAction() == MotionEvent.ACTION_UP) injectInput(bitmask, false);[span_57](end_span)
                return true;
            });
        }
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder h) {}
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isEngineRunning = false; }

    [span_58](start_span)public native String nativeExtractRom(String romPath, String outDir);[span_58](end_span)
    [span_59](start_span)public native void nativeInitEngine(String filesDir);[span_59](end_span)
    [span_60](start_span)public native void nativeUpdateSurface(Bitmap bitmap);[span_60](end_span)
    [span_61](start_span)public native void injectInput(int buttonBit, boolean isPressed);[span_61](end_span)
}
