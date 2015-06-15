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
VALUE wrap< wxRect >(wxRect *rect )
{
	return wrapTypedPtr(rect, rb_cWXRect);
}

template <>
bool is_wrapable< wxRect >(const VALUE &vrect)
{
	if (rb_obj_is_kind_of(vrect, rb_cWXRect)){
		return true;
	}else if(rb_respond_to(vrect,rwxID_x) &&
		rb_respond_to(vrect,rwxID_y) &&
		rb_respond_to(vrect,rwxID_width) &&
		rb_respond_to(vrect,rwxID_height)){
		return true;
	}else
		return false;
}

template <>
wxRect unwrap< wxRect >(const VALUE &vrect)
{
	if(rb_obj_is_kind_of(vrect, rb_cArray) && RARRAY_LEN(vrect) == 4 ){
		wxRect rect;
		rect.SetX(NUM2INT(RARRAY_AREF(vrect,0)));
		rect.SetY(NUM2INT(RARRAY_AREF(vrect,1)));
		rect.SetWidth(NUM2INT(RARRAY_AREF(vrect,2)));
		rect.SetHeight(NUM2INT(RARRAY_AREF(vrect,3)));
		return rect;
	}else if(rb_obj_is_kind_of(vrect, rb_cHash)){
		wxRect rect;
		rect.SetX(NUM2INT(rb_hash_aref(vrect,ID2SYM(rwxID_x))));
		rect.SetY(NUM2INT(rb_hash_aref(vrect,ID2SYM(rwxID_y))));
		rect.SetWidth(NUM2INT(rb_hash_aref(vrect,ID2SYM(rwxID_width))));
		rect.SetHeight(NUM2INT(rb_hash_aref(vrect,ID2SYM(rwxID_height))));
		return rect;
	}else if(!rb_obj_is_kind_of(vrect, rb_cWXRect) &&
		rb_respond_to(vrect,rwxID_x) &&
		rb_respond_to(vrect,rwxID_y) &&
		rb_respond_to(vrect,rwxID_width) &&
		rb_respond_to(vrect,rwxID_height)){
		wxRect rect;
		rect.SetX(NUM2INT(rb_funcall(vrect,rwxID_x,0)));
		rect.SetY(NUM2INT(rb_funcall(vrect,rwxID_y,0)));

		rect.SetWidth(NUM2INT(rb_funcall(vrect,rwxID_width,0)));
		rect.SetHeight(NUM2INT(rb_funcall(vrect,rwxID_height,0)));
		return rect;
	}else{
		return *unwrap<wxRect*>(vrect);
	}
}


template <>
wxRect* unwrap< wxRect* >(const VALUE &vrect)
{
	return unwrapTypedPtr<wxRect>(vrect, rb_cWXRect);
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
	return wrapTypedPtr(new wxRect,self);
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
    data = rb_Array(data);
	_setX(self, RARRAY_AREF(data,0));
    _setY(self, RARRAY_AREF(data,1));
    _setWidth(self, RARRAY_AREF(data,2));
    _setHeight(self, RARRAY_AREF(data,3));
    return Qnil;
}



struct equal_obj {
	wxRect* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(*obj->self == unwrap<wxRect>(obj->other));
}

VALUE _equal_rescue(VALUE val)
{
	return Qfalse;
}

/*
 * call-seq:
 *   == rect -> bool
 *
 * compares two rects.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
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
#endif

	using namespace RubyWX::Rect;
	rb_cWXRect = rb_define_class_under(rb_mWX,"Rect",rb_cObject);

	rb_define_alloc_func(rb_cWXRect,_alloc);

#if 0
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

	rb_define_method(rb_cWXRect,"==",RUBY_METHOD_FUNC(_equal),1);

	rb_define_method(rb_cWXRect,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXRect,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerType<wxRect>(rb_cWXRect, true);
}



