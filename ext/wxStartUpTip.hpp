/*
 * wxStartUpTip.hpp
 *
 *  Created on: 09.03.2012
 *      Author: hanmac
 */

#ifndef WXSTARTUPTIP_HPP_
#define WXSTARTUPTIP_HPP_

#include "main.hpp"
#if wxUSE_STARTUP_TIPS
#include <wx/tipdlg.h>
#endif
void Init_ShowTip(VALUE rb_mWX);
#endif /* WXSTARTUPTIP_HPP_ */
