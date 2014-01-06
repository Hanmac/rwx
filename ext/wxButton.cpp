/*
 * wxButton.cpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */

#include "wxButton.hpp"

VALUE rb_cWXButton;
#if wxUSE_BUTTON
#define _self unwrap<wxButton*>(self)

namespace RubyWX {
namespace Button {

macro_attr(AuthNeeded,bool)

APP_PROTECT(wxButton)

/*
 * call-seq:
 *   Button.new(parent, [options])
 *
 * creates a new Button widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	wxWindowID id = wxID_ANY;
	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_hash_option(hash,"id",id,unwrapID);
	}

	if(!_created) {
		_self->Create(unwrap<wxWindow*>(parent),id);
		
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXButton(VALUE rb_mWX)
{
#if 0
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);
	rb_define_attr(rb_cWXButton,"auth_needed",1,1);
#endif

#if wxUSE_BUTTON
	using namespace RubyWX::Button;
	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);
	rb_define_alloc_func(rb_cWXButton,_alloc);

	rb_define_method(rb_cWXButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXButton,"auth_needed",_getAuthNeeded,_setAuthNeeded);

	registerInfo<wxButton>(rb_cWXButton);

	registerEventType<wxCommandEvent>("button",wxEVT_BUTTON);

#endif
}
