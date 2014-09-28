/*
 * wxInfobarBase.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */


#include "wxInfoBarBase.hpp"

VALUE rb_cWXInfoBarBase;

#if wxUSE_INFOBAR

template <>
wxInfoBarBase* unwrap<wxInfoBarBase*>(const VALUE &arg)
{
	return unwrapTypedPtr<wxInfoBarBase>(arg,rb_cWXInfoBarBase);
}


#define _self unwrap<wxInfoBarBase*>(self)

namespace RubyWX {
namespace InfoBarBase {

singlefunc(Dismiss)

DLL_LOCAL VALUE _showMessage(int argc,VALUE *argv,VALUE self)
{
	VALUE text,icon;
	rb_scan_args(argc, argv, "11",&text,&icon);

	//use argc for check if parameter is given because nil is treated different
	_self->ShowMessage(unwrap<wxString>(text), argc == 1 ? wxICON_INFORMATION : unwrap_iconflag(icon));

	return self;
}

DLL_LOCAL VALUE _addButton(int argc,VALUE *argv,VALUE self)
{
	VALUE id,label;
	rb_scan_args(argc, argv, "11",&id,&label);
	
	wxWindowID wid = unwrapID(id);
	
	_self->AddButton(wid,unwrap<wxString>(label));
	
	if(rb_block_given_p())
	{
		VALUE proc = rb_block_proc();

		_self->Bind(wxEVT_BUTTON,RubyFunctor(proc),wid);
	
	}
	return self;
}

DLL_LOCAL VALUE _removeButton(VALUE self,VALUE id)
{
	wxWindowID wid = unwrapID(id);

#ifdef HAVE_WXINFOBAR_GETBUTTONCOUNT
	if(!_self->HasButtonId(wid))
		return Qnil;
#endif

	_self->RemoveButton(wid);
	return self;
}

#ifdef HAVE_WXINFOBAR_GETBUTTONCOUNT

DLL_LOCAL VALUE _hasButton(VALUE self,VALUE id)
{
	return wrap(_self->HasButtonId(unwrapID(id)));
}

DLL_LOCAL VALUE _each_button_size(VALUE self)
{
	return UINT2NUM(_self->GetButtonCount());
}


DLL_LOCAL VALUE _each_button(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_button_size));
	
	std::size_t count = _self->GetButtonCount();
	
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrapID(_self->GetButtonId(i)));
	
	return self;
}

DLL_LOCAL VALUE _getButton(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetButtonCount()))
		return wrapID(_self->GetButtonId(cidx));
	return Qnil;
}

#endif

}
}

#endif
DLL_LOCAL void Init_WXInfoBarBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_INFOBAR
	using namespace RubyWX::InfoBarBase;
	rb_cWXInfoBarBase = rb_define_class_under(rb_mWX,"InfoBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXInfoBarBase);

	rb_define_method(rb_cWXInfoBarBase,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);
	rb_define_method(rb_cWXInfoBarBase,"show_message",RUBY_METHOD_FUNC(_showMessage),-1);

	rb_define_method(rb_cWXInfoBarBase,"add_button",RUBY_METHOD_FUNC(_addButton),-1);
	rb_define_method(rb_cWXInfoBarBase,"remove_button",RUBY_METHOD_FUNC(_removeButton),1);

#ifdef HAVE_WXINFOBAR_GETBUTTONCOUNT

	rb_define_method(rb_cWXInfoBarBase,"has_button?",RUBY_METHOD_FUNC(_hasButton),1);
	rb_define_method(rb_cWXInfoBarBase,"get_button",RUBY_METHOD_FUNC(_getButton),1);

	rb_define_method(rb_cWXInfoBarBase,"each_button",RUBY_METHOD_FUNC(_each_button),0);

#endif

	registerType<wxInfoBarBase>(rb_cWXInfoBarBase);
#endif

}


