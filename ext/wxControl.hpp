/*
 * wxControl.hpp
 *
 *  Created on: 08.02.2012
 *      Author: hanmac
 */

#ifndef WXCONTROL_HPP_
#define WXCONTROL_HPP_

#include "wxWindow.hpp"

#include "wxAnyButton.hpp"

#include "wxToolBar.hpp"

#if wxUSE_STATUSBAR
#include "wxStatusBar.hpp"
#endif

#if wxUSE_GAUGE
#include "wxGauge.hpp"
#endif

extern VALUE rb_cWXControl;
void Init_WXControl(VALUE rb_mWX);
#if wxUSE_CONTROLS
template <>
inline VALUE wrap< wxControl >(wxControl* window)
{
	if(window==NULL)
		return Qnil;

	wxAnyButton *anybutton = dynamic_cast<wxAnyButton*>(window);
	if(anybutton)
		return wrap(anybutton);

	wxToolBar *toolbar = dynamic_cast<wxToolBar*>(window);
	if(toolbar)
		return wrap(toolbar);

#if wxUSE_STATUSBAR
	wxStatusBar *statusbar = dynamic_cast<wxStatusBar*>(window);
	if(statusbar)
		return wrap(statusbar);
#endif
#if wxUSE_GAUGE
	wxGauge *gauge = dynamic_cast<wxGauge*>(window);
	if(gauge)
		return wrap(gauge);
#endif


	return wrap(window,rb_cWXControl);
}


template <>
inline wxControl* wrap< wxControl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxControl>(vwindow, rb_cWXControl);
}
#endif

#endif /* WXCONTROL_HPP_ */
