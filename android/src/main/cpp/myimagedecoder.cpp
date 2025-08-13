#include <jni.h>
#include <jsi/jsi.h>
#include <string>
#include <vector>

using namespace facebook;

static jclass gInstallerClass = nullptr;
static jmethodID gDecodeMethod = nullptr;
static jclass gResultClass = nullptr;
static jfieldID gResultData = nullptr;
static jfieldID gResultWidth = nullptr;
static jfieldID gResultHeight = nullptr;

static void ensureJNIRefs(JNIEnv* env) {
  if (!gInstallerClass) {
    gInstallerClass = (jclass)env->NewGlobalRef(env->FindClass("com/myimagedecoder/MyImageDecoderJSIInstaller"));
    gDecodeMethod = env->GetStaticMethodID(
      gInstallerClass,
      "decodeImage",
      "([BZ)Lcom/myimagedecoder/DecodeResult;");
  }
  if (!gResultClass) {
    gResultClass = (jclass)env->NewGlobalRef(env->FindClass("com/myimagedecoder/DecodeResult"));
    gResultData = env->GetFieldID(gResultClass, "data", "[B");
    gResultWidth = env->GetFieldID(gResultClass, "width", "I");
    gResultHeight = env->GetFieldID(gResultClass, "height", "I");
  }
}

static jsi::Value installDecode(jsi::Runtime& rt, JNIEnv* env) {
  auto fn = jsi::Function::createFromHostFunction(
    rt,
    jsi::PropNameID::forAscii(rt, "__mid_decodeImage"),
    2,
    [env](jsi::Runtime& rt, const jsi::Value&, const jsi::Value* args, size_t count) -> jsi::Value {
      if (count < 2 || !args[0].isObject() || !args[1].isBool()) {
        throw jsi::JSError(rt, "my-image-decoder: expected (Uint8Array, boolean)");
      }

      // args[0]: Uint8Array -> беремо buffer, byteOffset, byteLength
      auto u8 = args[0].asObject(rt);
      auto bufObj = u8.getPropertyAsObject(rt, "buffer");
      auto ab = bufObj.getArrayBuffer(rt);

      size_t byteOffset = (size_t)u8.getProperty(rt, "byteOffset").asNumber();
      size_t byteLength = (size_t)u8.getProperty(rt, "byteLength").asNumber();

      const uint8_t* start = (const uint8_t*)ab.data(rt) + byteOffset;

      // Копіюємо у jbyteArray
      jbyteArray jIn = env->NewByteArray(byteLength);
      env->SetByteArrayRegion(jIn, 0, (jsize)byteLength, (const jbyte*)start);

      jboolean premul = (jboolean)args[1].getBool();

      // Java: DecodeResult decodeImage(byte[] data, boolean premultiplyAlpha)
      jobject jRes = env->CallStaticObjectMethod(gInstallerClass, gDecodeMethod, jIn, premul);
      env->DeleteLocalRef(jIn);

      if (!jRes) {
        return jsi::Value::null();
      }

      // Витягуємо поля
      jbyteArray jData = (jbyteArray)env->GetObjectField(jRes, gResultData);
      jint width = env->GetIntField(jRes, gResultWidth);
      jint height = env->GetIntField(jRes, gResultHeight);

      jsize outLen = env->GetArrayLength(jData);
      jbyte* outPtr = env->GetByteArrayElements(jData, nullptr);

      // Створюємо ArrayBuffer та Uint8Array для RGBA
      jsi::ArrayBuffer outAB = jsi::ArrayBuffer(rt, (size_t)outLen);
      memcpy(outAB.data(rt), outPtr, (size_t)outLen);
      env->ReleaseByteArrayElements(jData, outPtr, 0);

      // JS: new Uint8Array(outAB)
      auto u8Ctor = rt.global().getPropertyAsFunction(rt, "Uint8Array");
      auto u8Out = u8Ctor.callAsConstructor(rt, (double)outLen);
      // Заповнити з ArrayBuffer простіше через set
      // u8Out.set(...) в JSI напряму недоступний, тому створимо з ArrayBuffer конструктором:
      // new Uint8Array(outAB) — зробимо так:
      auto u8FromAB = u8Ctor.callAsConstructor(rt, outAB);

      // Повертаємо { data: Uint8Array, width, height }
      jsi::Object obj(rt);
      obj.setProperty(rt, "data", u8FromAB);
      obj.setProperty(rt, "width", (double)width);
      obj.setProperty(rt, "height", (double)height);

      return obj;
    }
  );

  rt.global().setProperty(rt, "__mid_decodeImage", std::move(fn));
  return jsi::Value::undefined();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_myimagedecoder_MyImageDecoderJSIInstaller_install(JNIEnv* env, jclass, jlong jsContextNativePointer) {
  auto rt = reinterpret_cast<jsi::Runtime*>(jsContextNativePointer);
  ensureJNIRefs(env);
  installDecode(*rt, env);
}
