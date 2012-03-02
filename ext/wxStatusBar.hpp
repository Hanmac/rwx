/*
 * wxStatusBar.hpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#ifndef WXSTATUSBAR_HPP_
#define WXSTATUSBAR_HPP_


#include "main.hpp"

extern VALUE rb_cWXStatusBar;

void Init_WXStatusBar(VALUE rb_mWX);


template <>
inline VALUE wrap< wxStatusBar >(wxStatusBar* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXStatusBar);
}


template <>
inline wxStatusBar* wrap< wxStatusBar* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStatusBar>(vwindow, rb_cWXStatusBar);
}




#endif /* WXSTATUSBAR_HPP_ */
