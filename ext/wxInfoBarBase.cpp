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
	return unwrapPtr<wxInfoBarBase>(arg,rb_cWXInfoBarBase);
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
	_self->ShowMessage(unwrap<wxString>(text), argc == 1 ? wxICON_INFORMATION : unwrap_infoflag(icon));

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
#ifdef wxHAS_EVENT_BIND
			_self->Bind(wxEVT_BUTTON,RubyFunctor(proc),wid);
#else
			_self->Connect(wid,wxEVT_BUTTON,wxCommandEventHandler(RubyFunctor::operator()),NULL,new RubyFunctor(proc));
#endif
	
	}
	return self;
}

DLL_LOCAL VALUE _removeButton(VALUE self,VALUE id)
{
	_self->RemoveButton(unwrapID(id));
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXInfoBarBase(VALUE rb_mWX)
{
#if wxUSE_INFOBAR
	using namespace RubyWX::InfoBarBase;
	rb_cWXInfoBarBase = rb_define_class_under(rb_mWX,"InfoBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXInfoBarBase);

	rb_define_method(rb_cWXInfoBarBase,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);
	rb_define_method(rb_cWXInfoBarBase,"show_message",RUBY_METHOD_FUNC(_showMessage),-1);

	rb_define_method(rb_cWXInfoBarBase,"add_button",RUBY_METHOD_FUNC(_addButton),-1);
	rb_define_method(rb_cWXInfoBarBase,"remove_button",RUBY_METHOD_FUNC(_removeButton),1);

	registerType<wxInfoBarBase>(rb_cWXInfoBarBase);
#endif

}


