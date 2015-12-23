/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
** Modified to support SQLite extensions by the SQLite developers: 
** sqlite-dev@sqlite.org.
*/

#ifndef _ANDROID_DATABASE_SQLITE_COMMON_H
#define _ANDROID_DATABASE_SQLITE_COMMON_H

#include <jni.h>
#include <JNIHelp.h>

#include <sqlite3.h>

#include <string>

// Special log tags defined in SQLiteDebug.java.
#define SQLITE_LOG_TAG "SQLiteLog"
#define SQLITE_TRACE_TAG "SQLiteStatements"
#define SQLITE_PROFILE_TAG "SQLiteTime"

namespace android {

struct SQLiteConnection {
    // Open flags.
    // Must be kept in sync with the constants defined in SQLiteDatabase.java.
    enum {
        OPEN_READWRITE          = 0x00000000,
        OPEN_READONLY           = 0x00000001,
        OPEN_READ_MASK          = 0x00000001,
        NO_LOCALIZED_COLLATORS  = 0x00000010,
        CREATE_IF_NECESSARY     = 0x10000000,
    };

    sqlite3* const db;
    const int openFlags;
    std::string path;
    std::string label;

    volatile bool canceled;

    SQLiteConnection(sqlite3* db, int openFlags, const std::string& path, const std::string& label) :
            db(db), openFlags(openFlags), path(path), label(label), canceled(false) { }
};


/* throw a SQLiteException with a message appropriate for the error in handle */
void throw_sqlite3_exception(JNIEnv* env, sqlite3* handle);

/* throw a SQLiteException with the given message */
void throw_sqlite3_exception(JNIEnv* env, const char* message);

/* throw a SQLiteException with a message appropriate for the error in handle
   concatenated with the given message
 */
void throw_sqlite3_exception(JNIEnv* env, sqlite3* handle, const char* message);

/* throw a SQLiteException for a given error code */
void throw_sqlite3_exception_errcode(JNIEnv* env, int errcode, const char* message);

void throw_sqlite3_exception(JNIEnv* env, int errcode,
        const char* sqlite3Message, const char* message);

}

#endif // _ANDROID_DATABASE_SQLITE_COMMON_H
