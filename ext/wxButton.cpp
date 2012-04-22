/*
 * wxButton.cpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXButton;

#define _self wrap<wxButton*>(self)

namespace RubyWX {
namespace Button {

macro_attr(AuthNeeded,bool)

VALUE _alloc(VALUE self)
{
	return wrap(new wxButton,self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	wxWindowID id = wxID_ANY;
	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("id")))))
			id = unwrapID(temp);
	}

	if(!_created) {
		_self->Create(wrap<wxWindow*>(parent),id);
		_created = true;
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

void Init_WXButton(VALUE rb_mWX)
{
	using namespace RubyWX::Button;
	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);
	rb_define_alloc_func(rb_cWXButton,_alloc);

	rb_define_method(rb_cWXButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXButton,"auth_needed",_getAuthNeeded,_setAuthNeeded);

}
