/*
 * wxBrush.hpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#ifndef WXBRUSH_HPP_
#define WXBRUSH_HPP_


#include "main.hpp"

extern VALUE rb_cWXBrush;
void Init_WXBrush(VALUE rb_mWX);

//template <>
//inline VALUE wrap< wxBrush >(wxBrush *bitmap )
//{
//	if(bitmap->IsOk())
//		return wrap(bitmap,rb_cWXBrush);
//	return Qnil;
//}
//
//template <>
//inline VALUE wrap< wxBrush >(const wxBrush &bitmap )
//{
//	if(bitmap.IsOk())
//		return wrap(const_cast<wxBrush*>(&bitmap),rb_cWXBrush);
//	return Qnil;
//}

template <>
VALUE wrap< wxBrushStyle >(const wxBrushStyle &style );

template <>
wxBrush* nullPtr<wxBrush*>();
template <>
wxBrush nullPtr<wxBrush>();

#endif /* WXBRUSH_HPP_ */
