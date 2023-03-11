package seker.logger

/**
 * @author xinjian
 */
internal object LogJni {

    external fun init(rootDir: String, baseLogFileName: String, minutes: Int, logFileHandler: LogFileHandler): Int

    external fun setConsole(console: Boolean): Int

    external fun setPriority(priority: Int): Int

    external fun setMinutes(minutes: Int): Int

    external fun log(priority: Int, tag: String, msg: String, threadName: String = Thread.currentThread().name): Int

    external fun flush(): Int

    init {
        System.loadLibrary("logger")
    }
}