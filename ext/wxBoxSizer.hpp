/*
 * wxBoxSizer.hpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#ifndef WXBOXSIZER_HPP_
#define WXBOXSIZER_HPP_

#include "main.hpp"

extern VALUE rb_cWXBoxSizer;

void Init_WXBoxSizer(VALUE rb_mWX);

template <>
inline VALUE wrap< wxBoxSizer >(wxBoxSizer* window)
{
	return wrap(window,rb_cWXBoxSizer);
}


template <>
inline wxBoxSizer* wrap< wxBoxSizer* >(const VALUE &vwindow)
{
	return unwrapPtr<wxBoxSizer>(vwindow, rb_cWXBoxSizer);
}

#endif /* WXBOXSIZER_HPP_ */
