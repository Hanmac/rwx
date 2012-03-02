/*
 * wxWindow.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */
#include "wxWindow.hpp"
#include "wxEvtHandler.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"


#define _self wrap<wxWindow*>(self)

VALUE rb_cWXWindow;

namespace RubyWX {
namespace Window {

macro_attr(Label,wxString)
macro_attr(Name,wxString)
macro_attr(WindowVariant,wxWindowVariant)
macro_attr(LayoutDirection,wxLayoutDirection)

macro_attr(Id,wxWindowID)

macro_attr(Position,wxPoint)

macro_attr(Size,wxSize)
macro_attr(ClientSize,wxSize)

macro_attr(MinSize,wxSize)
macro_attr(MinClientSize,wxSize)

macro_attr(MaxSize,wxSize)
macro_attr(MaxClientSize,wxSize)

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
macro_attr(ThemeEnabled,bool)

macro_attr(Sizer,wxSizer*)
macro_attr(ContainingSizer,wxSizer*)

singlefunc(Show)
singlefunc(Hide)

singlefunc(CaptureMouse)
singlefunc(ReleaseMouse)

singlereturn(GetParent)
singlereturn(GetGrandParent)

singlereturn(GetBackgroundColour)
singlereturn(GetForegroundColour)

singlereturn(LineUp)
singlereturn(LineDown)
singlereturn(PageUp)
singlereturn(PageDown)


VALUE _SetParent(VALUE self,VALUE parent)
{
	_self->Reparent(wrap<wxWindow*>(parent));
	return parent;
}


VALUE _SetBackgroundColour(VALUE self,VALUE val)
{
	_self->SetOwnBackgroundColour(wrap<wxColor>(val));
	return val;
}


VALUE _SetForegroundColour(VALUE self,VALUE val)
{
	_self->SetOwnForegroundColour(wrap<wxColor>(val));
	return val;
}

VALUE _alloc(VALUE self)
{
	return wrap(new wxWindow,self);
}


VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	return self;
}


VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	wxWindowList list = _self->GetChildren();
	for(wxWindowList::iterator it = list.begin();it != list.end();++it)
	{
		rb_yield(wrap(*it));
	}
	return self;
}
}
}

void Init_WXWindow(VALUE rb_mWX)
{
	using namespace RubyWX::Window;
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_define_alloc_func(rb_cWXWindow,_alloc);

	rb_define_method(rb_cWXWindow,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXWindow,rb_mWXEvtHandler);
	rb_include_module(rb_cWXWindow,rb_mEnumerable);

	rb_define_attr_method(rb_cWXWindow, "label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXWindow, "name",_getName,_setName);
	rb_define_attr_method(rb_cWXWindow, "parent",_GetParent,_SetParent);

	rb_define_attr_method(rb_cWXWindow, "id",_getId,_setId);

	rb_define_attr_method(rb_cWXWindow, "backgroundColour",_GetBackgroundColour,_SetBackgroundColour);
	rb_define_attr_method(rb_cWXWindow, "foregroundColour",_GetForegroundColour,_SetForegroundColour);

	rb_define_attr_method(rb_cWXWindow, "font",_getFont,_setFont);
#if wxUSE_HELP
	rb_define_attr_method(rb_cWXWindow, "helpText",_getHelpText,_setHelpText);
#endif // wxUSE_HELP

	rb_define_method(rb_cWXWindow,"show",RUBY_METHOD_FUNC(_Show),0);
	rb_define_method(rb_cWXWindow,"hide",RUBY_METHOD_FUNC(_Hide),0);

	rb_define_method(rb_cWXWindow,"each",RUBY_METHOD_FUNC(_each),0);
}
