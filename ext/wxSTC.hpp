/*
 * wxSTC.hpp
 *
 *  Created on: 01.04.2012
 *      Author: hanmac
 */

#ifndef WXSTC_HPP_
#define WXSTC_HPP_

#include "main.hpp"

extern VALUE rb_cWXSTC;

void Init_WXSTC(VALUE rb_mWX);
#if wxUSE_STC
#include <wx/stc/stc.h>
template <>
inline VALUE wrap< wxStyledTextCtrl >(wxStyledTextCtrl* window)
{
	return wrap(window,rb_cWXSTC);
}


template <>
inline wxStyledTextCtrl* wrap< wxStyledTextCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxStyledTextCtrl>(vwindow, rb_cWXSTC);
}

#endif


#endif /* WXSTC_HPP_ */
