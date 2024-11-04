package seker.logger.android.ui.main

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import seker.logger.Log

class MainViewModel(application: Application) : AndroidViewModel(application) {

    init {
        Log.i("mainVM", "time:${System.currentTimeMillis()}, application=$application")

        Log.v("v : msg")
        Log.v(RuntimeException("v : tr"))
        Log.v(TAG, "v : msg")
        Log.v(TAG, "v : msg", RuntimeException("v : tr"))
        Log.v(TAG, RuntimeException("v : tr"))

        Log.d("d : msg")
        Log.d(RuntimeException("d : tr"))
        Log.d(TAG, "d : msg")
        Log.d(TAG, "d : msg", RuntimeException("d : tr"))
        Log.d(TAG, RuntimeException("d : tr"))

        Log.i("i : msg")
        Log.i(RuntimeException("i : tr"))
        Log.i(TAG, "i : msg")
        Log.i(TAG, "i : msg", RuntimeException("i : tr"))
        Log.i(TAG, RuntimeException("i : tr"))

        Log.w("w : msg")
        Log.w(RuntimeException("w : tr"))
        Log.w(TAG, "w : msg")
        Log.w(TAG, "w : msg", RuntimeException("w : tr"))
        Log.w(TAG, RuntimeException("w : tr"))

        Log.e("e : msg")
        Log.e(RuntimeException("e : tr"))
        Log.e(TAG, "e : msg")
        Log.e(TAG, "e : msg", RuntimeException("e : tr"))
        Log.e(TAG, RuntimeException("e : tr"))

        Log.f("f : msg")
        Log.f(RuntimeException("f : tr"))
        Log.f(TAG, "f : msg")
        Log.f(TAG, "f : msg", RuntimeException("f : tr"))
        Log.f(TAG, RuntimeException("f : tr"))
    }

    companion object {
        private const val TAG = "MainViewModel"
    }
}