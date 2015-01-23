
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# If using SEE, uncomment the following:
LOCAL_CFLAGS += -DSQLITE_HAS_CODEC

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DKHTML_NO_EXCEPTIONS -DGKWQ_NO_JAVA
LOCAL_CFLAGS += -DNO_SUPPORT_JS_BINDING -DQT_NO_WHEELEVENT -DKHTML_NO_XBL
LOCAL_CFLAGS += -U__APPLE__
LOCAL_CFLAGS += -DHAVE_STRCHRNUL=0
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3_PARENTHESIS
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS4_UNICODE61
LOCAL_CFLAGS += -DWITH_STEMMER_porter
LOCAL_CFLAGS += -DWITH_STEMMER_russian
LOCAL_CFLAGS += -DWITH_STEMMER_dutch
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-int-to-pointer-cast
LOCAL_CFLAGS += -Wno-maybe-uninitialized -Wno-parentheses
LOCAL_CPPFLAGS += -Wno-conversion-null

ifeq ($(TARGET_ARCH), arm)
	LOCAL_CFLAGS += -DPACKED="__attribute__ ((packed))"
else
	LOCAL_CFLAGS += -DPACKED=""
endif

LOCAL_SRC_FILES := extension.c
LOCAL_SRC_FILES += fts3_unicodesn.c
LOCAL_SRC_FILES += fts3_unicode2.c
LOCAL_SRC_FILES += libstemmer_c/runtime/api_sq3.c
LOCAL_SRC_FILES += libstemmer_c/runtime/utilities_sq3.c

FILE_LIST += $(wildcard $(LOCAL_PATH)/libstemmer_c/src_c/*.c)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/nativehelper/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libstemmer_c/runtime/
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libstemmer_c/src_c/

LOCAL_MODULE:= unicodesn
LOCAL_SHARED_LIBRARIES := libsqliteX
LOCAL_LDLIBS += -ldl -llog

include $(BUILD_SHARED_LIBRARY)

