/*
 * wxStaticBox.hpp
 *
 *  Created on: 24.04.2012
 *      Author: hanmac
 */

#ifndef WXSTATICBOX_HPP_
#define WXSTATICBOX_HPP_

#include "main.hpp"

extern VALUE rb_cWXStaticBox;

void Init_WXStaticBox(VALUE rb_mWX);

#if wxUSE_STATBOX
#include <wx/statbox.h>

template <>
inline VALUE wrap< wxStaticBox >(wxStaticBox* window)
{
	return wrap(window,rb_cWXStaticBox);
}


template <>
inline wxStaticBox* wrap< wxStaticBox* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStaticBox>(vwindow, rb_cWXStaticBox);
}

#endif


#endif /* WXSTATICBOX_HPP_ */
