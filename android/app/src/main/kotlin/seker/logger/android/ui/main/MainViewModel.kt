package seker.logger.android.ui.main

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import seker.asynctask.AsyncTaskExecutor
import seker.logger.Log
import java.util.concurrent.TimeUnit

class MainViewModel(application: Application) : AndroidViewModel(application) {

    init {
        AsyncTaskExecutor.getInstance().scheduleAtFixedRate({
            Log.i("mainVM", "time:${System.currentTimeMillis()}, application=$application")
        }, "mainVM", 1, 5, TimeUnit.SECONDS)
    }
}