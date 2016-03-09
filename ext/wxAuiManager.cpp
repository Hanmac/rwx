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
#define _self unwrap<wxAuiManager*>(self)

namespace RubyWX {
namespace AuiManager {

DLL_LOCAL VALUE _get(VALUE self,VALUE key)
{
	if(rb_obj_is_kind_of(key,rb_cWXWindow))
		return wrap(_self->GetPane(unwrap<wxWindow*>(key)));
	else
		return wrap(_self->GetPane(unwrap<wxString>(key)));
}

DLL_LOCAL VALUE _set(VALUE self,VALUE key,VALUE val)
{
	wxAuiPaneInfo inf(_self->GetPane(unwrap<wxString>(key)));
	if(inf.IsOk())
		inf.Window(unwrap<wxWindow*>(val));
	else{
		_self->AddPane(unwrap<wxWindow*>(val),wxAuiPaneInfo().Name(unwrap<wxString>(key)));
		_self->Update();
	}
	return val;
}

DLL_LOCAL VALUE _addPane(int argc,VALUE *argv,VALUE self)
{
	VALUE window,hash;
	wxWindow* w = NULL;
	rb_scan_args(argc, argv, "11",&window,&hash);
	if(rb_obj_is_kind_of(window,rb_cClass) && RTEST(rb_class_inherited_p(window,rb_cWXWindow)))
	{
		VALUE argv2[] = {wrap(_self->GetManagedWindow()) };
		w = unwrap<wxWindow*>(rb_class_new_instance(1,argv2,window));
	}else
		w = unwrap<wxWindow*>(window);
	_self->AddPane(w,unwrap<wxAuiPaneInfo>(hash));

	_self->Update();
	return wrap(w);
}

}
}
#endif
DLL_LOCAL void Init_WXAuiManager(VALUE rb_mWX)
{
#if 0
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");
#endif

#if wxUSE_AUI
	using namespace RubyWX::AuiManager;
	rb_cWXAuiManager = rb_define_class_under(rb_mWX,"AuiManager",rb_cObject);
	rb_undef_alloc_func(rb_cWXAuiManager);
	rb_include_module(rb_cWXAuiManager,rb_mWXEvtHandler);

	rb_define_method(rb_cWXAuiManager,"add",RUBY_METHOD_FUNC(_addPane),-1);

	rb_define_method(rb_cWXAuiManager,"[]",RUBY_METHOD_FUNC(_get),1);
	rb_define_method(rb_cWXAuiManager,"[]=",RUBY_METHOD_FUNC(_set),2);

	registerInfo<wxAuiManager>(rb_cWXAuiManager);
#endif

}
