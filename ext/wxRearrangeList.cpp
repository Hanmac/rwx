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

/*
 * call-seq:
 *   RearrangeList.new(parent, [options])
 *
 * creates a new RearrangeList widget.
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
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxDefaultPosition,wxDefaultSize,wxArrayInt(),wxArrayString());
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRearrangeList(VALUE rb_mWX)
{
#if wxUSE_REARRANGECTRL
	using namespace RubyWX::RearrangeList;
	rb_cWXRearrangeList = rb_define_class_under(rb_mWX,"RearrangeList",rb_cWXCheckListBox);
	rb_define_alloc_func(rb_cWXRearrangeList,_alloc);

	rb_define_method(rb_cWXRearrangeList,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxRearrangeList>(rb_cWXRearrangeList);
#endif

}


