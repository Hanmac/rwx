/*
 * wxFileCtrl.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#ifndef WXFILECTRL_HPP_
#define WXFILECTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXFileCtrl;

void Init_WXFileCtrl(VALUE rb_mWX);
#if wxUSE_FILECTRL
#include <wx/filectrl.h>
template <>
inline VALUE wrap< wxFileCtrl >(wxFileCtrl* window)
{
	return wrap(window,rb_cWXFileCtrl);
}


template <>
inline wxFileCtrl* wrap< wxFileCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFileCtrl>(vwindow, rb_cWXFileCtrl);
}

#endif

#endif /* WXFILECTRL_HPP_ */
