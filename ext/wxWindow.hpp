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
	return wrap(window,rb_cWXWindow);
}


template <>
inline wxWindow* wrap< wxWindow* >(const VALUE &vwindow)
{
	return unwrapPtr<wxWindow>(vwindow, rb_cWXWindow);
}

#endif /* WXWINDOW_HPP_ */
