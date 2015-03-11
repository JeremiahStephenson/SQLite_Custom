#define LOG_TAG "SQLiteConnectionTokenizer"

#include <jni.h>
#include <JNIHelp.h>

extern "C" {
#include "tokenizers/extension.h"
}

#include "android_database_SQLiteCommon.h"
#include "android_database_SQLiteTokenizer.h"

#include <sys/stat.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>

#define APPNAME "HTML Tokenizer"

#include <string>

namespace android {

static void nativeRegisterTokenizer(JNIEnv* env, jclass obj, jlong connectionPtr, jstring name, jobject assetManager, jstring data) {
    const char *nameStr = env->GetStringUTFChars(name, NULL);
    const char *dataStr = NULL;

    if (data != NULL) {
        dataStr = env->GetStringUTFChars(data, NULL);
    }

    SQLiteConnection* connection = reinterpret_cast<SQLiteConnection*>(connectionPtr);
    if (connection) {

        const sqlite3_tokenizer_module *p;

        if (strcmp(nameStr, HTML_NAME) == 0) {
            copyStopWords(env, assetManager, dataStr);
            get_html_tokenizer_module(&p, dataStr);
        } else if (strcmp(nameStr, CHARACTER_NAME) == 0) {
            get_character_tokenizer_module(&p);
        }

        if (p != NULL) {
            registerExtensionTokenizer(connection->db, nameStr, p);
        }

        if (dataStr != NULL) {
            androidLog(dataStr);
        }
    }

    env->ReleaseStringUTFChars(name, nameStr);

    if (data != NULL && dataStr != NULL) {
        env->ReleaseStringUTFChars(data, dataStr);
    }
}

void copyStopWords(JNIEnv* env, jobject assetManager, const char *dataStr) {
    if (dataStr == NULL || assetManager == NULL) {
        return;
    }

    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAssetDir* assetDir = AAssetManager_openDir(mgr, "stopwords");
    const char* filename = (const char*)NULL;

    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {

        std::string filePath("stopwords");
        filePath.append("/");
        filePath.append(filename);

        struct stat statbuf;
        int isDir = 0;

        if (stat(dataStr, &statbuf) == -1 && !S_ISDIR(statbuf.st_mode)) {
            androidLog("Creating stopwords directory");
            mkdir(dataStr, S_IRWXU);
        }

        AAsset* asset = AAssetManager_open(mgr, filePath.c_str(), AASSET_MODE_STREAMING);

        if (asset != NULL) {

            std::string buffer(dataStr);
            buffer.append("/");
            buffer.append(filename);

            if (stat (buffer.c_str(), &statbuf) != 0) {
                androidLog(buffer.c_str());
                char buf[BUFSIZ];
                int nb_read = 0;
                FILE* out = fopen(buffer.c_str(), "w");

                // the line above is the problem!!!!
                while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0) {
                    fwrite(buf, nb_read, 1, out);
                }

                fclose(out);
            }
            AAsset_close(asset);
        }
    }
    AAssetDir_close(assetDir);
}

static JNINativeMethod sMethods[] =
{
    { "nativeRegisterTokenizer", "(JLjava/lang/String;Landroid/content/res/AssetManager;Ljava/lang/String;)V",
                (void*)nativeRegisterTokenizer },
};

int register_android_database_SQLiteConnection_Tokenizer(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, 
        "org/sqlite/database/sqlite/SQLiteConnection",
        sMethods, NELEM(sMethods)
    );
}

} // namespace android

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv *env = 0;

  vm->GetEnv((void**)&env, JNI_VERSION_1_4);

  android::register_android_database_SQLiteConnection_Tokenizer(env);

  return JNI_VERSION_1_4;
}



