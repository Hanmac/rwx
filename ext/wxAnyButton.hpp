/*
 * wxAnyButton.hpp
 *
 *  Created on: 13.02.2012
 *      Author: hanmac
 */

#ifndef WXANYBUTTON_HPP_
#define WXANYBUTTON_HPP_

#include "wxButton.hpp"

extern VALUE rb_cWXAnyButton;

void Init_WXAnyButton(VALUE rb_mWX);

#ifdef wxHAS_ANY_BUTTON

template <>
inline VALUE wrap< wxAnyButton >(wxAnyButton* window)
{
#if wxUSE_BUTTON
	if(wxButton *button = dynamic_cast<wxButton*>(window))
		return wrap(button);
#endif

	return wrap(window,rb_cWXAnyButton);
}


template <>
inline wxAnyButton* wrap< wxAnyButton* >(const VALUE &vwindow)
{
	return unwrapPtr<wxAnyButton>(vwindow, rb_cWXAnyButton);
}
#endif

#endif /* WXANYBUTTON_HPP_ */
