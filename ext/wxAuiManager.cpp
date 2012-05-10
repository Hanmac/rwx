/*
 * wxAuiManager.cpp
 *
 *  Created on: 26.03.2012
 *      Author: hanmac
 */

#include "wxAuiPane.hpp"
#include "wxEvtHandler.hpp"

VALUE rb_cWXAuiManager;

#if wxUSE_AUI
#define _self wrap<wxAuiManager*>(self)

template <>
VALUE wrap< wxAuiManager >(wxAuiManager* window)
{
	return wrap(window,rb_cWXAuiManager);
}

template <>
wxAuiManager* wrap< wxAuiManager* >(const VALUE &vwindow)
{
	return unwrapPtr<wxAuiManager>(vwindow, rb_cWXAuiManager);
}


namespace RubyWX {
namespace AuiManager {

VALUE _get(VALUE self,VALUE key)
{
	if(rb_obj_is_kind_of(key,rb_cWXWindow))
		return wrap(_self->GetPane(wrap<wxWindow*>(key)));
	else
		return wrap(_self->GetPane(wrap<wxString>(key)));
}

VALUE _set(VALUE self,VALUE key,VALUE val)
{
	wxAuiPaneInfo inf(_self->GetPane(wrap<wxString>(key)));
	if(inf.IsOk())
		inf.Window(wrap<wxWindow*>(val));
	else{
		_self->AddPane(wrap<wxWindow*>(val),wxAuiPaneInfo().Name(wrap<wxString>(key)));
		_self->Update();
	}
	return val;
}

VALUE _addPane(int argc,VALUE *argv,VALUE self)
{
	VALUE window,hash;
	rb_scan_args(argc, argv, "11",&window,&hash);
	VALUE result = wrap(_self->AddPane(wrap<wxWindow*>(window),wrap<wxAuiPaneInfo>(hash)));
	_self->Update();
	return result;
}

}
}
#endif
void Init_WXAuiManager(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiManager;
	rb_cWXAuiManager = rb_define_class_under(rb_mWX,"AuiManager",rb_cObject);
	rb_undef_alloc_func(rb_cWXAuiManager);
	rb_include_module(rb_cWXAuiManager,rb_mWXEvtHandler);

	rb_define_method(rb_cWXAuiManager,"add",RUBY_METHOD_FUNC(_addPane),-1);

	rb_define_method(rb_cWXAuiManager,"[]",RUBY_METHOD_FUNC(_get),1);
	rb_define_method(rb_cWXAuiManager,"[]=",RUBY_METHOD_FUNC(_set),2);

#endif

}
