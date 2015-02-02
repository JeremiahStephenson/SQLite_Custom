
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# If using SEE, uncomment the following:
LOCAL_CFLAGS += -DSQLITE_HAS_CODEC
LOCAL_CFLAGS += -std=c99

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DKHTML_NO_EXCEPTIONS -DGKWQ_NO_JAVA
LOCAL_CFLAGS += -DNO_SUPPORT_JS_BINDING -DQT_NO_WHEELEVENT -DKHTML_NO_XBL
LOCAL_CFLAGS += -U__APPLE__
LOCAL_CFLAGS += -DHAVE_STRCHRNUL=0
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3_PARENTHESIS
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS4_UNICODE61
LOCAL_CFLAGS += -DWITH_STEMMER_dutch
LOCAL_CFLAGS += -DWITH_STEMMER_danish
LOCAL_CFLAGS += -DWITH_STEMMER_english
LOCAL_CFLAGS += -DWITH_STEMMER_finnish
LOCAL_CFLAGS += -DWITH_STEMMER_french
LOCAL_CFLAGS += -DWITH_STEMMER_german
LOCAL_CFLAGS += -DWITH_STEMMER_hungarian
LOCAL_CFLAGS += -DWITH_STEMMER_italian
LOCAL_CFLAGS += -DWITH_STEMMER_norwegian
LOCAL_CFLAGS += -DWITH_STEMMER_porter
LOCAL_CFLAGS += -DWITH_STEMMER_portuguese
LOCAL_CFLAGS += -DWITH_STEMMER_romanian
LOCAL_CFLAGS += -DWITH_STEMMER_russian
LOCAL_CFLAGS += -DWITH_STEMMER_spanish
LOCAL_CFLAGS += -DWITH_STEMMER_swedish
LOCAL_CFLAGS += -DWITH_STEMMER_turkish
LOCAL_CFLAGS += -DSQLITE_DEFAULT_JOURNAL_SIZE_LIMIT=1048576
LOCAL_CFLAGS += -DSQLITE_POWERSAFE_OVERWRITE=1
LOCAL_CFLAGS += -DSQLITE_ENABLE_MEMORY_MANAGEMENT=1
LOCAL_CFLAGS += -DSQLITE_DEFAULT_AUTOVACUUM=1
LOCAL_CFLAGS += -DSQLITE_TEMP_STORE=3
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-int-to-pointer-cast
LOCAL_CFLAGS += -Wno-maybe-uninitialized -Wno-parentheses
LOCAL_CPPFLAGS += -Wno-conversion-null

ifeq ($(TARGET_ARCH), arm)
	LOCAL_CFLAGS += -DPACKED="__attribute__ ((packed))"
else
	LOCAL_CFLAGS += -DPACKED=""
endif

LOCAL_SRC_FILES := extension.c
LOCAL_SRC_FILES += unicodesn/fts3_unicodesn.c
LOCAL_SRC_FILES += unicodesn/fts3_unicode2.c
LOCAL_SRC_FILES += libstemmer_c/runtime/api_sq3.c
LOCAL_SRC_FILES += libstemmer_c/runtime/utilities_sq3.c

LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_danish.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_dutch.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_english.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_finnish.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_french.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_german.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_hungarian.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_italian.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_norwegian.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_porter.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_portuguese.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_romanian.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_russian.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_spanish.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_swedish.c
LOCAL_SRC_FILES += libstemmer_c/src_c/stem_UTF_8_turkish.c

LOCAL_SRC_FILES += character/character_tokenizer.c
LOCAL_SRC_FILES += html/fts3_html_tokenizer.c

LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/nativehelper/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libstemmer_c/runtime/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libstemmer_c/src_c/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/unicodesn/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/character/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/html/

LOCAL_MODULE:= unicodesn
LOCAL_SHARED_LIBRARIES := libsqliteX
LOCAL_LDLIBS += -ldl -llog

include $(BUILD_SHARED_LIBRARY)

