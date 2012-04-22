/*
 * wxDirPicker.hpp
 *
 *  Created on: 13.03.2012
 *      Author: hanmac
 */

#ifndef WXDIRPICKER_HPP_
#define WXDIRPICKER_HPP_

#include "main.hpp"

extern VALUE rb_cWXDirPicker;

void Init_WXDirPicker(VALUE rb_mWX);
#if wxUSE_DIRPICKERCTRL
#include <wx/filepicker.h>
template <>
inline VALUE wrap< wxDirPickerCtrl >(wxDirPickerCtrl* window)
{
	return wrap(window,rb_cWXDirPicker);
}


template <>
inline wxDirPickerCtrl* wrap< wxDirPickerCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxDirPickerCtrl>(vwindow, rb_cWXDirPicker);
}

#endif

#endif /* WXDIRPICKER_HPP_ */
