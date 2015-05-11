/*
 * wxAddRemoveCtrl.hpp
 *
 *  Created on: 27.04.2015
 *      Author: hanmac
 */

#ifndef WXADDREMOVECTRL_HPP_
#define WXADDREMOVECTRL_HPP_

#include "wxPanel.hpp"

extern VALUE rb_cWXAddRemoveCtrl;
void Init_WXAddRemoveCtrl(VALUE rb_mWX);

#if wxUSE_ADDREMOVECTRL
#include <wx/addremovectrl.h>

#endif

#endif /* WXADDREMOVECTRL_HPP_ */
