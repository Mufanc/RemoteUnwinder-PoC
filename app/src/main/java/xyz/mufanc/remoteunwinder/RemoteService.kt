package xyz.mufanc.remoteunwinder

import android.app.Service
import android.content.Intent
import android.os.Binder
import android.os.IBinder

class RemoteService : Service() {
    override fun onBind(intent: Intent?): IBinder {
        return Binder()
    }
}
