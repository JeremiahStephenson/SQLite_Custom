
package org.sqlite.app.customsqlite;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;
import java.io.FileInputStream;
import java.util.Arrays;

import java.lang.InterruptedException;

import org.sqlite.database.sqlite.SQLiteDatabase;
import org.sqlite.database.sqlite.SQLiteStatement;
import org.sqlite.database.sqlite.SQLiteDatabaseCorruptException;
import org.sqlite.database.sqlite.SQLiteOpenHelper;

import android.database.Cursor;
import android.content.Context;

/*
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
*/

import org.sqlite.database.DatabaseErrorHandler;
class DoNotDeleteErrorHandler implements DatabaseErrorHandler {
  private static final String TAG = "DoNotDeleteErrorHandler";
  public void onCorruption(SQLiteDatabase dbObj) {
    Log.e(TAG, "Corruption reported by sqlite on database: " + dbObj.getPath());
  }
}

public class CustomSqlite extends Activity
{
  private TextView myTV;          /* Text view widget */
  private int myNTest;            /* Number of tests attempted */
  private int myNErr;             /* Number of tests failed */

  File DB_PATH;

  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState){
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);
    myTV = (TextView)findViewById(R.id.tv_widget);
  }

  public void report_version(){
    SQLiteDatabase db = null;
    SQLiteStatement st;
    String res;

    db = SQLiteDatabase.openOrCreateDatabase(":memory:", null);
    st = db.compileStatement("SELECT sqlite_version()");
    res = st.simpleQueryForString();

    myTV.append("SQLite version " + res + "\n\n");
  }

  public void test_warning(String name, String warning){
    myTV.append("WARNING:" + name + ": " + warning + "\n");
  }

  public void test_result(String name, String res, String expected){
    myTV.append(name + "... ");
    myNTest++;

    if( res.equals(expected) ){
      myTV.append("ok\n");
    } else {
      myNErr++;
      myTV.append("FAILED\n");
      myTV.append("   res=     \"" + res + "\"\n");
      myTV.append("   expected=\"" + expected + "\"\n");
    }
  }

  /*
  ** Test if the database at DB_PATH is encrypted or not. The db
  ** is assumed to be encrypted if the first 6 bytes are anything
  ** other than "SQLite".
  **
  ** If the test reveals that the db is encrypted, return the string
  ** "encrypted". Otherwise, "unencrypted".
  */
  public String db_is_encrypted() throws Exception {
    FileInputStream in = new FileInputStream(DB_PATH);

    byte[] buffer = new byte[6];
    in.read(buffer, 0, 6);

    String res = "encrypted";
    if( Arrays.equals(buffer, (new String("SQLite")).getBytes()) ){
      res = "unencrypted";
    }
    return res;
  }

  /*
  ** Test that a database connection may be accessed from a second thread.
  */
  public void thread_test_1(){
    SQLiteDatabase.deleteDatabase(DB_PATH);
    final SQLiteDatabase db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);

    String db_path2 = DB_PATH.toString() + "2";

    db.execSQL("CREATE TABLE t1(x, y)");
    db.execSQL("INSERT INTO t1 VALUES (1, 2), (3, 4)");

    Thread t = new Thread( new Runnable() {
      public void run() {
        SQLiteStatement st = db.compileStatement("SELECT sum(x+y) FROM t1");
        String res = st.simpleQueryForString();
        test_result("thread_test_1", res, "10");
      }
    });

    t.start();
    try {
      t.join();
    } catch (InterruptedException e) {
    }
  }

  /*
  ** Test that a database connection may be accessed from a second thread.
  */
  public void thread_test_2(){
    SQLiteDatabase.deleteDatabase(DB_PATH);
    final SQLiteDatabase db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);

    db.execSQL("CREATE TABLE t1(x, y)");
    db.execSQL("INSERT INTO t1 VALUES (1, 2), (3, 4)");

    db.enableWriteAheadLogging();
    db.beginTransactionNonExclusive();
    db.execSQL("INSERT INTO t1 VALUES (5, 6)");

    Thread t = new Thread( new Runnable() {
      public void run() {
        SQLiteStatement st = db.compileStatement("SELECT sum(x+y) FROM t1");
        String res = st.simpleQueryForString();
      }
    });

    t.start();
    String res = "concurrent";

    int i;
    for(i=0; i<20 && t.isAlive(); i++){
      try { Thread.sleep(100); } catch(InterruptedException e) {}
    }
    if( t.isAlive() ){ res = "blocked"; }

    db.endTransaction();
    try { t.join(); } catch(InterruptedException e) {}
    if( SQLiteDatabase.hasCodec() ){
      test_result("thread_test_2", res, "blocked");
    } else {
      test_result("thread_test_2", res, "concurrent");
    }
  }

  /*
  ** Use a Cursor to loop through the results of a SELECT query.
  */
  public void csr_test_2() throws Exception {
    SQLiteDatabase.deleteDatabase(DB_PATH);
    SQLiteDatabase db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);
    String res = "";
    String expect = "";
    int i;
    int nRow = 0;

    db.execSQL("CREATE TABLE t1(x)");
    db.execSQL("BEGIN");
    for(i=0; i<1000; i++){
      db.execSQL("INSERT INTO t1 VALUES ('one'), ('two'), ('three')");
      expect += ".one.two.three";
    }
    db.execSQL("COMMIT");
    Cursor c = db.rawQuery("SELECT x FROM t1", null);
    if( c!=null ){
      boolean bRes;
      for(bRes=c.moveToFirst(); bRes; bRes=c.moveToNext()){
        String x = c.getString(0);
        res = res + "." + x;
      }
    }else{
      test_warning("csr_test_1", "c==NULL");
    }
    test_result("csr_test_2.1", res, expect);

    db.execSQL("BEGIN");
    for(i=0; i<1000; i++){
      db.execSQL("INSERT INTO t1 VALUES (X'123456'), (X'789ABC'), (X'DEF012')");
      db.execSQL("INSERT INTO t1 VALUES (45), (46), (47)");
      db.execSQL("INSERT INTO t1 VALUES (8.1), (8.2), (8.3)");
      db.execSQL("INSERT INTO t1 VALUES (NULL), (NULL), (NULL)");
    }
    db.execSQL("COMMIT");

    c = db.rawQuery("SELECT x FROM t1", null);
    if( c!=null ){
      boolean bRes;
      for(bRes=c.moveToFirst(); bRes; bRes=c.moveToNext()) nRow++;
    }else{
      test_warning("csr_test_1", "c==NULL");
    }
    test_result("csr_test_2.2", "" + nRow, "15000");

    db.close();
  }

  public void csr_test_1() throws Exception {
    SQLiteDatabase.deleteDatabase(DB_PATH);
    SQLiteDatabase db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);
    String res = "";

    db.execSQL("CREATE TABLE t1(x)");
    db.execSQL("INSERT INTO t1 VALUES ('one'), ('two'), ('three')");
    
    Cursor c = db.rawQuery("SELECT x FROM t1", null);
    if( c!=null ){
      boolean bRes;
      for(bRes=c.moveToFirst(); bRes; bRes=c.moveToNext()){
        String x = c.getString(0);
        res = res + "." + x;
      }
    }else{
      test_warning("csr_test_1", "c==NULL");
    }
    test_result("csr_test_1.1", res, ".one.two.three");

    db.close();
    test_result("csr_test_1.2", db_is_encrypted(), "unencrypted");
  }

  public String string_from_t1_x(SQLiteDatabase db){
    String res = "";

    Cursor c = db.rawQuery("SELECT x FROM t1", null);
    boolean bRes;
    for(bRes=c.moveToFirst(); bRes; bRes=c.moveToNext()){
      String x = c.getString(0);
      res = res + "." + x;
    }

    return res;
  }

  /*
  ** If this is a SEE build, check that encrypted databases work.
  */
  public void see_test_1() throws Exception {
    if( !SQLiteDatabase.hasCodec() ) return;

    SQLiteDatabase.deleteDatabase(DB_PATH);
    String res = "";
    
    SQLiteDatabase db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);
    db.execSQL("PRAGMA key = 'secretkey'");

    db.execSQL("CREATE TABLE t1(x)");
    db.execSQL("INSERT INTO t1 VALUES ('one'), ('two'), ('three')");
    
    res = string_from_t1_x(db);
    test_result("see_test_1.1", res, ".one.two.three");
    db.close();

    test_result("see_test_1.2", db_is_encrypted(), "encrypted");

    db = SQLiteDatabase.openOrCreateDatabase(DB_PATH, null);
    db.execSQL("PRAGMA key = 'secretkey'");
    res = string_from_t1_x(db);
    test_result("see_test_1.3", res, ".one.two.three");
    db.close();

    res = "unencrypted";
    try {
      db = SQLiteDatabase.openOrCreateDatabase(DB_PATH.getPath(), null);
      string_from_t1_x(db);
    } catch ( SQLiteDatabaseCorruptException e ){
      res = "encrypted";
    } finally {
      db.close();
    }
    test_result("see_test_1.4", res, "encrypted");

    res = "unencrypted";
    try {
      db = SQLiteDatabase.openOrCreateDatabase(DB_PATH.getPath(), null);
      db.execSQL("PRAGMA key = 'otherkey'");
      string_from_t1_x(db);
    } catch ( SQLiteDatabaseCorruptException e ){
      res = "encrypted";
    } finally {
      db.close();
    }
    test_result("see_test_1.5", res, "encrypted");
  }

  class MyHelper extends SQLiteOpenHelper {
    public MyHelper(Context ctx){
      super(ctx, DB_PATH.getPath(), null, 1);
    }
    public void onConfigure(SQLiteDatabase db){
      db.execSQL("PRAGMA key = 'secret'");
    }
    public void onCreate(SQLiteDatabase db){
      db.execSQL("CREATE TABLE t1(x)");
    }
    public void onUpgrade(SQLiteDatabase db, int iOld, int iNew){
    }
  } 

  /*
  ** If this is a SEE build, check that SQLiteOpenHelper still works.
  */
  public void see_test_2() throws Exception {
    if( !SQLiteDatabase.hasCodec() ) return;
    SQLiteDatabase.deleteDatabase(DB_PATH);

    MyHelper helper = new MyHelper(this);
    SQLiteDatabase db = helper.getWritableDatabase();
    db.execSQL("INSERT INTO t1 VALUES ('x'), ('y'), ('z')");

    String res = string_from_t1_x(db);
    test_result("see_test_2.1", res, ".x.y.z");
    test_result("see_test_2.2", db_is_encrypted(), "encrypted");

    helper.close();
    helper = new MyHelper(this);
    db = helper.getReadableDatabase();
    test_result("see_test_2.3", res, ".x.y.z");

    db = helper.getWritableDatabase();
    test_result("see_test_2.4", res, ".x.y.z");

    test_result("see_test_2.5", db_is_encrypted(), "encrypted");
  }


  public void run_the_tests(View view){
    System.loadLibrary("sqliteX");
    DB_PATH = getApplicationContext().getDatabasePath("test.db");
    DB_PATH.mkdirs();

    myTV.setText("");
    myNErr = 0;
    myNTest = 0;

    try {
      report_version();
      csr_test_1();
      csr_test_2();
      thread_test_1();
      thread_test_2(); 
      see_test_1();
      see_test_2();

      myTV.append("\n" + myNErr + " errors from " + myNTest + " tests\n");
    } catch(Exception e) {
      myTV.append("Exception: " + e.toString() + "\n");
      myTV.append(android.util.Log.getStackTraceString(e) + "\n");
    }
  }
}


