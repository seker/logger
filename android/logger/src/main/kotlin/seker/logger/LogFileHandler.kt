package seker.logger

import java.io.File
import java.text.ParseException
import java.text.SimpleDateFormat
import java.util.*
import java.util.concurrent.Executor
import java.util.concurrent.Executors

/**
 * @author xinjian
 */
internal class LogFileHandler(
    rootDir: String,
    baseLogFileName: String,
    minutes: Int,
    private val callback: LogFileCallback,
) {
    private val callbackExecutor: Executor = Executors.newSingleThreadExecutor { runnable: Runnable ->
        val thread = Thread(runnable)
        thread.name = "log_file"
        thread
    }

    init {
        indicateExpiredLogFiles(rootDir, baseLogFileName, minutes)
    }

    private fun indicateExpiredLogFiles(rootDir: String, prefix: String, minutes: Int) {
        Log.d(TAG, "indicateExpiredLogFiles(rootDir=$rootDir, prefix=$prefix, minutes=$minutes")
        val root = File(rootDir)
        val files = root.list { _: File, name: String ->
            val ret = name.startsWith(prefix) && name.endsWith(".log")
            if (!ret) {
                Log.w(TAG, "unexpected file(prefix=$prefix): $name")
            }
            ret
        }

        if (null == files) {
            Log.d(TAG, "null == files")
        } else if (files.isEmpty()) {
            Log.d(TAG, "files.isEmpty()")
        } else {
            files.apply {
                Arrays.sort(files)
                Log.i(TAG,"indicateExpiredLogFiles(): ${files.joinToString(prefix = "[", separator = ",", postfix = "]")}")

                val calendar = Calendar.getInstance()
                calendar[Calendar.MILLISECOND] = 0
                calendar[Calendar.SECOND] = 0
                val mm = calendar[Calendar.MINUTE]
                calendar[Calendar.MINUTE] = mm - mm % minutes
                Log.i(TAG, "expired(): to indicate ?? < " + SIMPLE_DATE_FORMAT.format(calendar.time))

                val expiredFiles = files.filter { fileName ->
                    var name = fileName.replace(prefix + "_", "")
                    name = name.substring(0, name.lastIndexOf('.'))
                    var expired = false
                    try {
                        val date: Date? = SIMPLE_DATE_FORMAT.parse(name)
                        expired = calendar.time.after(date)
                    } catch (tr: ParseException) {
                        Log.w(TAG, "name: $name", tr)
                    } finally {
                        Log.d(TAG, "$fileName -> $name -> expired=$expired")
                    }
                    expired
                }.map { fileName ->
                    File(root, fileName).absolutePath
                }

                if (expiredFiles.isNotEmpty()) {
                    try {
                        callback.expired(expiredFiles)
                    } catch (tr: Throwable) {
                        Log.w(TAG, tr = tr)
                    }
                }
            }
        }
    }


    fun beforeOpen(logFile: String) {
        callbackExecutor.execute {
            try {
                callback.beforeOpen(logFile)
            } catch (tr: Throwable) {
                Log.w(TAG, tr = tr)
            }
        }
    }

    fun afterOpen(logFile: String) {
        callbackExecutor.execute {
            try {
                callback.afterOpen(logFile)
            } catch (tr: Throwable) {
                Log.w(TAG, tr = tr)
            }
        }
    }

    fun beforeClose(logFile: String) {
        callbackExecutor.execute {
            try {
                callback.beforeClose(logFile)
            } catch (tr: Throwable) {
                Log.w(TAG, tr = tr)
            }
        }
    }

    fun afterClose(logFile: String) {
        callbackExecutor.execute {
            try {
                callback.afterClose(logFile)
            } catch (tr: Throwable) {
                Log.w(TAG, tr = tr)
            }
        }
    }

    companion object {
        private const val TAG = "LogFile"

        val SIMPLE_DATE_FORMAT = SimpleDateFormat("yyyy-MM-dd_HH-mm", Locale.US)
    }
}