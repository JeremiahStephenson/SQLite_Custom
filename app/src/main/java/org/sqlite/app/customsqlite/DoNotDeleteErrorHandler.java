package org.sqlite.app.customsqlite;

import android.util.Log;

import org.sqlite.database.DatabaseErrorHandler;
import org.sqlite.database.sqlite.SQLiteDatabase;

/**
 * Created by jeremiahstephenson on 1/21/15.
 */
public class DoNotDeleteErrorHandler implements DatabaseErrorHandler {
    private static final String TAG = "DoNotDeleteErrorHandler";
    public void onCorruption(SQLiteDatabase dbObj) {
        Log.e(TAG, "Corruption reported by sqlite on database: " + dbObj.getPath());
    }
}
