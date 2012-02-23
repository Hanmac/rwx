/*
 * wxFrame.hpp
 *
 *  Created on: 06.02.2012
 *      Author: hanmac
 */

#ifndef WXFRAME_HPP_
#define WXFRAME_HPP_

#include "wxToplevel.hpp"

extern VALUE rb_cWXFrame;

void Init_WXFrame(VALUE rb_mWX);

class RubyFrame : public wxFrame, public RubyToplevel {
public:
	RubyFrame(VALUE klass);
#if wxUSE_STATUSBAR
	wxStatusBar *OnCreateStatusBar(int number,
		long style, wxWindowID winid, const wxString& name);
#endif // wxUSE_STATUSBAR
#if wxUSE_TOOLBAR
	wxToolBar *OnCreateToolBar(long style,
    	wxWindowID winid, const wxString& name);
#endif // wxUSE_TOOLBAR

#if wxUSE_MENUS
#if wxUSE_STATUSBAR
    void OnMenuOpen(wxMenuEvent& event);
    void OnMenuClose(wxMenuEvent& event);
    void OnMenuHighlight(wxMenuEvent& event);
#endif // wxUSE_STATUSBAR

    // send wxUpdateUIEvents for all menu items in the menubar,
    // or just for menu if non-NULL
    virtual void DoMenuUpdates(wxMenu* menu = NULL);
#endif // wxUSE_MENUS

#if wxUSE_MENUS || wxUSE_TOOLBAR
    // show help text for the currently selected menu or toolbar item
    // (typically in the status bar) or hide it and restore the status bar text
    // originally shown before the menu was opened if show == false
    virtual void DoGiveHelp(const wxString& text, bool show);
#endif

};


template <>
inline VALUE wrap< RubyFrame >(RubyFrame *frame )
{
	return frame->mRuby;
}

template <>
inline VALUE wrap< wxFrame >(wxFrame* window)
{
	if(window==NULL)
		return Qnil;

	RubyFrame *frame = reinterpret_cast<RubyFrame*>(window);
	if(frame)
		return wrap(frame);

	return Qnil;
}


template <>
inline wxFrame* wrap< wxFrame* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFrame>(vwindow, rb_cWXFrame);
}

#endif /* WXFRAME_HPP_ */
