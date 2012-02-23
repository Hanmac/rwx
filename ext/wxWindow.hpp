/*
 * wxWindow.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef WXWINDOW_HPP_
#define WXWINDOW_HPP_


#include "main.hpp"

class RubyWindow : public wxWindow {
public:
	RubyWindow(VALUE klass);

#if wxUSE_HELP
	void OnHelp(wxHelpEvent& event);
#endif // wxUSE_HELP

    // do the UI update processing for this window
    virtual void UpdateWindowUI(long flags = wxUPDATE_UI_NONE);

    // Implement internal behaviour (menu updating on some platforms)
    virtual void OnInternalIdle();


	VALUE mRuby;

};


#include "wxControl.hpp"
#include "wxToplevel.hpp"

extern VALUE rb_cWXWindow;
void Init_WXWindow(VALUE rb_mWX);

template <>
inline VALUE wrap< RubyWindow >(RubyWindow *frame )
{
	return frame->mRuby;
}


template <>
inline VALUE wrap< wxWindow >(wxWindow* window)
{
	if(window==NULL)
		return Qnil;
	wxTopLevelWindow *toplevel = reinterpret_cast<wxTopLevelWindow*>(window);
	if(toplevel)
		return wrap(toplevel);

	wxControl *control = reinterpret_cast<wxControl*>(window);
		if(control)
			return wrap(control);

	return Qnil;
}


template <>
inline wxWindow* wrap< wxWindow* >(const VALUE &vwindow)
{
	return unwrapPtr<wxWindow>(vwindow, rb_cWXWindow);
}

#endif /* WXWINDOW_HPP_ */
