/*
 * wxSpinCtrl.hpp
 *
 *  Created on: 23.04.2012
 *      Author: hanmac
 */

#ifndef WXSPINCTRL_HPP_
#define WXSPINCTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXSpinCtrl;

DLL_LOCAL void Init_WXSpinCtrl(VALUE rb_mWX);
#if wxUSE_SPINCTRL
#include <wx/spinctrl.h>
template <>
inline VALUE wrap< wxSpinCtrl >(wxSpinCtrl* window)
{
	return wrap(window,rb_cWXSpinCtrl);
}


template <>
inline wxSpinCtrl* wrap< wxSpinCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxSpinCtrl>(vwindow, rb_cWXSpinCtrl);
}

#endif

#endif /* WXSPINCTRL_HPP_ */
