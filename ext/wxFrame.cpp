/*
 * wxFrame.cpp
 *
 *  Created on: 06.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"

VALUE rb_cWXFrame;

#define _self wrap<wxFrame*>(self)

RubyFrame::RubyFrame(VALUE klass)
: RubyToplevel::RubyToplevel(klass)
{}

wxStatusBar* RubyFrame::OnCreateStatusBar(int number,
	long style, wxWindowID winid, const wxString& name)
{
	rb_funcall(mRuby, rb_intern("on_create_StatusBar"), 4,
		INT2NUM(number),INT2NUM(style),wrap(winid),wrap(name));
	return wxFrame::OnCreateStatusBar(number,style,winid,name);
}
wxToolBar* RubyFrame::OnCreateToolBar(long style,
	wxWindowID winid, const wxString& name)
{
	rb_funcall(mRuby, rb_intern("on_create_ToolBar"), 3,
		INT2NUM(style),wrap(winid),wrap(name));
	return wxFrame::OnCreateToolBar(style,winid,name);
}

void RubyFrame::DoMenuUpdates(wxMenu* menu)
{
	wxFrame::DoMenuUpdates(menu);
	rb_funcall(mRuby, rb_intern("do_MenuUpdates"), 1, wrap(menu));
}

void RubyFrame::DoGiveHelp(const wxString& text, bool show)
{
	wxFrame::DoGiveHelp(text,show);
	rb_funcall(mRuby, rb_intern("do_giveHelp"), 1, wrap(text),wrap(show));
}
namespace RubyWX {
namespace Frame {

VALUE _alloc(VALUE self)
{
	return wrap(new RubyFrame(self));
}

#if wxUSE_MENUS
macro_attr(MenuBar,wxMenuBar*)
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
macro_attr(StatusBar,wxStatusBar*)
macro_attr(StatusBarPane,int)
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
macro_attr(ToolBar,wxToolBar*)
#endif // wxUSE_TOOLBAR
}
}

void Init_WXFrame(VALUE rb_mWX)
{
	using namespace RubyWX::Frame;
	rb_cWXFrame = rb_define_class_under(rb_mWX,"Frame",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXFrame,_alloc);


//rb_define_attr_method(rb_cWXWindow, "title",_getTitle,_setTitle);
}
