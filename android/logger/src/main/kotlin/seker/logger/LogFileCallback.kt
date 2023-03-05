package seker.logger

/**
 *
 * @author xinjian
 */
abstract class LogFileCallback {

    open fun beforeOpen(logFile: String) {}

    open fun afterOpen(logFile: String) {}

    open fun beforeClose(logFile: String) {}

    open fun afterClose(logFile: String) {}

    open fun expired(logFiles: List<String>) {}
}