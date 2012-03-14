/*
 * wxFontPicker.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXFONTPICKER_HPP_
#define WXFONTPICKER_HPP_

#include "main.hpp"

extern VALUE rb_cWXFontPicker;

void Init_WXFontPicker(VALUE rb_mWX);

#if wxUSE_FONTPICKERCTRL
#include <wx/fontpicker.h>

template <>
inline VALUE wrap< wxFontPickerCtrl >(wxFontPickerCtrl* window)
{
	return getEvtObj(window,rb_cWXFontPicker);
}


template <>
inline wxFontPickerCtrl* wrap< wxFontPickerCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFontPickerCtrl>(vwindow, rb_cWXFontPicker);
}

#endif

#endif /* WXFONTPICKER_HPP_ */
