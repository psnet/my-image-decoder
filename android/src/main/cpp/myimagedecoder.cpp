#include <jni.h>
#include <jsi/jsi.h>
#include <string>
#include <vector>
#include <android/bitmap.h>
#include <android/log.h>

using namespace facebook;

extern "C"
JNIEXPORT void JNICALL
Java_com_myimagedecoder_MyImageDecoderJSIInstaller_install(JNIEnv *env, jclass, jlong jsContextNativePointer) {
    auto runtime = reinterpret_cast<jsi::Runtime*>(jsContextNativePointer);

    auto decodeImageFn = jsi::Function::createFromHostFunction(
        *runtime,
        jsi::PropNameID::forAscii(*runtime, "decodeImage"),
        1,
        [env](jsi::Runtime& rt, const jsi::Value&, const jsi::Value* args, size_t count) -> jsi::Value {
            if (count < 1 || !args[0].isObject()) {
                throw jsi::JSError(rt, "Expected Uint8Array");
            }

            auto uint8Array = args[0].asObject(rt);
            auto buffer = uint8Array.getArrayBuffer(rt);
            size_t length = buffer.size(rt);

            std::vector<uint8_t> input(length);
            memcpy(input.data(), buffer.data(rt), length);

            // Виклик Java методу decodeImage
            jclass cls = env->FindClass("com/myimagedecoder/MyImageDecoderJSIInstaller");
            jmethodID mid = env->GetStaticMethodID(cls, "decodeImage", "([B)[B");

            jbyteArray jIn = env->NewByteArray(length);
            env->SetByteArrayRegion(jIn, 0, length, (jbyte*)input.data());

            jbyteArray jOut = (jbyteArray)env->CallStaticObjectMethod(cls, mid, jIn);
            env->DeleteLocalRef(jIn);

            if (!jOut) {
                return jsi::Value::null();
            }

            jsize outLen = env->GetArrayLength(jOut);
            jbyte* outData = env->GetByteArrayElements(jOut, nullptr);

            auto arrayBuffer = jsi::ArrayBuffer(rt, outLen);
            memcpy(arrayBuffer.data(rt), outData, outLen);
            env->ReleaseByteArrayElements(jOut, outData, 0);

            auto uint8ArrayClass = rt.global().getPropertyAsFunction(rt, "Uint8Array");
            auto uint8ArrayObj = uint8ArrayClass.callAsConstructor(rt, arrayBuffer);

            return uint8ArrayObj;
        }
    );

    runtime->global().setProperty(*runtime, "decodeImage", std::move(decodeImageFn));
}
