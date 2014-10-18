/*
 * wxComboCtrl.hpp
 *
 *  Created on: 14.10.2014
 *      Author: hanmac
 */

#ifndef WXCOMBOCTRL_HPP_
#define WXCOMBOCTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXComboCtrl;

void Init_WXComboCtrl(VALUE rb_mWX);

#if wxUSE_COMBOCTRL
#include <wx/combo.h>
#endif


#endif /* WXCOMBOCTRL_HPP_ */
