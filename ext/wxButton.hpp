/*
 * wxButton.hpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */

#ifndef WXBUTTON_HPP_
#define WXBUTTON_HPP_

#include "main.hpp"

extern VALUE rb_cWXButton;

void Init_WXButton(VALUE rb_mWX);

#if wxUSE_BUTTON

template <>
inline VALUE wrap< wxButton >(wxButton* window)
{
	return wrap(window,rb_cWXButton);
}


template <>
inline wxButton* wrap< wxButton* >(const VALUE &vwindow)
{
	return unwrapPtr<wxButton>(vwindow, rb_cWXButton);
}
#endif


#endif /* WXBUTTON_HPP_ */
