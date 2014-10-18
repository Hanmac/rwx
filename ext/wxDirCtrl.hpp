/*
 * wxDirCtrl.hpp
 *
 *  Created on: 29.09.2014
 *      Author: hanmac
 */

#ifndef WXDIRCTRL_HPP_
#define WXDIRCTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXDirCtrl;

void Init_WXDirCtrl(VALUE rb_mWX);

#if wxUSE_DIRDLG
#include <wx/dirctrl.h>
#endif

#endif /* WXDIRCTRL_HPP_ */
