package xyz.mufanc.remoteunwinder

import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.os.Bundle
import android.os.IBinder
import android.os.Parcel
import android.text.method.ScrollingMovementMethod
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import xyz.mufanc.remoteunwinder.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val connection: ServiceConnection = object : ServiceConnection {
        override fun onServiceConnected(name: ComponentName?, service: IBinder?) {
            if (service == null) return

            val data = Parcel.obtain()
            val reply = Parcel.obtain()

            val pid = try {
                service.transact(1599097156 /* _PID */, data, reply, 0)
                reply.readInt()
            } finally {
                data.recycle()
                reply.recycle()
            }

            Log.i(TAG, "service connected: $service ($pid)")

            binding.run {
                backtrace.text = getRemoteStack(pid)
            }
        }

        override fun onServiceDisconnected(name: ComponentName?) {
            Log.i(TAG, "service disconnected.")
        }
    }

    private external fun getRemoteStack(tid: Int): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        bindService(
            Intent(applicationContext, RemoteService::class.java),
            connection,
            Context.BIND_AUTO_CREATE
        )

        binding = ActivityMainBinding.inflate(layoutInflater)

        binding.run {
            backtrace.movementMethod = ScrollingMovementMethod.getInstance()
            backtrace.setTextIsSelectable(true)
        }

        setContentView(binding.root)
    }

    companion object {

        private const val TAG = "RemoteUnwinder"

        init {
            System.loadLibrary("unwind-remote")
        }
    }
}
