/*
 * wxRect.hpp
 *
 *  Created on: 23.03.2012
 *      Author: hanmac
 */

#ifndef WXRECT_HPP_
#define WXRECT_HPP_

#include "main.hpp"

extern VALUE rb_cWXRect;
void Init_WXRect(VALUE rb_mWX);


template <>
inline VALUE wrap< wxRect >(wxRect *size )
{
	return Data_Wrap_Struct(rb_cWXRect, NULL, free, size);
}

template <>
inline bool is_wrapable< wxRect >(const VALUE &vsize)
{
	if (rb_obj_is_kind_of(vsize, rb_cWXRect)){
		return true;
	}else if(rb_respond_to(vsize,rb_intern("x")) &&
		rb_respond_to(vsize,rb_intern("y")) &&
		rb_respond_to(vsize,rb_intern("width")) &&
		rb_respond_to(vsize,rb_intern("height"))){
		return true;
	}else
		return false;
}

template <>
inline wxRect* wrap< wxRect* >(const VALUE &vsize)
{
	if(!rb_obj_is_kind_of(vsize, rb_cWXRect) &&
		rb_respond_to(vsize,rb_intern("x")) &&
		rb_respond_to(vsize,rb_intern("y")) &&
		rb_respond_to(vsize,rb_intern("width")) &&
		rb_respond_to(vsize,rb_intern("height"))){
		wxRect *size = new wxRect;
		size->SetX(NUM2INT(rb_funcall(vsize,rb_intern("x"),0)));
		size->SetY(NUM2INT(rb_funcall(vsize,rb_intern("y"),0)));

		size->SetWidth(NUM2INT(rb_funcall(vsize,rb_intern("width"),0)));
		size->SetHeight(NUM2INT(rb_funcall(vsize,rb_intern("height"),0)));
		return size;
	}else{
		return unwrapPtr<wxRect>(vsize, rb_cWXRect);
	}
}


template <>
inline wxRect wrap< wxRect >(const VALUE &vsize)
{
	return *wrap<wxRect*>(vsize);
}

#endif /* WXRECT_HPP_ */
