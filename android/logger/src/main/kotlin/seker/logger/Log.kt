package seker.logger

import android.util.Log.*

/**
 * @author xinjian
 */
object Log {

    private var TAG = "Log"

    private var PRIORITY = VERBOSE

    fun setDefaultTag(tag: String) {
        TAG = tag
    }

    fun init(rootDir: String, baseLogFileName: String, callback: LogFileCallback) {
        LogJni.init(rootDir, baseLogFileName, LogFileHandler(callback))
    }

    fun setConsole(console: Boolean) {
        LogJni.setConsole(console)
    }

    fun setProperty(priority: Int) {
        PRIORITY = priority
        LogJni.setPriority(priority)
    }

    fun setMinutes(minutes: Int) {
        LogJni.setMinutes(minutes)
    }

    fun log(priority: Int, tag: String = TAG, msg: String? = null, tr: Throwable? = null) : Int {
        return if (null == msg)  {
            if (null == tr) {
                -1
            } else {
                LogJni.log(priority, tag, getStackTraceString(tr))
            }
        } else {
            if (null == tr) {
                LogJni.log(priority, tag, msg)
            } else {
                LogJni.log(priority, tag, "$msg\n${getStackTraceString(tr)}".trimIndent())
            }
        }
    }

    fun v(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= VERBOSE) {
            log(VERBOSE, tag, msg, tr)
        } else {
            -1
        }
    }

    fun d(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= DEBUG) {
            log(DEBUG, tag, msg, tr)
        } else {
            -1
        }
    }

    fun i(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= INFO) {
            log(INFO, tag, msg, tr)
        } else {
            -1
        }
    }

    fun w(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= WARN) {
            log(WARN, tag, msg, tr)
        } else {
            -1
        }
    }

    fun e(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= ERROR) {
            log(ERROR, tag, msg, tr)
        } else {
            -1
        }
    }

    fun f(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= ASSERT) {
            log(ASSERT, tag, msg, tr)
        } else {
            -1
        }
    }

    fun flush(reason: String) {
        i(TAG, "flush($reason)")
        LogJni.flush()
    }
}