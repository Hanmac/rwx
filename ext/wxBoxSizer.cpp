/*
 * wxBoxSizer.cpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#include "wxSizer.hpp"

#define _self unwrap<wxBoxSizer*>(self)

VALUE rb_cWXBoxSizer;

namespace RubyWX {
namespace BoxSizer {

macro_attr_enum(Orientation,wxOrientation)

singlereturn(IsVertical)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrapTypedPtr(new wxBoxSizer(wxHORIZONTAL),self);
}


DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE orient;
	rb_scan_args(argc, argv, "01",&orient);
	if(!NIL_P(orient))
		_setOrientation(self,orient);

	rb_call_super(argc,argv);

	return self;
}


}
}



DLL_LOCAL void Init_WXBoxSizer(VALUE rb_mWX)
{
	using namespace RubyWX::BoxSizer;
	rb_cWXBoxSizer = rb_define_class_under(rb_mWX,"BoxSizer",rb_cWXSizer);
	rb_define_alloc_func(rb_cWXBoxSizer,_alloc);

	rb_define_method(rb_cWXBoxSizer,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXBoxSizer,"orientation",_getOrientation,_setOrientation);

	rb_define_method(rb_cWXBoxSizer,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);

	registerInfo<wxBoxSizer>(rb_cWXBoxSizer);

	registerEnum<wxOrientation>("wxOrientation")
			->add(wxHORIZONTAL, "horizontal")
			->add(wxVERTICAL, "vertical")
			->add(wxBOTH, "both");


}
