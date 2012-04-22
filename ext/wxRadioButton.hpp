/*
 * wxRadioButton.hpp
 *
 *  Created on: 03.03.2012
 *      Author: hanmac
 */

#ifndef WXRADIOBUTTON_HPP_
#define WXRADIOBUTTON_HPP_

#include "main.hpp"

extern VALUE rb_cWXRadioButton;

void Init_WXRadioButton(VALUE rb_mWX);
#if wxUSE_RADIOBTN

template <>
inline VALUE wrap< wxRadioButton >(wxRadioButton* window)
{
	return wrap(window,rb_cWXRadioButton);
}


template <>
inline wxRadioButton* wrap< wxRadioButton* >(const VALUE &vwindow)
{
	return unwrapPtr<wxRadioButton>(vwindow, rb_cWXRadioButton);
}

#endif
#endif /* WXRADIOBUTTON_HPP_ */
