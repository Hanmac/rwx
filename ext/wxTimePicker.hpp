/*
 * wxTimePickerCtrl.hpp
 *
 *  Created on: 02.03.2012
 *      Author: hanmac
 */

#ifndef WXTIMEPICKER_HPP_
#define WXTIMEPICKER_HPP_


#include "wxControl.hpp"

extern VALUE rb_cWXTimePicker;

void Init_WXTimePicker(VALUE rb_mWX);

#if wxUSE_TIMEPICKCTRL
#include <wx/timectrl.h>
#endif

#endif /* WXTIMEPICKER_HPP_ */
