/*
 * wxStaticLine.hpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICLINE_HPP_
#define WXSTATICLINE_HPP_

#include "main.hpp"

extern VALUE rb_cWXStaticLine;

void Init_WXStaticLine(VALUE rb_mWX);

#if wxUSE_STATLINE
#include <wx/statline.h>

template <>
inline VALUE wrap< wxStaticLine >(wxStaticLine* window)
{
	return wrap(window,rb_cWXStaticLine);
}


template <>
inline wxStaticLine* wrap< wxStaticLine* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStaticLine>(vwindow, rb_cWXStaticLine);
}

#endif

#endif /* WXSTATICLINE_HPP_ */
