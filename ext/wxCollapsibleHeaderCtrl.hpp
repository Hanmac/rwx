/*
 * wxCollapsibleHeaderCtrl.hpp
 *
 *  Created on: 07.03.2016
 *      Author: hanmac
 */

#ifndef WXCOLLAPSIBLEHEADERCTRL_HPP_
#define WXCOLLAPSIBLEHEADERCTRL_HPP_

#include "wxWindow.hpp"

extern VALUE rb_cWXCollapsibleHeaderCtrl;
DLL_LOCAL void Init_WXCollapsibleHeaderCtrl(VALUE rb_mWX);

#if wxUSE_COLLPANE && HAVE_WXCOLLAPSIBLEHEADERCTRL
#include <wx/collheaderctrl.h>
#endif

#endif /* WXCOLLAPSIBLEHEADERCTRL_HPP_ */
