package com.myimagedecoder;

import androidx.annotation.NonNull;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;

/**
 * Модуль-інсталер. Не експортує методів у Bridge — лише тригерить JSI-інсталяцію при ініціалізації.
 */
public class MyImageDecoderModule extends ReactContextBaseJavaModule {
  public MyImageDecoderModule(ReactApplicationContext context) {
    super(context);
  }

  @NonNull
  @Override
  public String getName() {
    return "MyImageDecoderInstaller";
  }

  @Override
  public void initialize() {
    super.initialize();
    try {
      System.loadLibrary("myimagedecoder");
      long jsCtx = getReactApplicationContext().getJavaScriptContextHolder().get();
      if (jsCtx != 0) {
        MyImageDecoderJSIInstaller.install(jsCtx);
      }
    } catch (Throwable t) {
      // Якщо не Hermes/JSI — функція не встановиться (в Expo Go теж не працюватиме)
    }
  }
}
