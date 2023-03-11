package seker.logger

import java.util.concurrent.Executor
import java.util.concurrent.Executors

/**
 * @author xinjian
 */
internal class LogFileHandler(
    private val callback: LogFileCallback,
) {
    private val callbackExecutor: Executor = Executors.newSingleThreadExecutor { runnable: Runnable ->
        val thread = Thread(runnable)
        thread.name = "log_file"
        thread
    }

    fun beforeOpen(logFile: String) {
        callbackExecutor.execute {
            callback.beforeOpen(logFile)
        }
    }

    fun afterOpen(logFile: String) {
        callbackExecutor.execute {
            callback.afterOpen(logFile)
        }
    }

    fun beforeClose(logFile: String) {
        callbackExecutor.execute {
            callback.beforeClose(logFile)
        }
    }

    fun afterClose(logFile: String) {
        callbackExecutor.execute {
            callback.afterClose(logFile)
        }
    }
}