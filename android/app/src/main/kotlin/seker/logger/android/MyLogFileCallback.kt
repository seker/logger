package seker.logger.android

import seker.logger.Log
import seker.logger.LogFileCallback

/**
 *
 * @author xinjian
 */
class MyLogFileCallback: LogFileCallback() {

    companion object {
        private const val TAG = "LogFile"
    }

    override fun beforeOpen(logFile: String) {
        Log.d(TAG, "beforeOpen() : $logFile")
    }

    override fun afterOpen(logFile: String) {
        Log.d(TAG, "afterOpen() : $logFile")
    }

    override fun beforeClose(logFile: String) {
        Log.d(TAG, "beforeClose() : $logFile")
    }

    override fun afterClose(logFile: String) {
        Log.d(TAG, "afterClose() : $logFile")
    }

    override fun expired(logFiles: List<String>) {
        val str = logFiles.joinToString(prefix = "[", separator = ",", postfix = "]")
        Log.d(TAG, "expired() : $str")
    }
}