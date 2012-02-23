/*
 * wxBitmap.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXBITMAP_HPP_
#define WXBITMAP_HPP_

#include "main.hpp"

extern VALUE rb_cWXBitmap;
void Init_WXBitmap(VALUE rb_mWX);


template <>
inline VALUE wrap< wxBitmap >(wxBitmap *color )
{
	return Data_Wrap_Struct(rb_cWXBitmap, NULL, free, color);
}

template <>
inline wxBitmap* wrap< wxBitmap* >(const VALUE &vcolor)
{
	return unwrapPtr<wxBitmap>(vcolor, rb_cWXBitmap);
}



#endif /* WXBITMAP_HPP_ */
