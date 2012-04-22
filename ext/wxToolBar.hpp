/*
 * wxToolBar.hpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#ifndef WXTOOLBAR_HPP_
#define WXTOOLBAR_HPP_

#include "main.hpp"

extern VALUE rb_cWXToolBar;

void Init_WXToolBar(VALUE rb_mWX);


template <>
inline VALUE wrap< wxToolBar >(wxToolBar* window)
{
	return wrap(window,rb_cWXToolBar);
}


template <>
inline wxToolBar* wrap< wxToolBar* >(const VALUE &vwindow)
{
	return unwrapPtr<wxToolBar>(vwindow, rb_cWXToolBar);
}



#endif /* WXTOOLBAR_HPP_ */
