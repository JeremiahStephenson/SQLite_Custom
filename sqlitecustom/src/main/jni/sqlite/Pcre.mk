
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# If using SEE, uncomment the following:
LOCAL_CFLAGS += -DSQLITE_HAS_CODEC

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DKHTML_NO_EXCEPTIONS -DGKWQ_NO_JAVA
LOCAL_CFLAGS += -DNO_SUPPORT_JS_BINDING -DQT_NO_WHEELEVENT -DKHTML_NO_XBL
LOCAL_CFLAGS += -U__APPLE__
LOCAL_CFLAGS += -DHAVE_STRCHRNUL=0
LOCAL_CFLAGS += -DCOMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE
LOCAL_CFLAGS += -DSQLITE_ENABLE_LOAD_EXTENSION=1
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3_PARENTHESIS
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3_TOKENIZER
LOCAL_CFLAGS += -DSQLITE_DEFAULT_JOURNAL_SIZE_LIMIT=1048576
LOCAL_CFLAGS += -DSQLITE_POWERSAFE_OVERWRITE=1
LOCAL_CFLAGS += -DSQLITE_DEFAULT_AUTOVACUUM=1
LOCAL_CFLAGS += -DSQLITE_ENABLE_MEMORY_MANAGEMENT=1
LOCAL_CFLAGS += -DSQLITE_TEMP_STORE=3
LOCAL_CFLAGS += -DHAVE_CONFIG_H -DSUPPORT_PCRE8
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-int-to-pointer-cast
LOCAL_CFLAGS += -Wno-maybe-uninitialized -Wno-parentheses
LOCAL_CPPFLAGS += -Wno-conversion-null

ifeq ($(TARGET_ARCH), arm)
	LOCAL_CFLAGS += -DPACKED="__attribute__ ((packed))"
else
	LOCAL_CFLAGS += -DPACKED=""
endif

LOCAL_C_INCLUDES += $(LOCAL_PATH)/sqlite3-pcre
LOCAL_C_INCLUDES += $(LOCAL_PATH)/pcre-8.37
LOCAL_C_INCLUDES += $(LOCAL_PATH)/pcre-8.37-generic

LOCAL_SRC_FILES += sqlite3-pcre/pcre.c
LOCAL_SRC_FILES += pcre-8.37/pcre_byte_order.c
LOCAL_SRC_FILES += pcre-8.37/pcre_compile.c
LOCAL_SRC_FILES += pcre-8.37/pcre_config.c
LOCAL_SRC_FILES += pcre-8.37/pcre_dfa_exec.c
LOCAL_SRC_FILES += pcre-8.37/pcre_exec.c
LOCAL_SRC_FILES += pcre-8.37/pcre_fullinfo.c
LOCAL_SRC_FILES += pcre-8.37/pcre_get.c
LOCAL_SRC_FILES += pcre-8.37/pcre_globals.c
LOCAL_SRC_FILES += pcre-8.37/pcre_jit_compile.c
LOCAL_SRC_FILES += pcre-8.37/pcre_maketables.c
LOCAL_SRC_FILES += pcre-8.37/pcre_newline.c
LOCAL_SRC_FILES += pcre-8.37/pcre_ord2utf8.c
LOCAL_SRC_FILES += pcre-8.37/pcre_printint.c
LOCAL_SRC_FILES += pcre-8.37/pcre_refcount.c
LOCAL_SRC_FILES += pcre-8.37/pcre_string_utils.c
LOCAL_SRC_FILES += pcre-8.37/pcre_study.c
LOCAL_SRC_FILES += pcre-8.37/pcre_tables.c
LOCAL_SRC_FILES += pcre-8.37/pcre_ucd.c
LOCAL_SRC_FILES += pcre-8.37/pcre_valid_utf8.c
LOCAL_SRC_FILES += pcre-8.37/pcre_version.c
LOCAL_SRC_FILES += pcre-8.37/pcre_xclass.c
LOCAL_SRC_FILES += pcre-8.37-generic/pcre_chartables.c


LOCAL_MODULE:= pcre
LOCAL_LDLIBS += -ldl -llog

include $(BUILD_SHARED_LIBRARY)

