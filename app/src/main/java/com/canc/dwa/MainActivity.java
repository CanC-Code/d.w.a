[span_14](start_span)// ... (imports and fields) [cite: 23-32]

    private boolean isRomExtracted() {
        [cite_start]// Must match Bank00.bin created in nativeExtractRom[span_14](end_span)
        File bank = new File(getFilesDir(), "Bank00.bin");
        return bank.exists();
    }

    private void copyAndExtract(Uri uri) {
        try {
            [span_15](start_span)InputStream is = getContentResolver().openInputStream(uri);[span_15](end_span)
            [span_16](start_span)File internalRom = new File(getFilesDir(), "base.nes");[span_16](end_span)
            [span_17](start_span)FileOutputStream os = new FileOutputStream(internalRom);[span_17](end_span)

            byte[] buffer = new byte[8192];
            int length;
            [span_18](start_span)while ((length = is.read(buffer)) > 0) os.write(buffer, 0, length);[span_18](end_span)
            os.close(); is.close();

            [span_19](start_span)// Calling the now-implemented native function[span_19](end_span)
            String result = nativeExtractRom(internalRom.getAbsolutePath(), getFilesDir().getAbsolutePath());
            if (result.startsWith("Success")) {
                [span_20](start_span)startNativeEngine();[span_20](end_span)
            } else {
                [span_21](start_span)Toast.makeText(this, result, Toast.LENGTH_LONG).show();[span_21](end_span)
            }
        } catch (Exception e) {
            [span_22](start_span)Toast.makeText(this, "IO Error: " + e.getMessage(), Toast.LENGTH_SHORT).show();[span_22](end_span)
        }
    }
[span_23](start_span)[span_24](start_span)// ... [cite: 62-66]
