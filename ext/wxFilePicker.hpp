/*
 * wxFilePicker.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXFILEPICKER_HPP_
#define WXFILEPICKER_HPP_

#include "main.hpp"

extern VALUE rb_cWXFilePicker;

void Init_WXFilePicker(VALUE rb_mWX);
#if wxUSE_FILEPICKERCTRL
#include <wx/filepicker.h>
template <>
inline VALUE wrap< wxFilePickerCtrl >(wxFilePickerCtrl* window)
{
	return wrap(window,rb_cWXFilePicker);
}


template <>
inline wxFilePickerCtrl* wrap< wxFilePickerCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFilePickerCtrl>(vwindow, rb_cWXFilePicker);
}

#endif

#endif /* WXFILEPICKER_HPP_ */
