/*
 * wxDatePickerCtrl.hpp
 *
 *  Created on: 15.12.2013
 *      Author: hanmac
 */

#ifndef WXDATEPICKER_HPP_
#define WXDATEPICKER_HPP_


#include "wxControl.hpp"

extern VALUE rb_cWXDatePicker;

void Init_WXDatePicker(VALUE rb_mWX);

#if wxUSE_DATEPICKCTRL
#include <wx/datectrl.h>
#endif

#endif /* WXDATEPICKER_HPP_ */
