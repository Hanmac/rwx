/*
 * wxRearrangeList.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRearrangeList.hpp"

VALUE rb_cWXRearrangeList;

#if wxUSE_REARRANGECTRL

namespace RubyWX {
namespace RearrangeList {
#define _self unwrap<wxRearrangeList*>(self)

APP_PROTECT(wxRearrangeList)

singlereturn(CanMoveCurrentUp)
singlereturn(CanMoveCurrentDown)

singlefunc(MoveCurrentUp)
singlefunc(MoveCurrentDown)

/*
 * call-seq:
 *   RearrangeList.new(parent, name, [options])
 *   RearrangeList.new(parent, [options])
 *
 * creates a new RearrangeList widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * items [string]
 *   * order [Integer]
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxArrayString items;
		wxArrayInt order;
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"items",items);
			set_hash_option(hash,"order",order);
			order.resize(items.size());
			set_hash_option(hash,"style",style);
		}
		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,order,items,style
			);
		}
		
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRearrangeList(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
	rb_cWXCheckListBox = rb_define_class_under(rb_mWX,"CheckListBox",rb_cWXListBox);
#endif

#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeList;
	rb_cWXRearrangeList = rb_define_class_under(rb_mWX,"RearrangeList",rb_cWXCheckListBox);
	rb_define_alloc_func(rb_cWXRearrangeList,_alloc);

	rb_define_method(rb_cWXRearrangeList,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRearrangeList>(rb_cWXRearrangeList);
#endif

}


