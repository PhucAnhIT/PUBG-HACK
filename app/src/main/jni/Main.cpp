
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu.h"

#if defined(__aarch64__)
#include <And64InlineHook/And64InlineHook.hpp>
#else

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#include <iostream>
#include <ESP/ESP.h>
#endif

#include "ESP/ESP.h"
#include "ESP/Hacks.h"
#include "ESP/struct.h"
using namespace std;
ESP espOverlay;

struct My_Patches {
} hexPatches;


//Target lib here
#define targetLibName OBFUSCATE("libUE4.so")


extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_DrawOl(JNIEnv *env, jclass, jobject espView,
                                                  jobject canvas) {
    espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()) {
        DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_ESPView_DrawESP(JNIEnv *env, jobject thiz) {
    // TODO: implement DrawESP()
    //DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());

}


extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_MainActivity_Toast(JNIEnv *env, jclass obj, jobject context) {
    MakeToast(env, context, OBFUSCATE("Modded by Sami Gaming"), Toast::LENGTH_LONG);
}



// Usage:
// (Optional feature number)_Toggle_(feature name)
// (Optional feature number)_SeekBar_(feature name)_(min value)_(max value)
// (Optional feature number)_Spinner_(feature name)_(Items e.g. item1,item2,item3)
// (Optional feature number)_Button_(feature name)
// (Optional feature number)_ButtonOnOff_(feature name)
// (Optional feature number)_InputValue_(feature name)
// (Optional feature number)_CheckBox_(feature name)
// (Optional feature number)_RadioButton_(feature name)_(Items e.g. radio1,radio2,radio3)
// RichTextView_(Text with limited HTML support)
// RichWebView_(Full HTML support)
// ButtonLink_(feature name)_(URL/Link here)
// Category_(text)


JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_FEATURES"), //Not counted
            OBFUSCATE("ButtonOnOff_ESP BOX"), //0
            OBFUSCATE("ButtonOnOff_ESP LINE"), //1
            OBFUSCATE("ButtonOnOff_ESP DISTANCE"), //2
            OBFUSCATE("ButtonOnOff_ESP PLAYER HEALTH"), //3
            OBFUSCATE("ButtonOnOff_ESP PLAYER HEAD"), //3
            OBFUSCATE("ButtonOnOff_ESP 360 ALERT"), //4
            OBFUSCATE("ButtonOnOff_ESP SKELETON"), //5
            OBFUSCATE("ButtonOnOff_GRENADE WARNING"), //6
            OBFUSCATE("ButtonOnOff_ESP ITEMS"), //7
            OBFUSCATE("ButtonOnOff_ESP VEHICLES"), //8
            OBFUSCATE("ButtonOnOff_ESP USERNAME"), //9
    };

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean jboolean1, jstring str) {
    //Convert java string to c++
    const char *featureName = env->GetStringUTFChars(featName, 0);
    const char *TextInput;
    if (str != NULL)
        TextInput = env->GetStringUTFChars(str, 0);
    else
        TextInput = "Empty";

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         featureName, value,
         jboolean1, TextInput);

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 0:
            isPlayerBox = jboolean1;
            break;
        case 1:
            isPlayerLine = jboolean1;
            break;
        case 2:
            isPlayerDist = jboolean1;
            break;
        case 3:
            isPlayerHealth = jboolean1;
            break;
        case 4:
            isPlayerHead = jboolean1;
            break;
        case 5:
            isr360Alert = jboolean1;
            break;
        case 6:
            isSkelton1 = jboolean1;
            break;
        case 7:
            isGrenadeWarning = jboolean1;
            break;
        case 8:
            isItems2 = jboolean1;
            break;
        case 9:
            isVehicles = jboolean1;
            break;
        case 10:
            isPlayerename = jboolean1;
            break;
		/* case 11: // IF YOU WANNA CREATE NEW SWITCH 
            NewFeature = jboolean1;
            if (NewFeature) {
                hexPatches.NewFeature.Modify();
            } else {
                hexPatches.NewFeature.Restore();
            }
            break; */	
    }
}
}
}




void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread called"));

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__) //Compile for arm64 lib only

#else //FOR ARM V7A

     /* hexPatches.NewFeature = MemoryPatch::createWithHex(targetLibName,
                                                    string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                    OBFUSCATE("00 00 A0 E3 1E FF 2F E1")); */

    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    return JNI_VERSION_1_6;
}
 */
