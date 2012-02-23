/*
 * wxWindow.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */
#include "wxWindow.hpp"

#define _self wrap<wxWindow*>(self)

VALUE rb_cWXWindow;

RubyWindow::RubyWindow(VALUE klass)
{
	mRuby = Data_Wrap_Struct(klass, 0, 0, this);
}

void RubyWindow::UpdateWindowUI(long flags)
{
	wxWindow::UpdateWindowUI(flags);
	rb_funcall(mRuby, rb_intern("updateWindowUI"), 1, INT2NUM(flags));
}

void RubyWindow::OnInternalIdle()
{
	wxWindow::OnInternalIdle();
	rb_funcall(mRuby, rb_intern("on_InternalIdle"), 0);
}

#if wxUSE_HELP
void RubyWindow::OnHelp(wxHelpEvent& event)
{
	wxWindow::OnHelp(event);
	rb_funcall(mRuby, rb_intern("on_help"), 1,wrap(event));
}
#endif // wxUSE_HELP

namespace RubyWX {
namespace Window {

macro_attr(Label,wxString)
macro_attr(Name,wxString)
macro_attr(WindowVariant,wxWindowVariant)
macro_attr(LayoutDirection,wxLayoutDirection)

macro_attr(Position,wxPoint)

macro_attr(Size,wxSize)
macro_attr(ClientSize,wxSize)

macro_attr(MinSize,wxSize)
macro_attr(MinClientSize,wxSize)

macro_attr(MaxSize,wxSize)
macro_attr(MaxClientSize,wxSize)

macro_attr(BackgroundColour,wxColour)
macro_attr(ForegroundColour,wxColour)

macro_attr(BackgroundStyle,wxBackgroundStyle)

macro_attr(Font,wxFont)
macro_attr(Cursor,wxCursor)

#if wxUSE_VALIDATORS
macro_attr(Validator,wxValidator)
#endif // wxUSE_VALIDATORS
#if wxUSE_CARET
macro_attr(Caret,wxCaret*)
#endif // wxUSE_CARET
#if wxUSE_HELP
macro_attr(HelpText,wxString)
#endif // wxUSE_HELP
#if wxUSE_TOOLTIPS
macro_attr(ToolTip,wxToolTip*)
#endif // wxUSE_TOOLTIPS
#if wxUSE_DRAG_AND_DROP
macro_attr(DropTarget,wxDropTarget*)
#endif // wxUSE_DRAG_AND_DROP
macro_attr(AutoLayout,bool)

macro_attr(Sizer,wxSizer*)
macro_attr(ContainingSizer,wxSizer*)

singlefunc(Show)
singlefunc(Hide)

singlefunc(CaptureMouse)
singlefunc(ReleaseMouse)

singlereturn(GetParent)
singlereturn(GetGrandParent)


singlereturn(LineUp)
singlereturn(LineDown)
singlereturn(PageUp)
singlereturn(PageDown)


VALUE _SetParent(VALUE self,VALUE parent)
{
	_self->Reparent(wrap<wxWindow*>(parent));
	return parent;
}


VALUE _alloc(VALUE self)
{
	return wrap(new RubyWindow(self));
}


}
}

void Init_WXWindow(VALUE rb_mWX)
{
	using namespace RubyWX::Window;
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_define_alloc_func(rb_cWXWindow,_alloc);

	rb_define_attr_method(rb_cWXWindow, "label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXWindow, "name",_getName,_setName);
	rb_define_attr_method(rb_cWXWindow, "parent",_GetParent,_SetParent);
}
