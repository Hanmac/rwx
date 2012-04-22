/*
 * wxStaticText.hpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICTEXT_HPP_
#define WXSTATICTEXT_HPP_

#include "main.hpp"

extern VALUE rb_cWXStaticText;

void Init_WXStaticText(VALUE rb_mWX);

#if wxUSE_STATTEXT
#include <wx/stattext.h>

template <>
inline VALUE wrap< wxStaticText >(wxStaticText* window)
{
	return wrap(window,rb_cWXStaticText);
}


template <>
inline wxStaticText* wrap< wxStaticText* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStaticText>(vwindow, rb_cWXStaticText);
}

#endif

#endif /* WXSTATICTEXT_HPP_ */
