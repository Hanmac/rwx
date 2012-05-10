/*
 * wxSpinCtrl.hpp
 *
 *  Created on: 23.04.2012
 *      Author: hanmac
 */

#ifndef WXSPINCTRL_HPP_
#define WXSPINCTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXSpinCtrl;

DLL_LOCAL void Init_WXSpinCtrl(VALUE rb_mWX);
#if wxUSE_SPINCTRL
#include <wx/spinctrl.h>

#endif

#endif /* WXSPINCTRL_HPP_ */
