/*
 * wxSize.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
VALUE rb_cWXSize;

#define _self wrap<wxSize*>(self)

template <>
VALUE wrap< wxSize >(wxSize *size )
{
	return Data_Wrap_Struct(rb_cWXSize, NULL, free, size);
}

template <>
bool is_wrapable< wxSize >(const VALUE &vsize)
{
	if (rb_obj_is_kind_of(vsize, rb_cWXSize)){
		return true;
	}else if(rb_respond_to(vsize,rb_intern("width")) &&
		rb_respond_to(vsize,rb_intern("height"))){
		return true;
	}else
		return false;
}

template <>
wxSize* wrap< wxSize* >(const VALUE &vsize)
{
	return unwrapPtr<wxSize>(vsize, rb_cWXSize);
}


template <>
wxSize wrap< wxSize >(const VALUE &vsize)
{
	if(!rb_obj_is_kind_of(vsize, rb_cWXSize) &&
		rb_respond_to(vsize,rb_intern("width")) &&
		rb_respond_to(vsize,rb_intern("height"))){
		wxSize size;
		size.SetWidth(NUM2INT(rb_funcall(vsize,rb_intern("width"),0)));
		size.SetHeight(NUM2INT(rb_funcall(vsize,rb_intern("height"),0)));
		return size;
	}else{
		return *wrap<wxSize*>(vsize);
	}
}


namespace RubyWX {
namespace Size {

macro_attr(Width,int)
macro_attr(Height,int)

VALUE _alloc(VALUE self)
{
	return wrap(new wxSize());
}

VALUE _initialize(VALUE self,VALUE width,VALUE height)
{
	_setWidth(self,width);
	_setHeight(self,height);
	return self;
}

/*
*/
VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setWidth(self,_getWidth(other));
	_setHeight(self,_getHeight(other));
	return result;
}
/*
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
VALUE _inspect(VALUE self)
{
	VALUE array[4];
	array[0]=rb_str_new2("#<%s:(%d, %d)>");
	array[1]=rb_class_of(self);
	array[2]=_getWidth(self);
	array[3]=_getHeight(self);
	return rb_f_sprintf(4,array);
}

}
}

void Init_WXSize(VALUE rb_mWX)
{
	using namespace RubyWX::Size;
	rb_cWXSize = rb_define_class_under(rb_mWX,"Size",rb_cObject);

	rb_define_alloc_func(rb_cWXSize,_alloc);
	rb_define_method(rb_cWXSize,"initialize",RUBY_METHOD_FUNC(_initialize),2);
	rb_define_private_method(rb_cWXSize,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXSize,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXSize,"height",_getHeight,_setHeight);

	rb_define_method(rb_cWXSize,"inspect",RUBY_METHOD_FUNC(_inspect),0);

}
