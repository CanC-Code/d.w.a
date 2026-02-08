package com.canc.dwa;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "DWA_Main";
    private static final int REQUEST_CODE_PICK_ROM = 1;
    private static final int REQUEST_CODE_PERMISSIONS = 2;
    
    // Native methods
    private native boolean loadROM(byte[] romData);
    private native void runFrame(Bitmap bitmap);
    private native void setButton(int button, boolean pressed);
    private native void cleanup();
    
    // Button constants (must match Controller.cpp)
    private static final int BTN_A = 0x01;
    private static final int BTN_B = 0x02;
    private static final int BTN_SELECT = 0x04;
    private static final int BTN_START = 0x08;
    private static final int BTN_UP = 0x10;
    private static final int BTN_DOWN = 0x20;
    private static final int BTN_LEFT = 0x40;
    private static final int BTN_RIGHT = 0x80;
    
    static {
        System.loadLibrary("dwa");
    }
    
    private ImageView screenView;
    private Bitmap frameBitmap;
    private boolean isRunning = false;
    private Thread emulatorThread;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        screenView = findViewById(R.id.screenView);
        frameBitmap = Bitmap.createBitmap(256, 240, Bitmap.Config.ARGB_8888);
        
        // Set up controller buttons
        setupButton(R.id.btnUp, BTN_UP);
        setupButton(R.id.btnDown, BTN_DOWN);
        setupButton(R.id.btnLeft, BTN_LEFT);
        setupButton(R.id.btnRight, BTN_RIGHT);
        setupButton(R.id.btnA, BTN_A);
        setupButton(R.id.btnB, BTN_B);
        setupButton(R.id.btnStart, BTN_START);
        setupButton(R.id.btnSelect, BTN_SELECT);
        
        // Load ROM button
        findViewById(R.id.btnLoadRom).setOnClickListener(v -> {
            if (checkPermissions()) {
                pickRomFile();
            }
        });
        
        // Auto-load bundled ROM if exists
        loadBundledROM();
    }
    
    private void setupButton(int viewId, int buttonMask) {
        View button = findViewById(viewId);
        if (button != null) {
            button.setOnTouchListener((v, event) -> {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        setButton(buttonMask, true);
                        v.setAlpha(0.5f);
                        return true;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                        setButton(buttonMask, false);
                        v.setAlpha(1.0f);
                        return true;
                }
                return false;
            });
        }
    }
    
    private boolean checkPermissions() {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.TIRAMISU) {
            // Android 13+ doesn't need storage permissions for document picker
            return true;
        }
        
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    REQUEST_CODE_PERMISSIONS);
            return false;
        }
        return true;
    }
    
    private void pickRomFile() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        intent.putExtra(Intent.EXTRA_MIME_TYPES, new String[]{"application/octet-stream", "*/*"});
        startActivityForResult(intent, REQUEST_CODE_PICK_ROM);
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        
        if (requestCode == REQUEST_CODE_PICK_ROM && resultCode == RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) {
                loadROMFromUri(uri);
            }
        }
    }
    
    private void loadBundledROM() {
        try {
            InputStream is = getAssets().open("dragon_warrior.nes");
            byte[] romData = readInputStream(is);
            is.close();
            
            if (loadROM(romData)) {
                Log.i(TAG, "Bundled ROM loaded successfully");
                startEmulator();
            } else {
                Log.e(TAG, "Failed to load bundled ROM");
            }
        } catch (Exception e) {
            Log.w(TAG, "No bundled ROM found: " + e.getMessage());
            Toast.makeText(this, "Please select a Dragon Warrior ROM", Toast.LENGTH_LONG).show();
        }
    }
    
    private void loadROMFromUri(Uri uri) {
        try {
            InputStream is = getContentResolver().openInputStream(uri);
            byte[] romData = readInputStream(is);
            is.close();
            
            if (loadROM(romData)) {
                Toast.makeText(this, "ROM loaded!", Toast.LENGTH_SHORT).show();
                startEmulator();
            } else {
                Toast.makeText(this, "Failed to load ROM", Toast.LENGTH_SHORT).show();
            }
        } catch (Exception e) {
            Log.e(TAG, "Error loading ROM", e);
            Toast.makeText(this, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }
    
    private byte[] readInputStream(InputStream is) throws Exception {
        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        byte[] data = new byte[16384];
        int bytesRead;
        
        while ((bytesRead = is.read(data, 0, data.length)) != -1) {
            buffer.write(data, 0, bytesRead);
        }
        
        return buffer.toByteArray();
    }
    
    private void startEmulator() {
        if (isRunning) return;
        
        isRunning = true;
        emulatorThread = new Thread(() -> {
            Log.i(TAG, "Emulator thread started");
            
            while (isRunning) {
                long frameStart = System.nanoTime();
                
                // Run one frame of emulation
                runFrame(frameBitmap);
                
                // Update screen on UI thread
                runOnUiThread(() -> screenView.setImageBitmap(frameBitmap));
                
                // Target 60 FPS (16.67ms per frame)
                long frameTime = (System.nanoTime() - frameStart) / 1_000_000;
                long sleepTime = 16 - frameTime;
                
                if (sleepTime > 0) {
                    try {
                        Thread.sleep(sleepTime);
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }
            
            Log.i(TAG, "Emulator thread stopped");
        });
        
        emulatorThread.start();
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        isRunning = false;
        if (emulatorThread != null) {
            try {
                emulatorThread.join(1000);
            } catch (InterruptedException e) {
                Log.e(TAG, "Error stopping emulator thread", e);
            }
        }
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        if (frameBitmap != null && !isRunning) {
            // Restart emulator if ROM was previously loaded
            startEmulator();
        }
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        isRunning = false;
        cleanup();
        if (frameBitmap != null) {
            frameBitmap.recycle();
            frameBitmap = null;
        }
    }
}