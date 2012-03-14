/*
 * wxStaticBitmap.hpp
 *
 *  Created on: 09.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICBITMAP_HPP_
#define WXSTATICBITMAP_HPP_


#include "main.hpp"

extern VALUE rb_cWXStaticBitmap;

void Init_WXStaticBitmap(VALUE rb_mWX);

#if wxUSE_STATBMP
#include <wx/statbmp.h>

template <>
inline VALUE wrap< wxStaticBitmap >(wxStaticBitmap* window)
{
	return getEvtObj(window,rb_cWXStaticBitmap);
}


template <>
inline wxStaticBitmap* wrap< wxStaticBitmap* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStaticBitmap>(vwindow, rb_cWXStaticBitmap);
}

#endif


#endif /* WXSTATICBITMAP_HPP_ */
