package seker.logger

/**
 * @author xinjian
 */
object LogJni {

    external fun init(rootDir: String, baseLogFileName: String, callback: LogFileCallback)

    external fun setConsole(console: Boolean)

    external fun setMinutes(minutes: Int)

    external fun setPriority(priority: Int)

    external fun log(priority: Int, tag: String, msg: String, threadName: String = Thread.currentThread().name): Int

    external fun flush()

    init {
        System.loadLibrary("logger_jni")
    }
}