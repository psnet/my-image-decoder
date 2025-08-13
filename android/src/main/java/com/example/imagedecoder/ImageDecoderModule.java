package com.example.imagedecoder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import expo.modules.core.Promise;
import expo.modules.core.interfaces.ExpoMethod;
import expo.modules.core.BaseModule;

import java.util.ArrayList;
import java.util.HashMap;

public class ImageDecoderModule extends BaseModule {

    public ImageDecoderModule() {
        super();
    }

    @Override
    public String getName() {
        return "ImageDecoderModule";
    }

    @ExpoMethod
    public void decodeByteArray(final ArrayList<Integer> byteArray, boolean premultiplyAlpha, Promise promise) {
        try {
            byte[] bytes = new byte[byteArray.size()];
            for (int i = 0; i < byteArray.size(); i++) {
                bytes[i] = byteArray.get(i).byteValue();
            }

            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inPremultiplied = premultiplyAlpha;
            options.inPreferredConfig = Bitmap.Config.ARGB_8888;

            Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length, options);

            if (bitmap == null) {
                promise.reject("E_DECODE", "Failed to decode image from byte array.");
                return;
            }

            int width = bitmap.getWidth();
            int height = bitmap.getHeight();
            int[] pixels = new int[width * height];
            bitmap.getPixels(pixels, 0, width, 0, 0, width, height);

            byte[] rgba = new byte[width * height * 4];
            for (int i = 0; i < pixels.length; i++) {
                int p = pixels[i];
                rgba[i * 4]     = (byte) ((p >> 16) & 0xFF); // R
                rgba[i * 4 + 1] = (byte) ((p >> 8) & 0xFF);  // G
                rgba[i * 4 + 2] = (byte) (p & 0xFF);         // B
                rgba[i * 4 + 3] = (byte) ((p >> 24) & 0xFF); // A
            }

            HashMap<String, Object> map = new HashMap<>();
            map.put("data", rgba);
            map.put("width", width);
            map.put("height", height);

            promise.resolve(map);

        } catch (Exception e) {
            promise.reject("E_DECODE", e);
        }
    }
}