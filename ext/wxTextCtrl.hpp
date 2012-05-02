/*
 * wxtextCtrl.hpp
 *
 *  Created on: 28.04.2012
 *      Author: hanmac
 */

#ifndef WXTEXTCTRL_HPP_
#define WXTEXTCTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXTextCtrl;

void Init_WXTextCtrl(VALUE rb_mWX);

#if wxUSE_TEXTCTRL
#include <wx/textctrl.h>

template <>
inline VALUE wrap< wxTextCtrl >(wxTextCtrl* window)
{
	return wrap(window,rb_cWXTextCtrl);
}


template <>
inline wxTextCtrl* wrap< wxTextCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxTextCtrl>(vwindow, rb_cWXTextCtrl);
}

#endif


#endif /* WXTEXTCTRL_HPP_ */
