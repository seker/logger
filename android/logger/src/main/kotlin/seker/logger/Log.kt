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

    fun init(rootDir: String, baseLogFileName: String, minutes: Int, callback: LogFileCallback) {
        val logFileHandler = LogFileHandler(rootDir, baseLogFileName, minutes, callback)
        LogJni.init(rootDir, baseLogFileName, minutes, logFileHandler)
    }

    fun setConsole(console: Boolean) {
        LogJni.setConsole(console)
    }

    fun setPriority(priority: Int) {
        PRIORITY = priority
        LogJni.setPriority(priority)
    }

    fun setMinutes(minutes: Int) {
        LogJni.setMinutes(minutes)
    }

    fun log(priority: Int, tag: String, msg: String? = null, tr: Throwable? = null) : Int {
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

    fun v(msg: String): Int {
        return v(TAG, msg, null)
    }

    fun v(tr: Throwable): Int {
        return v(TAG, null, tr)
    }

    fun v(tag: String = TAG, msg: String): Int {
        return v(tag, msg, null)
    }

    fun v(tag: String = TAG, tr: Throwable): Int {
        return v(tag, null, tr)
    }

    fun v(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= VERBOSE) {
            log(VERBOSE, tag, msg, tr)
        } else {
            -1
        }
    }

    fun d(msg: String): Int {
        return d(TAG, msg, null)
    }

    fun d(tr: Throwable): Int {
        return d(TAG, null, tr)
    }

    fun d(tag: String = TAG, msg: String): Int {
        return d(tag, msg, null)
    }

    fun d(tag: String = TAG, tr: Throwable? = null): Int {
        return d(tag, null, tr)
    }

    fun d(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= DEBUG) {
            log(DEBUG, tag, msg, tr)
        } else {
            -1
        }
    }

    fun i(msg: String): Int {
        return i(TAG, msg, null)
    }

    fun i(tr: Throwable): Int {
        return i(TAG, null, tr)
    }

    fun i(tag: String = TAG, msg: String): Int {
        return i(tag, msg, null)
    }

    fun i(tag: String = TAG, tr: Throwable? = null): Int {
        return i(tag, null, tr)
    }

    fun i(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= INFO) {
            log(INFO, tag, msg, tr)
        } else {
            -1
        }
    }

    fun w(tag: String = TAG, msg: String): Int {
        return w(tag, msg, null)
    }

    fun w(msg: String): Int {
        return w(TAG, msg, null)
    }

    fun w(tr: Throwable): Int {
        return w(TAG, null, tr)
    }

    fun w(tag: String = TAG, tr: Throwable? = null): Int {
        return w(tag, null, tr)
    }

    fun w(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= WARN) {
            log(WARN, tag, msg, tr)
        } else {
            -1
        }
    }

    fun e(msg: String): Int {
        return e(TAG, msg, null)
    }

    fun e(tr: Throwable): Int {
        return e(TAG, null, tr)
    }

    fun e(tag: String = TAG, msg: String): Int {
        return e(tag, msg, null)
    }

    fun e(tag: String = TAG, tr: Throwable? = null): Int {
        return e(tag, null, tr)
    }

    fun e(tag: String = TAG, msg: String? = null, tr: Throwable? = null): Int {
        return if (PRIORITY <= ERROR) {
            log(ERROR, tag, msg, tr)
        } else {
            -1
        }
    }

    fun f(msg: String): Int {
        return f(TAG, msg, null)
    }

    fun f(tr: Throwable): Int {
        return f(TAG, null, tr)
    }

    fun f(tag: String = TAG, msg: String): Int {
        return f(tag, msg, null)
    }

    fun f(tag: String = TAG, tr: Throwable? = null): Int {
        return f(tag, null, tr)
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