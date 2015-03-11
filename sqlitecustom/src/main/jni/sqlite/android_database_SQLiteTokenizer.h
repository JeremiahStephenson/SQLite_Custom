
#ifndef _ANDROID_DATABASE_SQLITE_TOKENIZER_H
#define _ANDROID_DATABASE_SQLITE_TOKENIZER_H

namespace android {

void copyStopWords(JNIEnv* env, jobject assetManager, const char *dataStr);

}

#endif // _ANDROID_DATABASE_SQLITE_TOKENIZER_H