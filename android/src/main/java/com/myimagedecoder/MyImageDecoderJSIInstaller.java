package com.myimagedecoder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.facebook.proguard.annotations.DoNotStrip;

/**
 * Java-частина: декодування через BitmapFactory, і JSI installer.
 */
@DoNotStrip
public class MyImageDecoderJSIInstaller {
  static {
    // libmyimagedecoder.so завантажується в MyImageDecoderModule.initialize()
  }

  /** Викликається з C++ для інсталяції JSI-API у JS runtime */
  public static native void install(long jsContextNativePointer);

  /** Декодування PNG/JPEG -> RGBA, з опцією premultiplied alpha */
  public static DecodeResult decodeImage(byte[] imageBytes, boolean premultiplyAlpha) {
    BitmapFactory.Options opts = new BitmapFactory.Options();
    opts.inPreferredConfig = Bitmap.Config.ARGB_8888;
    opts.inPremultiplied = premultiplyAlpha;

    Bitmap bmp = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.length, opts);
    if (bmp == null) {
      return null;
    }

    int w = bmp.getWidth();
    int h = bmp.getHeight();
    int[] pixels = new int[w * h];
    bmp.getPixels(pixels, 0, w, 0, 0, w, h);

    byte[] rgba = new byte[w * h * 4];
    for (int i = 0; i < pixels.length; i++) {
      int c = pixels[i];
      rgba[i * 4]     = (byte)((c >> 16) & 0xFF); // R
      rgba[i * 4 + 1] = (byte)((c >> 8)  & 0xFF); // G
      rgba[i * 4 + 2] = (byte)( c        & 0xFF); // B
      rgba[i * 4 + 3] = (byte)((c >> 24) & 0xFF); // A
    }

    return new DecodeResult(rgba, w, h);
  }
}
