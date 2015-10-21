# SQLite Custom

## Overview

The purpose of this project is to use the Android NDK to create a custom version of SQLite to be embedded within an application and run on any Android device with Android API 15 or later.
This library can be further customized by adding additional SQLite tokenizers.

## Build

To build this project you will need to download and extract the [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html "Title").

You will then need to modify your local.properties file and add a line that points to your Android NDK directory.

`ndk.dir=/Users/Android/android-ndk-r10d`

The gradle file is configured to ignore the C and C++ files. This is due to an issue with Android Studio not being able to build properly. 
To get around this problem the gradle file calls the ndk-build command manually. For this reason the C and C++ files don't show under the project files under the 'Android' project perspective.
You can change the perspective to 'Project' and view all the files. The other option is to comment out the following lines in the gradle file to allow for the files to display. They will of course need to be un-commented for Android to build the project properly. It might be better just to change the perspective.
        
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

Most of the code is taken directly from this [repository](http://www.sqlite.org/android/tree?ci=api-level-15 "Title").

One of the main differences with this repository is that the code is broken up into two separate modules.

1. An app that can be used for testing the custom SQLite database.
2. The library project containing all relevant C, C++, and Java code for the custom SQLite database. This compiles into an aar that can be used for any application.

Some customization has been implemented to allow for custom tokenizers. 

##SQLite

The initial code for SQLite was pulled from this [repository](http://www.sqlite.org/android/tree?ci=api-level-15 "Title") but the version of SQLite has since been update to 3.9.1.
As SQLite is update this can be updated in this repository easily by downloading the amalgamation source from [here](http://www.sqlite.org/download.html "Title"). 
Extract the source from the zip file and copy the updated files into the jni folder. That is all that needs to be done to update the version of SQLite.

## Tokenizers

SQLite comes prepackaged with some built in tokenizers but this project has been set up with the ability to add your own. 

There are currently two different custom tokenizers already included.

1. [SqliteSubstringSearch](https://github.com/haifengkao/SqliteSubstringSearch "Title")
2. [FTS3HTMLTokenizer](https://github.com/stephanheilner/FTS3HTMLTokenizer "Title")

To add more tokenizers follow these steps:

1. Add the source under tokenizers in the jni folder.
2. If you haven't already done so then add a function like the one below in your tokenizer to set the module.

        void set_character_tokenizer_module(const sqlite3_tokenizer_module **ppModule){
            *ppModule = &characterTokenizerModule;
        }
    
3. Open extension.h and add the setter function you created in your tokenizer from step 2. You also need to set a name for your tokenizer like below.
   
        #define HTML_NAME "HTMLTokenizer"
        
4. Open android_database_SQLTokenizer.cpp and add the setter function from your tokenizer to the 'if' statement.

        if (strcmp(nameStr, HTML_NAME) == 0) {
            get_html_tokenizer_module(&p, dataStr);
        } else if (strcmp(nameStr, CHARACTER_NAME) == 0) {
            get_character_tokenizer_module(&p);
        } else if (strcmp(nameStr, YOUR_TOKENIZER_NAME) == 0) {
            get_your_tokenizer_module(&p);
        }   
   
5. Open Tokenizers.mk and add your C files to the list of C files being compiled into the module.

        LOCAL_SRC_FILES += fts3_html_tokenizer.c

6. Open Tokenizer.java and add your tokenizer to the list of enums with the correct name defined in step 3.

7. Follow the steps below for how to use a tokenizer in your project.

    a. Before performing any database operations you must call the following code:
        
        System.loadLibrary("sqliteX"); // loads the custom sqlite library
        System.loadLibrary("tokenizers"); // loads the tokenizer library
        
    b. After opening or creating a database run the following code:
    
        final Cursor load = db.rawQuery("SELECT load_extension(?)", new String[]{"libtokenizers"});
        if (load == null || !load.moveToFirst()) {
            throw new RuntimeException("Unicode Extension load failed!");
        }
       
    c. Call `registerTokenizer()` from `SQLiteDatabase` and pass in the tokenizer enum defined in step 6.
        
    d. Now you can create your table that utilizes the tokenizer with the following:
    
        db.execSQL("CREATE VIRTUAL TABLE v1 USING fts3(name, tokenize=HTMLTokenizer)");
        
          

Any of the current built in custom tokenizers such as the html tokenizer can be updated easily by simple copying in the new files from the above listed repositories.

Refer to the example app project for more about how to use the tokenizers. 


License
=======

    Copyright 2015 Jerry Miah.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.




   