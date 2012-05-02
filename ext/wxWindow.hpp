/*
 * wxWindow.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef WXWINDOW_HPP_
#define WXWINDOW_HPP_


#include "main.hpp"

#include "wxMenuBar.hpp"
#include "wxPanel.hpp"
#include "wxControl.hpp"
#include "wxToplevel.hpp"

extern VALUE rb_cWXWindow;
void Init_WXWindow(VALUE rb_mWX);

VALUE wrapID(wxWindowID val);
wxWindowID unwrapID(VALUE val);
void registerID(const char *name,wxWindowID id);

template <>
inline VALUE wrap< wxWindow >(wxWindow* window)
{
	if(window==NULL)
		return Qnil;

	if(wxMenuBar *menubar = dynamic_cast<wxMenuBar*>(window))
		return wrap(menubar);

	if(wxTopLevelWindow *toplevel = dynamic_cast<wxTopLevelWindow*>(window))
		return wrap(toplevel);

	if(wxPanel *panel = dynamic_cast<wxPanel*>(window))
		return wrap(panel);

#if wxUSE_CONTROLS
	if(wxControl *control = dynamic_cast<wxControl*>(window))
		return wrap(control);
#endif
	return wrap(window,rb_cWXWindow);
}


template <>
inline wxWindow* wrap< wxWindow* >(const VALUE &vwindow)
{
	return unwrapPtr<wxWindow>(vwindow, rb_cWXWindow);
}


#if wxUSE_TOOLTIPS
#include <wx/tooltip.h>

template <>
inline VALUE wrap< wxToolTip >(wxToolTip* window)
{
	return wrap(window->GetTip());
}

template <>
inline wxToolTip* wrap< wxToolTip* >(const VALUE &vwindow)
{
	if(NIL_P(vwindow))
		return NULL;
	return new wxToolTip(wrap<wxString>(vwindow));
}
#endif
#endif /* WXWINDOW_HPP_ */
