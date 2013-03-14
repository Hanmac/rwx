/*
 * wxRearrangeCtrl.hpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#ifndef WXREARRANGECTRL_HPP_
#define WXREARRANGECTRL_HPP_

#include "wxPanel.hpp"

extern VALUE rb_cWXRearrangeCtrl;

void Init_WXRearrangeCtrl(VALUE rb_mWX);
#if wxUSE_REARRANGECTRL
#include <wx/rearrangectrl.h>
#endif

#endif /* WXREARRANGECTRL_HPP_ */
