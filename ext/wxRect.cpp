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

template <>
VALUE wrap< wxRect >(wxRect *size )
{
	return Data_Wrap_Struct(rb_cWXRect, NULL, free, size);
}

template <>
bool is_wrapable< wxRect >(const VALUE &vsize)
{
	if (rb_obj_is_kind_of(vsize, rb_cWXRect)){
		return true;
	}else if(rb_respond_to(vsize,rwxID_x) &&
		rb_respond_to(vsize,rwxID_y) &&
		rb_respond_to(vsize,rwxID_width) &&
		rb_respond_to(vsize,rwxID_height)){
		return true;
	}else
		return false;
}

template <>
wxRect* unwrap< wxRect* >(const VALUE &vsize)
{
	if(!rb_obj_is_kind_of(vsize, rb_cWXRect) &&
		rb_respond_to(vsize,rwxID_x) &&
		rb_respond_to(vsize,rwxID_y) &&
		rb_respond_to(vsize,rwxID_width) &&
		rb_respond_to(vsize,rwxID_height)){
		wxRect *size = new wxRect;
		size->SetX(NUM2INT(rb_funcall(vsize,rwxID_x,0)));
		size->SetY(NUM2INT(rb_funcall(vsize,rwxID_y,0)));

		size->SetWidth(NUM2INT(rb_funcall(vsize,rwxID_width,0)));
		size->SetHeight(NUM2INT(rb_funcall(vsize,rwxID_height,0)));
		return size;
	}else{
		return unwrapPtr<wxRect>(vsize, rb_cWXRect);
	}
}

template <>
wxRect unwrap< wxRect >(const VALUE &vsize)
{
	return *unwrap<wxRect*>(vsize);
}


namespace RubyWX {
namespace Rect {

macro_attr(X,int)
macro_attr(Y,int)
macro_attr(Width,int)
macro_attr(Height,int)

macro_attr(Position,wxPoint)
macro_attr(Size,wxSize)

macro_attr(Left,int)
macro_attr(Top,int)
macro_attr(Bottom,int)
macro_attr(Right,int)

macro_attr(TopLeft,wxPoint)
macro_attr(TopRight,wxPoint)
macro_attr(BottomLeft,wxPoint)
macro_attr(BottomRight,wxPoint)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrap(new wxRect());
}

DLL_LOCAL VALUE _initialize(VALUE self,VALUE x,VALUE y,VALUE width,VALUE height)
{
	_setX(self,x);
	_setY(self,y);
	_setWidth(self,width);
	_setHeight(self,height);
	return self;
}

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
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
DLL_LOCAL VALUE _inspect(VALUE self)
{
	return rb_sprintf( "%s(%d, %d, %d, %d)",
		rb_obj_classname( self ),
		FIX2INT(_getX(self)),
		FIX2INT(_getY(self)),
		FIX2INT(_getWidth(self)),
		FIX2INT(_getHeight(self)));
}

/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
    VALUE ptr[4];
    ptr[0] = _getX(self);
    ptr[1] = _getY(self);
    ptr[2] = _getWidth(self);
    ptr[3] = _getHeight(self);
    return rb_ary_new4( 4, ptr );
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self, VALUE data)
{
	_setX(self, RARRAY_AREF(data,0));
    _setY(self, RARRAY_AREF(data,1));
    _setWidth(self, RARRAY_AREF(data,2));
    _setHeight(self, RARRAY_AREF(data,3));
    return Qnil;
}


}
}

/*
 * Document-class: WX::Rect
 *
 * This class represents an Rect.
 */

/* Document-attr: x
 * returns the x value of Rect. Integer
 */
/* Document-attr: y
 * returns the y value of Rect. Integer
 */
/* Document-attr: width
 * returns the width value of Rect. Integer
 */
/* Document-attr: height
 * returns the height value of Rect. Integer
 */

/* Document-attr: position
 * returns the position of Rect. WX::Point
 */
/* Document-attr: size
 * returns the size of Rect. WX::Size
 */

/* Document-attr: left
 * returns the left of Rect. Integer
 */
/* Document-attr: top
 * returns the top of Rect. Integer
 */
/* Document-attr: bottom
 * returns the bottom of Rect. Integer
 */
/* Document-attr: right
 * returns the right of Rect. Integer
 */

/* Document-attr: top_left
 * returns the top left of Rect. WX::Point
 */
/* Document-attr: top_right
 * returns the top right of Rect. WX::Point
 */
/* Document-attr: bottom_left
 * returns the bottom left of Rect. WX::Point
 */
/* Document-attr: bottom_right
 * returns the bottom right of Rect. WX::Point
 */

DLL_LOCAL void Init_WXRect(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");

	rb_define_attr(rb_cWXRect,"x",1,1);
	rb_define_attr(rb_cWXRect,"y",1,1);
	rb_define_attr(rb_cWXRect,"width",1,1);
	rb_define_attr(rb_cWXRect,"height",1,1);

	rb_define_attr(rb_cWXRect,"position",1,1);
	rb_define_attr(rb_cWXRect,"size",1,1);

	rb_define_attr(rb_cWXRect,"left",1,1);
	rb_define_attr(rb_cWXRect,"top",1,1);
	rb_define_attr(rb_cWXRect,"bottom",1,1);
	rb_define_attr(rb_cWXRect,"right",1,1);

	rb_define_attr(rb_cWXRect,"top_left",1,1);
	rb_define_attr(rb_cWXRect,"top_right",1,1);
	rb_define_attr(rb_cWXRect,"bottom_left",1,1);
	rb_define_attr(rb_cWXRect,"bottom_right",1,1);

#endif

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

	rb_define_attr_method(rb_cWXRect,"left",_getLeft,_setLeft);
	rb_define_attr_method(rb_cWXRect,"top",_getTop,_setTop);
	rb_define_attr_method(rb_cWXRect,"bottom",_getBottom,_setBottom);
	rb_define_attr_method(rb_cWXRect,"right",_getRight,_setRight);

	rb_define_attr_method(rb_cWXRect,"top_left",_getTopLeft,_setTopLeft);
	rb_define_attr_method(rb_cWXRect,"top_right",_getTopRight,_setTopRight);
	rb_define_attr_method(rb_cWXRect,"bottom_left",_getBottomLeft,_setBottomLeft);
	rb_define_attr_method(rb_cWXRect,"bottom_right",_getBottomRight,_setBottomRight);

	rb_define_method(rb_cWXRect,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cWXRect,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXRect,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);
}



