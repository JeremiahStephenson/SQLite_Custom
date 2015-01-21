package org.sqlite.app.customsqlite.util;

import android.os.Handler;
import android.os.Looper;
import android.support.annotation.NonNull;

import java.util.concurrent.Executor;

/**
 * Created by jeremiahstephenson on 1/21/15.
 */
public class ExecutorUtil {

    public static Executor getThreadExecutor = new Executor() {
        public void execute(@NonNull Runnable command) {
            new Handler(Looper.getMainLooper()).post(command);
        }
    };

}
