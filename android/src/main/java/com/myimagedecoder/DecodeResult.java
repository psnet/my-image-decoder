package com.myimagedecoder;

public class DecodeResult {
  public final byte[] data;
  public final int width;
  public final int height;

  public DecodeResult(byte[] data, int width, int height) {
    this.data = data;
    this.width = width;
    this.height = height;
  }
}
