/*
 * wxSize.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
VALUE rb_cWXSize;

#define _self wrap<wxSize*>(self)

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
