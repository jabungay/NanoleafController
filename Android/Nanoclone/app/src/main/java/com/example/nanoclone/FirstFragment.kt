package com.example.nanoclone

import android.app.Activity
import android.content.res.Resources
import android.graphics.Color
import android.graphics.ColorFilter
import android.graphics.PorterDuff
import android.graphics.drawable.Drawable
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.navigation.fragment.findNavController
import kotlinx.android.synthetic.main.fragment_first.*

/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class FirstFragment : Fragment() {

    override fun onCreateView(
            inflater: LayoutInflater, container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_first, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val res: Resources = resources
        val id: Int = res.getIdentifier("button_test", "id", context!!.packageName)

        view.findViewById<Button>(id).setOnClickListener {
            button_test.isSelected = !button_test.isSelected

            val drawable: Drawable = button_test.background
            Functions().setDrawableColor(drawable, 0xff, 0xaa, 0x10, 0xff)
        }
    }
}
