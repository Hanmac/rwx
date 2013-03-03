/*
 * wxRect.cpp
 *
 *  Created on: 23.03.2012
 *      Author: hanmac
 */

#include "wxRect.hpp"
#include "wxPoint.hpp"
#include "wxSize.hpp"

VALUE rb_cWXRect;

#define _self unwrap<wxRect*>(self)


namespace RubyWX {
namespace Rect {

macro_attr(X,int)
macro_attr(Y,int)
macro_attr(Width,int)
macro_attr(Height,int)

macro_attr(Position,wxPoint)
macro_attr(Size,wxSize)

macro_attr(TopLeft,wxPoint)
macro_attr(TopRight,wxPoint)
macro_attr(BottomLeft,wxPoint)
macro_attr(BottomRight,wxPoint)

VALUE _alloc(VALUE self)
{
	return wrap(new wxRect());
}

VALUE _initialize(VALUE self,VALUE x,VALUE y,VALUE width,VALUE height)
{
	_setX(self,x);
	_setY(self,y);
	_setWidth(self,width);
	_setHeight(self,height);
	return self;
}

/*
*/
VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setX(self,_getX(other));
	_setY(self,_getY(other));
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
	VALUE array[6];
	array[0]=rb_str_new2("#<%s:(%d, %d, %d, %d)>");
	array[1]=rb_class_of(self);
	array[2]=_getX(self);
	array[3]=_getY(self);
	array[4]=_getWidth(self);
	array[5]=_getHeight(self);
	return rb_f_sprintf(6,array);
}

}
}


void Init_WXRect(VALUE rb_mWX)
{
	using namespace RubyWX::Rect;
	rb_cWXRect = rb_define_class_under(rb_mWX,"Rect",rb_cObject);

	rb_define_alloc_func(rb_cWXRect,_alloc);

	rb_define_method(rb_cWXRect,"initialize",RUBY_METHOD_FUNC(_initialize),4);
	rb_define_private_method(rb_cWXRect,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXRect,"x",_getX,_setX);
	rb_define_attr_method(rb_cWXRect,"y",_getY,_setY);

	rb_define_attr_method(rb_cWXRect,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXRect,"height",_getHeight,_setHeight);

	rb_define_attr_method(rb_cWXRect,"position",_getPosition,_setPosition);
	rb_define_attr_method(rb_cWXRect,"size",_getSize,_setSize);

	rb_define_method(rb_cWXRect,"inspect",RUBY_METHOD_FUNC(_inspect),0);
}



