#include <map>
#include <jni.h>

#include "unwindstack/AndroidUnwinder.h"

#include "unwinder.h"


extern "C" {

JNIEXPORT jstring JNICALL
Java_xyz_mufanc_remoteunwinder_MainActivity_getRemoteStack(JNIEnv *env, jobject, jint tid) {
    std::string backtrace;

    auto ru = RemoteUnwinder(tid);

    ru.Attach();

    AndroidUnwinderData data;

    ru.Unwind(data);
    ru.Detach();

    for (const auto& frame : data.frames) {
        backtrace += absl::StrCat(ru.FormatFrame(frame).c_str(), "\n");
    }

    return env->NewStringUTF(backtrace.c_str());
}

}
