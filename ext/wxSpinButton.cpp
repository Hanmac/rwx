/*
 * wxSpinButton.cpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"


VALUE rb_cWXSpinButton;

#if wxUSE_SPINBTN
#define _self wrap<wxSpinButton*>(self)

namespace RubyWX {
namespace SpinButton {

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)


VALUE _alloc(VALUE self)
{
	return wrap(new wxSpinButton(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXSpinButton(VALUE rb_mWX)
{
#if wxUSE_SPINBTN
	using namespace RubyWX::SpinButton;
	rb_cWXSpinButton = rb_define_class_under(rb_mWX,"SpinButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinButton,_alloc);

	rb_define_method(rb_cWXSpinButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinButton,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinButton,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSpinButton,"max",_getMax,_setMax);
#endif

}



