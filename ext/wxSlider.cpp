/*
 * wxSlider.cpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */


#include "wxSlider.hpp"


VALUE rb_cWXSlider;

#if wxUSE_SLIDER
#define _self unwrap<wxSlider*>(self)

namespace RubyWX {
namespace Slider {

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)

macro_attr(LineSize,int)
macro_attr(PageSize,int)

macro_attr(ThumbLength,int)

APP_PROTECT(wxSlider)

DLL_LOCAL void set_ref_option(VALUE hash,const char* name, int& val)
{
	VALUE temp;
	if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern(name)))))
		val = NUM2INT(temp);

}

#define set_option(name,cname) \
	if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern(#name)))))\
		_self->Set##cname(NUM2INT(temp));


DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	int value=0, min=0, max=0;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_ref_option(hash,"min",min);
		set_ref_option(hash,"max",max);
		set_ref_option(hash,"value",value);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,value,min,max);
	_created = true;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(line_size,LineSize)
		set_option(page_size,PageSize)
		set_option(thumb_length,ThumbLength)
	}

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXSlider(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXSlider,"value",1,1);
	rb_define_attr(rb_cWXSlider,"min",1,1);
	rb_define_attr(rb_cWXSlider,"max",1,1);

	rb_define_attr(rb_cWXSlider,"line_size",1,1);
	rb_define_attr(rb_cWXSlider,"page_size",1,1);
	rb_define_attr(rb_cWXSlider,"thumb_length",1,1);
#endif

#if wxUSE_SLIDER
	using namespace RubyWX::Slider;
	rb_cWXSlider = rb_define_class_under(rb_mWX,"Slider",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSlider,_alloc);

	rb_define_method(rb_cWXSlider,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSlider,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSlider,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSlider,"max",_getMax,_setMax);

	rb_define_attr_method(rb_cWXSlider,"line_size",_getLineSize,_setLineSize);
	rb_define_attr_method(rb_cWXSlider,"page_size",_getPageSize,_setPageSize);
	rb_define_attr_method(rb_cWXSlider,"thumb_length",_getThumbLength,_setThumbLength);

	registerEventType("slider", wxEVT_SLIDER,rb_cWXCommandEvent);

	registerInfo<wxSlider>(rb_cWXSlider);
#endif

}



