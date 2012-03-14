/*
 * wxTimePickerCtrl.hpp
 *
 *  Created on: 02.03.2012
 *      Author: hanmac
 */

#ifndef WXTIMEPICKER_HPP_
#define WXTIMEPICKER_HPP_


#include "main.hpp"

extern VALUE rb_cWXTimePicker;

void Init_WXTimePicker(VALUE rb_mWX);

#if wxUSE_TIMEPICKCTRL
#include <wx/timectrl.h>

template <>
inline VALUE wrap< wxTimePickerCtrl >(wxTimePickerCtrl* window)
{
	return getEvtObj(window,rb_cWXTimePicker);
}


template <>
inline wxTimePickerCtrl* wrap< wxTimePickerCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxTimePickerCtrl>(vwindow, rb_cWXTimePicker);
}

#endif

#endif /* WXTIMEPICKER_HPP_ */
