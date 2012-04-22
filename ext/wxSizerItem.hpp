/*
 * wxSizerItem.hpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#ifndef WXSIZERITEM_HPP_
#define WXSIZERITEM_HPP_


#include "main.hpp"

extern VALUE rb_cWXSizerItem;

void Init_WXSizerItem(VALUE rb_mWX);

template <>
inline VALUE wrap< wxSizerItem >(wxSizerItem* window)
{
	return wrap(window,rb_cWXSizerItem);
}


template <>
inline wxSizerItem* wrap< wxSizerItem* >(const VALUE &vwindow)
{
	return unwrapPtr<wxSizerItem>(vwindow, rb_cWXSizerItem);
}


#endif /* WXSIZERITEM_HPP_ */
