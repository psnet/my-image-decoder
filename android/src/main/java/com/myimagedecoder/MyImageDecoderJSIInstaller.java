package com.myimagedecoder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.facebook.jni.HybridData;
import com.facebook.proguard.annotations.DoNotStrip;
import com.facebook.react.bridge.ReactApplicationContext;

import java.nio.ByteBuffer;

@DoNotStrip
public class MyImageDecoderJSIInstaller {
    static {
        System.loadLibrary("myimagedecoder");
    }

    @DoNotStrip
    private final HybridData mHybridData;

    private MyImageDecoderJSIInstaller(HybridData hd) {
        mHybridData = hd;
    }

    public static native void install(long jsContextNativePointer);

    public static byte[] decodeImage(byte[] imageData) {
        Bitmap bmp = BitmapFactory.decodeByteArray(imageData, 0, imageData.length);
        if (bmp == null) return null;

        int width = bmp.getWidth();
        int height = bmp.getHeight();
        int[] pixels = new int[width * height];
        bmp.getPixels(pixels, 0, width, 0, 0, width, height);

        byte[] rgba = new byte[width * height * 4];
        for (int i = 0; i < pixels.length; i++) {
            int c = pixels[i];
            rgba[i * 4]     = (byte) ((c >> 16) & 0xFF);
            rgba[i * 4 + 1] = (byte) ((c >> 8) & 0xFF);
            rgba[i * 4 + 2] = (byte) (c & 0xFF);
            rgba[i * 4 + 3] = (byte) ((c >> 24) & 0xFF);
        }

        return rgba;
    }
}
