/*
 * wxControl.cpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

#if wxUSE_CONTROLS
VALUE rb_cWXControl;
#define _self unwrap<wxControl*>(self)

namespace RubyWX {
namespace Control {

macro_attr(LabelText,wxString)

APP_PROTECT(wxControl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created) {
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}


}
}

#endif
DLL_LOCAL void Init_WXControl(VALUE rb_mWX)
{
#if wxUSE_CONTROLS
	using namespace RubyWX::Control;
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXControl,_alloc);

	rb_define_method(rb_cWXControl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXControl,"label_text",_getLabelText,_setLabelText);

	registerInfo<wxControl>(rb_cWXControl);
#endif
}
