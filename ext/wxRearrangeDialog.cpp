/*
 * wxRearrangeDialog.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRearrangeDialog.hpp"

VALUE rb_cWXRearrangeDialog;

#if wxUSE_REARRANGECTRL

namespace RubyWX {
namespace RearrangeDialog {
#define _self unwrap<wxRearrangeDialog*>(self)

APP_PROTECT(wxRearrangeDialog)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	wxArrayString items;
	wxArrayInt order;

	wxString message;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("items")))))
			items = unwrap<wxArrayString>(temp);
			order.resize(items.size());
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("order")))))
			order = unwrap<wxArrayInt>(temp);
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("message")))))
			message = unwrap<wxString>(temp);

	}
	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),message,wxEmptyString,order,items);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _AddExtraControls(int argc,VALUE *argv,VALUE self)
{
	VALUE wnd;
	wxWindow *cwnd;
	rb_scan_args(argc, argv, "01",&wnd);
	if(rb_obj_is_kind_of(wnd,rb_cClass) && rb_class_inherited(wnd,rb_cWXWindow)) {
		VALUE args[] = {self};
		cwnd = unwrap<wxWindow*>(rb_class_new_instance(1,args,wnd));
	}else
		cwnd = unwrap<wxWindow*>(wnd);

	_self->AddExtraControls(cwnd);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRearrangeDialog(VALUE rb_mWX)
{
#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeDialog;
	rb_cWXRearrangeDialog = rb_define_class_under(rb_mWX,"RearrangeDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXRearrangeDialog,_alloc);

	rb_define_method(rb_cWXRearrangeDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXRearrangeDialog,"add_extra_controls",RUBY_METHOD_FUNC(_AddExtraControls),-1);

	registerInfo<wxRearrangeDialog>(rb_cWXRearrangeDialog);
#endif

}


