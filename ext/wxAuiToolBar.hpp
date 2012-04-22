/*
 * wxAuiToolBar.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXAUITOOLBAR_HPP_
#define WXAUITOOLBAR_HPP_

#include "wxAui.hpp"

extern VALUE rb_cWXAuiToolBar;

void Init_WXAuiToolBar(VALUE rb_mWX);


template <>
inline VALUE wrap< wxAuiToolBar >(wxAuiToolBar* window)
{
	return wrap(window,rb_cWXAuiToolBar);
}


template <>
inline wxAuiToolBar* wrap< wxAuiToolBar* >(const VALUE &vwindow)
{
	return unwrapPtr<wxAuiToolBar>(vwindow, rb_cWXAuiToolBar);
}


#endif /* WXAUITOOLBAR_HPP_ */
