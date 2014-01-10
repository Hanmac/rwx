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

singlereturn(GetList)
singlereturn(GetOrder)

/*
 * call-seq:
 *   RearrangeDialog.new(parent, name, [options])
 *   RearrangeDialog.new(parent, [options])
 *
 * creates a new RearrangeDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * items [string]
 *   * order [Integer]
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxArrayString items;
		wxArrayInt order;

		wxString message;
		wxString title;

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"items",items);

			set_hash_option(hash,"order",order);

			set_hash_option(hash,"message",message);
			set_hash_option(hash,"title",title);

			order.resize(items.size());
		}

		_self->Create(unwrap<wxWindow*>(parent),message,title,order,items);
		
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


