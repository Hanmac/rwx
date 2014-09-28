/*
 * wxRealPoint.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxPoint.hpp"
VALUE rb_cwxPoint;

ID rwxID_x,rwxID_y;

#define _self unwrap<wxRealPoint*>(self)


template <>
VALUE wrap< wxRealPoint >(wxRealPoint *point )
{
	return wrapTypedPtr(point, rb_cwxPoint);
}

template <>
VALUE wrap< wxPoint >(const wxPoint &point )
{
	return wrap(new wxRealPoint(point));
}

template <>
bool is_wrapable< wxRealPoint >(const VALUE &vpoint)
{
	if (rb_obj_is_kind_of(vpoint, rb_cwxPoint)){
		return true;
	}else if(rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		return true;
	}else
		return false;
}
template <>
bool is_wrapable< wxPoint >(const VALUE &vpoint)
{
	return is_wrapable< wxRealPoint >(vpoint);
}

template <>
wxRealPoint unwrap< wxRealPoint >(const VALUE &vpoint)
{
	if(rb_obj_is_kind_of(vpoint, rb_cArray)){
		wxRealPoint point;
		point.x = NUM2DBL(RARRAY_AREF(vpoint,0));
		point.y = NUM2DBL(RARRAY_AREF(vpoint,1));
		return point;
	}else if(!rb_obj_is_kind_of(vpoint, rb_cwxPoint) &&
		rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		wxRealPoint point;
		point.x = NUM2DBL(rb_funcall(vpoint,rwxID_x,0));
		point.y = NUM2DBL(rb_funcall(vpoint,rwxID_y,0));
		return point;
	}else{
		return *unwrap<wxRealPoint*>(vpoint);
	}

}


template <>
wxPoint unwrap< wxPoint >(const VALUE &vpoint)
{
	if(rb_obj_is_kind_of(vpoint, rb_cArray)){
		wxPoint point;
		point.x = NUM2INT(RARRAY_AREF(vpoint,0));
		point.y = NUM2INT(RARRAY_AREF(vpoint,1));
		return point;
	}else if(!rb_obj_is_kind_of(vpoint, rb_cwxPoint) &&
		rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		wxPoint point;
		point.x = NUM2INT(rb_funcall(vpoint,rwxID_x,0));
		point.y = NUM2INT(rb_funcall(vpoint,rwxID_y,0));
		return point;
	}else{
		return *unwrap<wxRealPoint*>(vpoint);
	}

}

template <>
wxPointList* unwrap< wxPointList* >(const VALUE &val)
{
	wxPointList *result = new wxPointList;
	VALUE dp(rb_Array(val));
	std::size_t length = RARRAY_LEN(dp);
	for(std::size_t i = 0; i < length; ++i)
		result->Append(new wxPoint(unwrap<wxPoint>(RARRAY_AREF(dp,i))));
	return result;
}

namespace RubyWX {
namespace Point {

macro_attr_prop(x,double)
macro_attr_prop(y,double)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrap(new wxRealPoint());
}

DLL_LOCAL VALUE _initialize(VALUE self,VALUE x,VALUE y)
{
	_set_x(self,x);
	_set_y(self,y);
	return self;
}

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_set_x(self,_get_x(other));
	_set_y(self,_get_y(other));
	return result;
}

/* Document-method: inspect
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
DLL_LOCAL VALUE _inspect(VALUE self)
{
	return rb_sprintf( "%s(%f, %f)",
		rb_obj_classname( self ),
		NUM2DBL(_get_x(self)),
		NUM2DBL(_get_y(self)));
}

/* Document-method: marshal_dump
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
    VALUE ptr[2];
    ptr[0] = _get_x(self);
    ptr[1] = _get_y(self);
    return rb_ary_new4(2, ptr);
}

/* Document-method: marshal_load
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self, VALUE data)
{
    _set_x(self, RARRAY_AREF(data,0));
    _set_y(self, RARRAY_AREF(data,1));
    return Qnil;
}


}
}



/* Document-class: WX::Point
 *
 * This class represents an Point.
 */

/* Document-attr: x
 * returns the x value of Point.
 */
/* Document-attr: y
 * returns the y value of Point.
 */

/**/
DLL_LOCAL void Init_WXPoint(VALUE rb_mWX)
{

#if 0
	rb_define_attr(rb_cwxPoint,"x",1,1);
	rb_define_attr(rb_cwxPoint,"y",1,1);
#endif

	using namespace RubyWX::Point;
	rb_cwxPoint = rb_define_class_under(rb_mWX,"Point",rb_cObject);

	rb_define_alloc_func(rb_cwxPoint,_alloc);

	rb_define_method(rb_cwxPoint,"initialize",RUBY_METHOD_FUNC(_initialize),2);
	rb_define_private_method(rb_cwxPoint,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cwxPoint,"x",_get_x,_set_x);
	rb_define_attr_method(rb_cwxPoint,"y",_get_y,_set_y);

	rb_define_method(rb_cwxPoint,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cwxPoint,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cwxPoint,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerType<wxRealPoint>(rb_cwxPoint, true);

	rwxID_x = rb_intern("x");
	rwxID_y = rb_intern("y");
}



