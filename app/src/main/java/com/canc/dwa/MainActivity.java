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
        [span_4](start_span)// Load the native library compiled via CMake[span_4](end_span)
        System.loadLibrary("dwa");
    }

    private static final int PICK_ROM_REQUEST = 1;

    [span_5](start_span)[span_6](start_span)// NES Controller Bitmask values[span_5](end_span)[span_6](end_span)
    private static final int BUTTON_A      = 0x80;
    private static final int BUTTON_B      = 0x40;
    private static final int BUTTON_SELECT = 0x20;
    private static final int BUTTON_START  = 0x10;
    private static final int BUTTON_UP     = 0x08;
    private static final int BUTTON_DOWN   = 0x04;
    private static final int BUTTON_LEFT   = 0x02;
    private static final int BUTTON_RIGHT  = 0x01;

    private View gameContainer;
    private View setupContainer;
    private SurfaceView gameSurface;
    private Bitmap screenBitmap;
    private boolean isEngineRunning = false;
    private final Rect destRect = new Rect();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        [span_7](start_span)setContentView(R.layout.activity_main);[span_7](end_span)

        [span_8](start_span)setupContainer = findViewById(R.id.setup_layout);[span_8](end_span)
        [span_9](start_span)gameContainer = findViewById(R.id.game_layout);[span_9](end_span)
        [span_10](start_span)gameSurface = findViewById(R.id.game_surface);[span_10](end_span)

        [span_11](start_span)// Initialize 256x240 bitmap for native NES resolution[span_11](end_span)
        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        [span_12](start_span)gameSurface.getHolder().addCallback(this);[span_12](end_span)

        [span_13](start_span)Button selectButton = findViewById(R.id.btn_select_rom);[span_13](end_span)
        if (selectButton != null) {
            [span_14](start_span)selectButton.setOnClickListener(v -> openFilePicker());[span_14](end_span)
        }

        [span_15](start_span)// Auto-start if ROM is already present in internal storage[span_15](end_span)
        if (isRomExtracted()) {
            startNativeEngine();
        }
    }

    // --- High-Performance Rendering Loop (V-Sync) ---

    @Override
    public void doFrame(long frameTimeNanos) {
        [span_16](start_span)if (!isEngineRunning) return;[span_16](end_span)

        [span_17](start_span)SurfaceHolder holder = gameSurface.getHolder();[span_17](end_span)
        [span_18](start_span)[span_19](start_span)// Fetch current NES frame pixels from C++ buffer[span_18](end_span)[span_19](end_span)
        nativeUpdateSurface(screenBitmap);

        [span_20](start_span)Canvas canvas = holder.lockCanvas();[span_20](end_span)
        if (canvas != null) {
            [span_21](start_span)int viewWidth = gameSurface.getWidth();[span_21](end_span)
            [span_22](start_span)int viewHeight = gameSurface.getHeight();[span_22](end_span)
            
            [span_23](start_span)// Maintain aspect ratio with letterboxing[span_23](end_span)
            float scale = Math.min((float)viewWidth / 256, (float)viewHeight / 240);
            [span_24](start_span)int w = (int)(256 * scale);[span_24](end_span)
            int h = (int)(240 * scale);
            [span_25](start_span)destRect.set((viewWidth - w) / 2, (viewHeight - h) / 2, (viewWidth + w) / 2, (viewHeight + h) / 2);[span_25](end_span)

            canvas.drawColor(0xFF000000); [span_26](start_span)// Black background[span_26](end_span)
            [span_27](start_span)canvas.drawBitmap(screenBitmap, null, destRect, null);[span_27](end_span)
            [span_28](start_span)holder.unlockCanvasAndPost(canvas);[span_28](end_span)
        }

        [span_29](start_span)// Request next frame sync[span_29](end_span)
        Choreographer.getInstance().postFrameCallback(this);
    }

    private void startNativeEngine() {
        [span_30](start_span)if (setupContainer != null) setupContainer.setVisibility(View.GONE);[span_30](end_span)
        [span_31](start_span)if (gameContainer != null) gameContainer.setVisibility(View.VISIBLE);[span_31](end_span)

        [span_32](start_span)applyImmersiveMode();[span_32](end_span)
        [span_33](start_span)setupTouchControls();[span_33](end_span)

        [span_34](start_span)[span_35](start_span)// Pass internal files directory for save paths and ROM loading[span_34](end_span)[span_35](end_span)
        nativeInitEngine(getFilesDir().getAbsolutePath());
        [span_36](start_span)isEngineRunning = true;[span_36](end_span)
        [span_37](start_span)Choreographer.getInstance().postFrameCallback(this);[span_37](end_span)
    }

    @Override public void surfaceCreated(@NonNull SurfaceHolder holder) {}
    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { 
        [span_38](start_span)isEngineRunning = false;[span_38](end_span)
        [span_39](start_span)Choreographer.getInstance().removeFrameCallback(this);[span_39](end_span)
    }

    // --- Touch Input Handling ---

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        [span_40](start_span)[span_41](start_span)// Safe binding to handle IDs that may or may not exist in XML[span_40](end_span)[span_41](end_span)
        safeBind(R.id.btn_up, BUTTON_UP);
        safeBind(R.id.btn_down, BUTTON_DOWN);
        safeBind(R.id.btn_left, BUTTON_LEFT);
        safeBind(R.id.btn_right, BUTTON_RIGHT);
        safeBind(R.id.btn_a, BUTTON_A);
        safeBind(R.id.btn_b, BUTTON_B);
        [span_42](start_span)safeBind(R.id.btn_start, BUTTON_START);[span_42](end_span)
        [span_43](start_span)safeBind(R.id.btn_select, BUTTON_SELECT);[span_43](end_span)
    }

    private void safeBind(int resId, int bitmask) {
        try {
            [span_44](start_span)View v = findViewById(resId);[span_44](end_span)
            if (v != null) {
                [span_45](start_span)bindButton(v, bitmask);[span_45](end_span)
            }
        [span_46](start_span)} catch (Exception ignored) {}[span_46](end_span)
    }

    private void bindButton(View btn, int bitmask) {
        btn.setOnTouchListener((v, event) -> {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    [span_47](start_span)injectInput(bitmask, true);[span_47](end_span)
                    [span_48](start_span)v.setPressed(true);[span_48](end_span)
                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    [span_49](start_span)injectInput(bitmask, false);[span_49](end_span)
                    [span_50](start_span)v.setPressed(false);[span_50](end_span)
                    break;
            }
            return true;
        });
    }

    // --- File Handling ---

    private void openFilePicker() {
        [span_51](start_span)Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);[span_51](end_span)
        [span_52](start_span)intent.addCategory(Intent.CATEGORY_OPENABLE);[span_52](end_span)
        intent.setType("*/*"); 
        [span_53](start_span)startActivityForResult(intent, PICK_ROM_REQUEST);[span_53](end_span)
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        [span_54](start_span)super.onActivityResult(requestCode, resultCode, data);[span_54](end_span)
        if (requestCode == PICK_ROM_REQUEST && resultCode == Activity.RESULT_OK && data != null) {
            [span_55](start_span)Uri uri = data.getData();[span_55](end_span)
            [span_56](start_span)if (uri != null) copyAndExtract(uri);[span_56](end_span)
        }
    }

    private void copyAndExtract(Uri uri) {
        try {
            [span_57](start_span)InputStream is = getContentResolver().openInputStream(uri);[span_57](end_span)
            [span_58](start_span)File internalRom = new File(getFilesDir(), "base.nes");[span_58](end_span)
            [span_59](start_span)FileOutputStream os = new FileOutputStream(internalRom);[span_59](end_span)

            byte[] buffer = new byte[8192];
            int length;
            [span_60](start_span)while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);[span_60](end_span)
            [span_61](start_span)os.close(); is.close();[span_61](end_span)

            [span_62](start_span)[span_63](start_span)// Trigger C++ ROM splitter[span_62](end_span)[span_63](end_span)
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if (result.startsWith("Success")) {
                [span_64](start_span)startNativeEngine();[span_64](end_span)
            } else {
                [span_65](start_span)Toast.makeText(this, result, Toast.LENGTH_LONG).show();[span_65](end_span)
            }
        } catch (Exception e) {
            [span_66](start_span)Toast.makeText(this, "IO Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();[span_66](end_span)
        }
    }

    private boolean isRomExtracted() {
        [span_67](start_span)// Matches the Bank naming produced by Ophis or native extractor[span_67](end_span)
        return new File(getFilesDir(), "Bank00.bin").exists();
    }

    private void applyImmersiveMode() {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            [span_68](start_span)final WindowInsetsController controller = getWindow().getInsetsController();[span_68](end_span)
            if (controller != null) {
                [span_69](start_span)controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());[span_69](end_span)
                [span_70](start_span)controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);[span_70](end_span)
            }
        }
    }

    [span_71](start_span)// --- JNI Signatures[span_71](end_span) ---
    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
}
