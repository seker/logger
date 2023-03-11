package seker.logger.android

import android.content.Context
import seker.logger.Log

/**
 *
 * @author xinjian
 */
class Application : android.app.Application() {

    override fun attachBaseContext(base: Context) {
        super.attachBaseContext(base)
        val rootDir = getExternalFilesDir("applog")
        Log.init(rootDir!!.absolutePath, "applog", MyLogFileCallback())
    }

    override fun onCreate() {
        super.onCreate()

        Log.setConsole(true)
//        Log.setMinutes(10)
        Log.setProperty(android.util.Log.DEBUG)
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