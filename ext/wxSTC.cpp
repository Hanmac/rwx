/*
 * wxSTC.cpp
 *
 *  Created on: 01.04.2012
 *      Author: hanmac
 */

#include "wxSTC.hpp"
#include "wxColor.hpp"

VALUE rb_cWXSTC;

#if wxUSE_STC
#define _self unwrap<wxStyledTextCtrl*>(self)

namespace RubyWX {
namespace STC {

macro_attr(Text,wxString)
macro_attr(UseTabs,bool)

macro_attr(Indent,int)

macro_attr(CaretLineVisible,bool)
macro_attr(CaretForeground,wxColor)
macro_attr(CaretLineBackground,wxColor)

APP_PROTECT(wxStyledTextCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	_self->StyleClearAll();
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _test_style(VALUE self)
{
	_self->SetLexer(wxSTC_LEX_RUBY);
	_self->StyleSetBold(wxSTC_RB_STDERR,true);
	_self->StyleSetForeground(wxSTC_RB_POD,*wxBLUE);
	_self->StyleSetForeground(wxSTC_RB_MODULE_NAME,*wxGREEN);
	_self->StyleSetForeground(wxSTC_RB_STDERR,*wxRED);
	_self->StyleSetForeground(wxSTC_RB_SYMBOL,*wxYELLOW);

	_self->StyleSetForeground(wxSTC_RB_COMMENTLINE,"grey");


	return self;
}

DLL_LOCAL VALUE _UndoAction(VALUE self)
{
	_self->BeginUndoAction();
	rb_yield(Qnil);
	_self->EndUndoAction();
	return self;
}



}
}
#endif
DLL_LOCAL void Init_WXSTC(VALUE rb_mWX)
{
#if wxUSE_STC
	using namespace RubyWX::STC;
	rb_cWXSTC = rb_define_class_under(rb_mWX,"STC",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSTC,_alloc);

	rb_define_method(rb_cWXSTC,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSTC,"text",_getText,_setText);
	rb_define_attr_method(rb_cWXSTC,"use_tabs",_getUseTabs,_setUseTabs);


	rb_define_method(rb_cWXSTC,"test_style",RUBY_METHOD_FUNC(_test_style),0);


	rb_define_const(rb_cWXSTC,"Version",wrap(wxStyledTextCtrl::GetLibraryVersionInfo().GetVersionString()));

	registerInfo<wxStyledTextCtrl>(rb_cWXSTC);
#endif

}



