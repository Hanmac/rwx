/*
 * wxPoint.hpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#ifndef WXPOINT_HPP_
#define WXPOINT_HPP_

#include "main.hpp"

extern VALUE rb_cWXPoint;
void Init_WXPoint(VALUE rb_mWX);


template <>
inline VALUE wrap< wxPoint >(wxPoint *point )
{
	return Data_Wrap_Struct(rb_cWXPoint, NULL, free, point);
}

template <>
inline bool is_wrapable< wxPoint >(const VALUE &vpoint)
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
inline wxPoint* wrap< wxPoint* >(const VALUE &vpoint)
{
	if(!rb_obj_is_kind_of(vpoint, rb_cWXPoint) &&
		rb_respond_to(vpoint,rb_intern("x")) &&
		rb_respond_to(vpoint,rb_intern("y"))){
		wxPoint *point = new wxPoint;
		point->x = NUM2INT(rb_funcall(vpoint,rb_intern("x"),0));
		point->y = NUM2INT(rb_funcall(vpoint,rb_intern("y"),0));
		return point;
	}else{
		return unwrapPtr<wxPoint>(vpoint, rb_cWXPoint);
	}
}


template <>
inline wxPoint wrap< wxPoint >(const VALUE &vpoint)
{
	return *wrap<wxPoint*>(vpoint);
}

#endif /* WXPOINT_HPP_ */
