/*
 * wxSpinCtrlDouble.hpp
 *
 *  Created on: 18.10.2014
 *      Author: hanmac
 */

#ifndef WXSPINCTRLDOUBLE_HPP_
#define WXSPINCTRLDOUBLE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXSpinCtrlDouble;

DLL_LOCAL void Init_WXSpinCtrlDouble(VALUE rb_mWX);
#if wxUSE_SPINCTRL
#include <wx/spinctrl.h>

#endif

#endif /* WXSPINCTRLDOUBLE_HPP_ */
