/*
 * wxRadioButton.cpp
 *
 *  Created on: 03.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXRadioButton;

#if wxUSE_RADIOBTN
#define _self wrap<wxRadioButton*>(self)

namespace RubyWX {
namespace RadioButton {

macro_attr(Value,bool)


VALUE _alloc(VALUE self)
{
	return wrap(new wxRadioButton(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXRadioButton(VALUE rb_mWX)
{
#if wxUSE_RADIOBTN
	using namespace RubyWX::RadioButton;
	rb_cWXRadioButton = rb_define_class_under(rb_mWX,"RadioButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioButton,_alloc);

	rb_define_method(rb_cWXRadioButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXRadioButton,"value",_getValue,_setValue);

#endif

}
