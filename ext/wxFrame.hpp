/*
 * wxFrame.hpp
 *
 *  Created on: 06.02.2012
 *      Author: hanmac
 */

#ifndef WXFRAME_HPP_
#define WXFRAME_HPP_

#include "wxToplevel.hpp"

extern VALUE rb_cWXFrame;

void Init_WXFrame(VALUE rb_mWX);

template <>
inline VALUE wrap< wxFrame >(wxFrame* window)
{
	return wrap(window,rb_cWXFrame);
}


template <>
inline wxFrame* wrap< wxFrame* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFrame>(vwindow, rb_cWXFrame);
}

#endif /* WXFRAME_HPP_ */
