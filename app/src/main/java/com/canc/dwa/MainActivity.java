package com.canc.dwa;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.Button;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    static {
        // This loads the 'dwa' library which includes our updated nativeExtractRom
        System.loadLibrary("dwa");
    }

    private static final int PICK_ROM_REQUEST = 1;
    private View gameContainer;
    private View setupContainer;

    @Override
    protected void onCreate(Bundle savedInstanceState) { // Fixed syntax error here
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupContainer = findViewById(R.id.setup_layout); // Container for the Select Button
        gameContainer = findViewById(R.id.game_layout);   // Container for SurfaceView/Buttons

        Button selectButton = findViewById(R.id.btn_select_rom);
        selectButton.setOnClickListener(v -> openFilePicker());

        // Check if ROM was already extracted previously
        if (isRomExtracted()) {
            startNativeEngine();
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
                copyAndExtract(uri);
            }
        }
    }

    private void copyAndExtract(Uri uri) {
        try {
            InputStream is = getContentResolver().openInputStream(uri);
            File internalRom = new File(getFilesDir(), "base.nes");
            FileOutputStream os = new FileOutputStream(internalRom);

            byte[] buffer = new byte[1024];
            int length;
            while ((length = is.read(buffer)) > 0) {
                os.write(buffer, 0, length);
            }
            os.flush();
            os.close();
            is.close();

            // Native extraction: Returns "Success" or Error message
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            
            if (result.startsWith("Success")) {
                startNativeEngine();
            } else {
                Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            }

        } catch (Exception e) {
            Toast.makeText(this, "IO Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    private boolean isRomExtracted() {
        File check = new File(getFilesDir(), "prg_bank_0.bin");
        return check.exists();
    }

    private void startNativeEngine() {
        setupContainer.setVisibility(View.GONE);
        gameContainer.setVisibility(View.VISIBLE);
        
        // Enter Immersive Fullscreen Mode for a "Perfect" feel
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            final WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        }
        
        Toast.makeText(this, "Initializing Dragon Warrior Engine...", Toast.LENGTH_SHORT).show();
    }

    // JNI Declarations
    public native String nativeExtractRom(String romPath, String outDir);
    public native void injectInput(int buttonBit, boolean isPressed);
}
