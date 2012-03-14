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

	wxMenuBar *menubar = dynamic_cast<wxMenuBar*>(window);
	if(menubar)
		return wrap(menubar);

	wxTopLevelWindow *toplevel = dynamic_cast<wxTopLevelWindow*>(window);
	if(toplevel)
		return wrap(toplevel);

#if wxUSE_CONTROLS
	wxControl *control = dynamic_cast<wxControl*>(window);
	if(control)
		return wrap(control);
#endif
//	VALUE result = wrap(window,rb_cWXWindow);
//	windowholder.insert(std::make_pair(window,result));
//	return result;
	return getEvtObj(window,rb_cWXWindow);
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
	return new wxToolTip(wrap<wxString>(vwindow));
}
#endif
#endif /* WXWINDOW_HPP_ */
