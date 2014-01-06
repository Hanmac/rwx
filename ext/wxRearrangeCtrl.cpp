/*
 * wxRearrangeCtrl.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRearrangeCtrl.hpp"

VALUE rb_cWXRearrangeCtrl;

#if wxUSE_REARRANGECTRL

namespace RubyWX {
namespace RearrangeCtrl {
#define _self unwrap<wxRearrangeCtrl*>(self)

APP_PROTECT(wxRearrangeCtrl)

/*
 * call-seq:
 *   RearrangeCtrl.new(parent, [options])
 *
 * creates a new RearrangeCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!_created)
	{
		wxArrayString items;
		wxArrayInt order;

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"items",items);
			order.resize(items.size());
			set_hash_option(hash,"order",order);
		}

		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxDefaultPosition,wxDefaultSize,order,items);
		
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRearrangeCtrl(VALUE rb_mWX)
{
#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeCtrl;
	rb_cWXRearrangeCtrl = rb_define_class_under(rb_mWX,"RearrangeCtrl",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXRearrangeCtrl,_alloc);

	rb_define_method(rb_cWXRearrangeCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRearrangeCtrl>(rb_cWXRearrangeCtrl);
#endif

}


