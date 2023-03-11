package seker.logger.android

import android.content.Context
import seker.logger.Log

/**
 *
 * @author xinjian
 */
class Application : android.app.Application() {

    companion object {
        private const val MINUTES = 20
//        private const val MINUTES = 1
    }

    override fun attachBaseContext(base: Context) {
        super.attachBaseContext(base)
        val rootDir = getExternalFilesDir("applog")
        Log.init(rootDir!!.absolutePath, "applog", MINUTES, MyLogFileCallback())
    }

    override fun onCreate() {
        super.onCreate()

        Log.setConsole(true)
        Log.setPriority(android.util.Log.DEBUG)
        Log.setMinutes(MINUTES)
        Log.setDefaultTag("applog")

        seker.asynctask.logger.Log.setLogger(object: seker.asynctask.logger.Logger() {
            override fun println(priority: Int, tag: String, msg: String): Int {
                return Log.log(priority, tag, msg)
            }
        })

        seker.framework.logger.Log.setLogger(object : seker.framework.logger.Logger() {
            override fun println(priority: Int, tag: String, msg: String): Int {
                return Log.log(priority, tag, msg)
            }
        })
    }
}