/*
 * wxSearchCtrl.hpp
 *
 *  Created on: 14.05.2012
 *      Author: hanmac
 */

#ifndef WXSEARCHCTRL_HPP_
#define WXSEARCHCTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXSearchCtrl;

void Init_WXSearchCtrl(VALUE rb_mWX);

#if wxUSE_SEARCHCTRL
#include <wx/srchctrl.h>
#endif

#endif /* WXSEARCHCTRL_HPP_ */
