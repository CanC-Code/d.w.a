package com.canc.dwa;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
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

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("dwa");
    }

    private static final int PICK_ROM_REQUEST = 1;

    // NES Controller Bitmask
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
    private Thread graphicsThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupContainer = findViewById(R.id.setup_layout);
        gameContainer = findViewById(R.id.game_layout);
        gameSurface = findViewById(R.id.game_surface);
        
        // Initialize the 256x240 buffer for the Native PPU
        screenBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        gameSurface.getHolder().addCallback(this);

        Button selectButton = findViewById(R.id.btn_select_rom);
        selectButton.setOnClickListener(v -> openFilePicker());

        if (isRomExtracted()) {
            startNativeEngine();
        }
    }

    // --- SAF Picker & Extraction Logic ---

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

            byte[] buffer = new byte[1024];
            int length;
            while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);
            os.close(); is.close();

            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if (result.startsWith("Success")) startNativeEngine();
            else Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            
        } catch (Exception e) {
            Toast.makeText(this, "IO Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    private boolean isRomExtracted() {
        return new File(getFilesDir(), "prg_bank_0.bin").exists();
    }

    // --- Native Engine Control ---

    private void startNativeEngine() {
        setupContainer.setVisibility(View.GONE);
        gameContainer.setVisibility(View.VISIBLE);

        applyImmersiveMode();
        setupTouchControls();

        // Start C++ Master Clock
        nativeInitEngine(getFilesDir().getAbsolutePath());
        isEngineRunning = true;
    }

    private void applyImmersiveMode() {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            final WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        }
    }

    // --- SurfaceView Callback (Rendering) ---

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        graphicsThread = new Thread(() -> {
            Rect destRect = new Rect();
            while (isEngineRunning) {
                // 1. Update the Bitmap from C++ Screen Buffer
                nativeUpdateSurface(screenBitmap);

                // 2. Draw to Surface
                Canvas canvas = holder.lockCanvas();
                if (canvas != null) {
                    // Maintain aspect ratio: Scale 256x240 to fit screen
                    int viewWidth = gameSurface.getWidth();
                    int viewHeight = gameSurface.getHeight();
                    float scale = Math.min((float)viewWidth / 256, (float)viewHeight / 240);
                    int w = (int)(256 * scale);
                    int h = (int)(240 * scale);
                    destRect.set((viewWidth-w)/2, (viewHeight-h)/2, (viewWidth+w)/2, (viewHeight+h)/2);
                    
                    canvas.drawColor(0xFF000000); // Black Bars
                    canvas.drawBitmap(screenBitmap, null, destRect, null);
                    holder.unlockCanvasAndPost(canvas);
                }
                
                try { Thread.sleep(16); } catch (InterruptedException ignored) {}
            }
        });
        graphicsThread.start();
    }

    @Override public void surfaceChanged(@NonNull SurfaceHolder h, int f, int w, int h1) {}
    @Override public void surfaceDestroyed(@NonNull SurfaceHolder h) { isEngineRunning = false; }

    // --- Input & JNI ---

    @SuppressLint("ClickableViewAccessibility")
    private void setupTouchControls() {
        bindButton(R.id.btn_up, BUTTON_UP);
        bindButton(R.id.btn_down, BUTTON_DOWN);
        bindButton(R.id.btn_left, BUTTON_LEFT);
        bindButton(R.id.btn_right, BUTTON_RIGHT);
        bindButton(R.id.btn_a, BUTTON_A);
        bindButton(R.id.btn_b, BUTTON_B);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void bindButton(int viewId, int bitmask) {
        View btn = findViewById(viewId);
        if (btn != null) {
            btn.setOnTouchListener((v, event) -> {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    injectInput(bitmask, true);
                    v.setPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                    injectInput(bitmask, false);
                    v.setPressed(false);
                }
                return true;
            });
        }
    }

    public native String nativeExtractRom(String romPath, String outDir);
    public native void nativeInitEngine(String filesDir);
    public native void nativeUpdateSurface(Bitmap bitmap);
    public native void injectInput(int buttonBit, boolean isPressed);
}
