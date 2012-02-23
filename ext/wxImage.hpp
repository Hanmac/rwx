/*
 * wxImage.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXIMAGE_HPP_
#define WXIMAGE_HPP_


#include "main.hpp"

extern VALUE rb_cWXImage;
void Init_WXImage(VALUE rb_mWX);


template <>
inline VALUE wrap< wxImage >(wxImage *color )
{
	return Data_Wrap_Struct(rb_cWXImage, NULL, free, color);
}

template <>
inline wxImage* wrap< wxImage* >(const VALUE &vcolor)
{
	return unwrapPtr<wxImage>(vcolor, rb_cWXImage);
}

#endif /* WXIMAGE_HPP_ */
