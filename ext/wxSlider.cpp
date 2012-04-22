/*
 * wxSlider.cpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"


VALUE rb_cWXSlider;

#if wxUSE_STATBMP
#define _self wrap<wxSlider*>(self)

namespace RubyWX {
namespace Slider {

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)

VALUE _alloc(VALUE self)
{
	return wrap(new wxSlider(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,0,0,0);
	_created = true;


	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("min")))))
			_self->SetMin(NUM2INT(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("max")))))
			_self->SetMax(NUM2INT(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("value")))))
			_self->SetValue(NUM2INT(temp));
	}

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXSlider(VALUE rb_mWX)
{
#if wxUSE_STATBMP
	using namespace RubyWX::Slider;
	rb_cWXSlider = rb_define_class_under(rb_mWX,"Slider",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSlider,_alloc);

	rb_define_method(rb_cWXSlider,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSlider,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSlider,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSlider,"max",_getMax,_setMax);
#endif

}



