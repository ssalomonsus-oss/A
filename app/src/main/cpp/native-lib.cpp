#include <jni.h>
#include "BNM/Loading.hpp"
#include "BNM/Class.hpp"
#include "BNMResolve.hpp"
#include <httplib.h>
#include <json.hpp>


void (*Awake)(void*);
void new_Awake(void* instance) {
    Awake(instance);

    // VERY simple webhook log thing. Probably doesn't work, I'm not sure

    nlohmann::json body{};
    body["content"] = "Some string!";

    httplib::SSLClient cli("discord.com", 443);
    if (auto req = cli.Post("path/to/webhook/btw", body.dump(), "application/json")) {
        BNM_LOG_INFO("Made request");
    }
}

void (*LateUpdate)(void*);
void new_LateUpdate(void* instance) {
    LateUpdate(instance);
}

void OnLoaded() {
    BNM_LOG_INFO("Loaded Successfully");

    // PRO TIP: Use BNM_OBFUSCATE on all the string you use, just in case you want to implement a string obfuscator for later.

    InvokeHook(Class(BNM_OBFUSCATE("GorillaLocomotion"), BNM_OBFUSCATE("Player"), Image(
            BNM_OBFUSCATE("Assembly-CSharp.dll"))).GetMethod(BNM_OBFUSCATE("LateUpdate")), new_LateUpdate, LateUpdate);

    InvokeHook(Class(BNM_OBFUSCATE("GorillaLocomotion"), BNM_OBFUSCATE("Player"), Image(
            BNM_OBFUSCATE("Assembly-CSharp.dll"))).GetMethod(BNM_OBFUSCATE("Awake")), new_Awake, Awake);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    BNM::Loading::AddOnLoadedEvent(OnLoaded);

    BNM::Loading::TryLoadByJNI(env);

    return JNI_VERSION_1_6;
}