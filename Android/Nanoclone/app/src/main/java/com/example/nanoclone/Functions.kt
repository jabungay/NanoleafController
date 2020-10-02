package com.example.nanoclone

import android.app.Activity
import android.graphics.Color
import android.graphics.PorterDuff
import android.graphics.drawable.Drawable
import android.widget.Toast

class Functions {
    fun toast (activity: Activity?, text: String)
    {
        val t: CharSequence = text
        val duration = Toast.LENGTH_SHORT
        val toast = Toast.makeText(activity!!.baseContext, t, duration)
        toast.show()
    }

    fun setDrawableColor(drawable: Drawable, r: Int, g: Int, b: Int, a: Int) {
        val cStr: String = "#" + String.format("%02X", a) + String.format("%02X", r) + String.format("%02X", g) + String.format("%02X", b)
        val c: Int = Color.parseColor(cStr)

        drawable.setColorFilter(c, PorterDuff.Mode.MULTIPLY)
    }

    fun setDrawableColor(drawable: Drawable, color: Long) {
        val cStr: String = "#" + String.format("%02X", color)
        val c: Int = Color.parseColor(cStr)

        drawable.setColorFilter(c, PorterDuff.Mode.MULTIPLY)
    }
}