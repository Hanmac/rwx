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
 *   Button.new(parent, name, [options])
 *   Button.new(parent, [options])
 *
 * creates a new Button widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString label(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,label);
		
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
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

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
