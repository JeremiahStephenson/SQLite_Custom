# SQLite Custom

## Overview

The purpose of this project is to allow an application to use the Android NDK to build a custom version of SQLite to be embedded within the application while still utilizing the standard Java interface on all devices running Android 2.3 or later. 
The custom SQLite database can be further customized by adding additional tokenizers.

## Build

To build this project you will need to download and extract the [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html "Title").

You will then need to modify your local.properties file and add a line that points to your Android NDK directory.

`ndk.dir=/Users/Android/android-ndk-r10d`

The gradle file is configured to ignore the C and C++ files. This is due to an issue with Android Studio not being able to build properly. 
To get around this problem the gradle file calls the ndk-build command manually. For this reason the C and C++ files don't show under the project files. 
The following lines from the gradle file can be commented out to allow for the files to display. They will of course need to be un-commented for Android to build the project properly.
        
    sourceSets.main {
        jniLibs.srcDir 'src/main/libs'
        jni.srcDirs = [] //disable automatic ndk-build call
    }

    // call regular ndk-build(.cmd) script from app directory
    task ndkBuild(type: Exec) {
        Properties properties = new Properties()
        properties.load(project.rootProject.file('local.properties').newDataInputStream())
        def ndkDir = properties.getProperty('ndk.dir')
        if (Os.isFamily(Os.FAMILY_WINDOWS)) {
            commandLine 'ndk-build.cmd', '-C', file('src/main').absolutePath
        } else {
            commandLine ndkDir + '/ndk-build', '-C', file('src/main').absolutePath
        }
    }

    tasks.withType(JavaCompile) {
        compileTask -> compileTask.dependsOn ndkBuild
    }

## Java Interface

Most of the code is taken directly from this [repository](http://www.sqlite.org/android/tree?ci=trunk "Title").

One of the main differences with this repository is that the code has broken up into two separate modules.

1. An app that can be used for testing the custom SQLite database.
2. The library project containing all relevant C, C++, and Java code for the custom SQLite database. This compiles into an aar that can be used in any application.

Some additional classes were pulled from the [api-level-15](http://www.sqlite.org/android/timeline?n=100&r=api-level-15 "Title") branch to allow for compatibility with API 15.

To allow the project to be compatible with API 10 and up the `executeForBlobFileDescriptor(String sql, Object[] bindArgs,
                                                                          CancellationSignal cancellationSignal)` had to be removed.
                                                                          
Other additional changes were implemented to allow for compatibility with API 10 and above including the addition of DatabaseUtils. All other files had their imports adjusted to account for the changes.

If anything is updated in the source [repository](http://www.sqlite.org/android/tree?ci=trunk "Title") the code can be copied over here fairly easily. 
Some import statements might need to be adjusted to point to some of the OS related classes such as `CancellationSignal`.

##SQLite

The initial code for SQLite was pulled from this [repository](http://www.sqlite.org/android/tree?ci=trunk "Title") but the version of SQLite has since been update to 3.8.8.2.
As SQLite is update this can be updated in this repository easily by downloading the amalgamation source from [here](http://www.sqlite.org/download.html "Title"). 
Extract the source from the zip file and copy the updated files into the jni folder. That is all that needs to be done to update the version of SQLite.

## Tokenizers

SQLite comes prepackaged with some built in tokenizers but this project has been set up with the ability to add your own. 

There are currently two different custom tokenizers already included.

1. [SqliteSubstringSearch](https://github.com/haifengkao/SqliteSubstringSearch "Title")
2. [FTS3HTMLTokenizer](https://github.com/stephanheilner/FTS3HTMLTokenizer "Title")

To add more tokenizers follow these steps:

1. Add the source under tokenizers in the jni folder.
2. Create a header file that links to the header file for your tokenizer like the one below.
        
        #include "../../fts3_html_tokenizer.h"
        
        #define HTML_NAME "FTS3HTMLTokenizer"
        
        #define UNICODE0_DLL_EXPORTED __attribute__((__visibility__("default")))
        
        void sqlite3Fts3UnicodeTokenizer(sqlite3_tokenizer_module const **ppModule);
        
        struct sqlite3_api_routines;
        
        UNICODE0_DLL_EXPORTED int sqlite3_extension_init_custom(
           sqlite3 *db,          /* The database connection */
           char **pzErrMsg,      /* Write error messages here */
           const struct sqlite3_api_routines *pApi  /* API methods */
           );
    Define a name for the tokenizer and and change the exported function call to something unique.
3. Open extension.c and add an entry for the exported function such as the one below.

        int sqlite3_extension_init_custom(
              sqlite3 *db,          /* The database connection */
              char **pzErrMsg,      /* Write error messages here */
              const sqlite3_api_routines *pApi  /* API methods */
              )
        {
           const sqlite3_tokenizer_module *tokenizer;
        
           SQLITE_EXTENSION_INIT2(pApi)
        
           sqlite3Fts3UnicodeTokenizer(&tokenizer);
        
           registerExtensionTokenizer(db, CHARACTER_NAME, tokenizer);
        
           return 0;
        }
    
    Add an import statement to the header file you created in step 2.
    Make sure the name of the function matches what you defined in your header file. 
    You also need to change the function being called to grab the tokenizer to the one defined in your tokenizer.
4. Open Tokenizers.mk and add your C files to the list of C files being compiled into the module.

        LOCAL_SRC_FILES += fts3_html_tokenizer.c

5. Follow the steps below for how to use a tokenizer in your project.

    a. Before performing any database operations you must call the following code:
        
        System.loadLibrary("sqliteX"); // loads the custom sqlite library
        System.loadLibrary("tokenizers"); // loads the tokenizer library
        
    b. After opening or creating a database run the following code:
    
        final Cursor load = db.rawQuery("SELECT load_extension(?)", new String[]{"libtokenizers"});
        if (load == null || !load.moveToFirst()) {
            throw new RuntimeException("Unicode Extension load failed!");
        }
        
    c. Now you can create your table that utilizes the tokenizer with the following:
    
        db.execSQL("CREATE VIRTUAL TABLE v1 USING fts3(name, tokenize=FTS3HTMLTokenizer)");
        
          

Any of the current built in custom tokenizers such as the html tokenizer can be updated easily by simple copying in the new files from the above listed repositories.

Refer to the example app project for more about how to use the tokenizers. 





   