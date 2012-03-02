/*
 * wxSize.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef WXSIZE_HPP_
#define WXSIZE_HPP_

#include "main.hpp"

extern VALUE rb_cWXSize;
void Init_WXSize(VALUE rb_mWX);


template <>
inline VALUE wrap< wxSize >(wxSize *size )
{
	return Data_Wrap_Struct(rb_cWXSize, NULL, free, size);
}

template <>
inline bool is_wrapable< wxSize >(const VALUE &vsize)
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
inline wxSize* wrap< wxSize* >(const VALUE &vsize)
{
	if(!rb_obj_is_kind_of(vsize, rb_cWXSize) &&
		rb_respond_to(vsize,rb_intern("width")) &&
		rb_respond_to(vsize,rb_intern("height"))){
		wxSize *size = new wxSize;
		size->SetWidth(NUM2INT(rb_funcall(vsize,rb_intern("width"),0)));
		size->SetHeight(NUM2INT(rb_funcall(vsize,rb_intern("height"),0)));
		return size;
	}else{
		return unwrapPtr<wxSize>(vsize, rb_cWXSize);
	}
}


template <>
inline wxSize wrap< wxSize >(const VALUE &vsize)
{
	return *wrap<wxSize*>(vsize);
}

#endif /* WXSIZE_HPP_ */
