/*
 * wxColorPicker.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXCOLORPICKER_HPP_
#define WXCOLORPICKER_HPP_

#include "main.hpp"

extern VALUE rb_cWXColorPicker;

void Init_WXColorPicker(VALUE rb_mWX);
#if wxUSE_COLOURPICKERCTRL
#include <wx/clrpicker.h>
template <>
inline VALUE wrap< wxColourPickerCtrl >(wxColourPickerCtrl* window)
{
	return wrap(window,rb_cWXColorPicker);
}


template <>
inline wxColourPickerCtrl* wrap< wxColourPickerCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxColourPickerCtrl>(vwindow, rb_cWXColorPicker);
}

#endif

#endif /* WXCOLORPICKER_HPP_ */
