/*
 * wxCalendarCtrlBase.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXCALENDARCTRLBASE_HPP_
#define WXCALENDARCTRLBASE_HPP_


#include "wxControl.hpp"

extern VALUE rb_cWXCalendarCtrlBase;

void Init_WXCalendarCtrlBase(VALUE rb_mWX);

#if wxUSE_CALENDARCTRL
#include <wx/calctrl.h>

template <>
wxCalendarCtrlBase* unwrap<wxCalendarCtrlBase*>(const VALUE &arg);

namespace RubyWX {
namespace CalendarCtrlBase {

void _set_options(VALUE hash,
		wxWindowID &id,
		wxDateTime &date,
		int &style
);

}
}

#endif

#endif /* WXCALENDARCTRLBASE_HPP_ */
