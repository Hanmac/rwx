/*
 * wxTextCtrl.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxTextCtrl.hpp"

#include "wxTextArea.hpp"
#include "wxTextEntry.hpp"

VALUE rb_cWXTextCtrl;

#if wxUSE_TEXTCTRL
#define _self unwrap<wxTextCtrl*>(self)

namespace RubyWX {
namespace TextCtrl {

macro_attr(Value,wxString)

APP_PROTECT(wxTextCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(value,Value,wxString)
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXTextCtrl(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");

	rb_define_attr(rb_cWXTextCtrl,"value",1,1);
#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextCtrl;
	rb_cWXTextCtrl = rb_define_class_under(rb_mWX,"TextCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTextCtrl,_alloc);

	rb_include_module(rb_cWXTextCtrl,rb_mWXTextArea);
	rb_include_module(rb_cWXTextCtrl,rb_mWXTextEntry);

	rb_define_method(rb_cWXTextCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXTextCtrl,"value",_getValue,_setValue);


	registerInfo<wxTextCtrl>(rb_cWXTextCtrl);
#endif

}
