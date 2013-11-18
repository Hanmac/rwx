/*
 * wxPoint.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxPoint.hpp"
VALUE rb_cWXPoint;

#define _self unwrap<wxPoint*>(self)


template <>
VALUE wrap< wxPoint >(wxPoint *point )
{
	return Data_Wrap_Struct(rb_cWXPoint, NULL, free, point);
}

template <>
bool is_wrapable< wxPoint >(const VALUE &vpoint)
{
	if (rb_obj_is_kind_of(vpoint, rb_cWXPoint)){
		return true;
	}else if(rb_respond_to(vpoint,rb_intern("x")) &&
		rb_respond_to(vpoint,rb_intern("y"))){
		return true;
	}else
		return false;
}

template <>
wxPoint unwrap< wxPoint >(const VALUE &vpoint)
{
	if(rb_obj_is_kind_of(vpoint, rb_cArray)){
		wxPoint point;
		point.x = NUM2INT(rb_ary_entry(vpoint,0));
		point.y = NUM2INT(rb_ary_entry(vpoint,1));
		return point;
	}else if(!rb_obj_is_kind_of(vpoint, rb_cWXPoint) &&
		rb_respond_to(vpoint,rb_intern("x")) &&
		rb_respond_to(vpoint,rb_intern("y"))){
		wxPoint point;
		point.x = NUM2INT(rb_funcall(vpoint,rb_intern("x"),0));
		point.y = NUM2INT(rb_funcall(vpoint,rb_intern("y"),0));
		return point;
	}else{
		return *unwrap<wxPoint*>(vpoint);
	}

}

template <>
wxPointList* unwrap< wxPointList* >(const VALUE &val)
{
	wxPointList *result = new wxPointList;
	VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
	size_t length = RARRAY_LEN(dp);
	for(size_t i = 0; i < length; ++i)
		result->Append(new wxPoint(unwrap<wxPoint>(RARRAY_PTR(dp)[i])));
	return result;
}

namespace RubyWX {
namespace Point {

macro_attr_prop(x,int)
macro_attr_prop(y,int)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrap(new wxPoint());
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
	return rb_sprintf( "%s(%f, %f)",
		rb_obj_classname( self ),
		NUM2DBL(_get_x(self)),
		NUM2DBL(_get_y(self)));
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
    VALUE ptr[2];
    ptr[0] = _get_x(self);
    ptr[1] = _get_y(self);
    return rb_ary_new4(2, ptr);
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
    VALUE* ptr = RARRAY_PTR(data);
    _set_x(self, ptr[0]);
    _set_y(self, ptr[1]);
    return Qnil;
}


}
}



/*
 * Document-class: WX::Point
 *
 * This class represents an Point.
*/

/* Document-attr: x
 * returns the x value of Point. */
/* Document-attr: y
 * returns the y value of Point. */

DLL_LOCAL void Init_WXPoint(VALUE rb_mWX)
{

#if 0
	rb_define_attr(rb_cWXPoint,"x",1,1);
	rb_define_attr(rb_cWXPoint,"y",1,1);
#endif

	using namespace RubyWX::Point;
	rb_cWXPoint = rb_define_class_under(rb_mWX,"Point",rb_cObject);

	rb_define_alloc_func(rb_cWXPoint,_alloc);

	rb_define_method(rb_cWXPoint,"initialize",RUBY_METHOD_FUNC(_initialize),2);
	rb_define_private_method(rb_cWXPoint,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXPoint,"x",_get_x,_set_x);
	rb_define_attr_method(rb_cWXPoint,"y",_get_y,_set_y);

	rb_define_method(rb_cWXPoint,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cWXPoint,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXPoint,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerType<wxPoint>(rb_cWXPoint);
}



