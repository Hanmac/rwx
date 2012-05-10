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
VALUE wrap< wxAnyButton >(wxAnyButton* window);

template <>
wxAnyButton* wrap< wxAnyButton* >(const VALUE &vwindow);
#endif

#endif /* WXANYBUTTON_HPP_ */
