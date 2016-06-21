LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -DHAVE_CONFIG_H

ifeq ($(TARGET_ARCH), arm)
	LOCAL_CFLAGS += -DPACKED="__attribute__ ((packed))"
else
	LOCAL_CFLAGS += -DPACKED=""
endif

LOCAL_C_INCLUDES += $(LOCAL_PATH)/regex
LOCAL_C_INCLUDES += $(LOCAL_PATH)/pcre-8.38
LOCAL_C_INCLUDES += $(LOCAL_PATH)/pcre-8.38-generic

LOCAL_SRC_FILES += regex/regex.c
LOCAL_SRC_FILES += pcre-8.38/pcre_byte_order.c
LOCAL_SRC_FILES += pcre-8.38/pcre_compile.c
LOCAL_SRC_FILES += pcre-8.38/pcre_config.c
LOCAL_SRC_FILES += pcre-8.38/pcre_dfa_exec.c
LOCAL_SRC_FILES += pcre-8.38/pcre_exec.c
LOCAL_SRC_FILES += pcre-8.38/pcre_fullinfo.c
LOCAL_SRC_FILES += pcre-8.38/pcre_get.c
LOCAL_SRC_FILES += pcre-8.38/pcre_globals.c
LOCAL_SRC_FILES += pcre-8.38/pcre_jit_compile.c
LOCAL_SRC_FILES += pcre-8.38/pcre_maketables.c
LOCAL_SRC_FILES += pcre-8.38/pcre_newline.c
LOCAL_SRC_FILES += pcre-8.38/pcre_ord2utf8.c
LOCAL_SRC_FILES += pcre-8.38/pcre_printint.c
LOCAL_SRC_FILES += pcre-8.38/pcre_refcount.c
LOCAL_SRC_FILES += pcre-8.38/pcre_string_utils.c
LOCAL_SRC_FILES += pcre-8.38/pcre_study.c
LOCAL_SRC_FILES += pcre-8.38/pcre_tables.c
LOCAL_SRC_FILES += pcre-8.38/pcre_ucd.c
LOCAL_SRC_FILES += pcre-8.38/pcre_valid_utf8.c
LOCAL_SRC_FILES += pcre-8.38/pcre_version.c
LOCAL_SRC_FILES += pcre-8.38/pcre_xclass.c
LOCAL_SRC_FILES += pcre-8.38-generic/pcre_chartables.c

LOCAL_MODULE:= regex
LOCAL_LDLIBS += -ldl -llog

include $(BUILD_SHARED_LIBRARY)

