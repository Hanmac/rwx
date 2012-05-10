/*
 * wxCalendarCtrlGeneric.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXCALENDARCTRLGENERIC_HPP_
#define WXCALENDARCTRLGENERIC_HPP_

#include "wxCalendarCtrlBase.hpp"

extern VALUE rb_cWXCalendarCtrlGeneric;

void Init_WXCalendarCtrlGeneric(VALUE rb_mWX);

#if wxUSE_CALENDARCTRL
#include <wx/generic/calctrlg.h>
#endif

#endif /* WXCALENDARCTRLGENERIC_HPP_ */
