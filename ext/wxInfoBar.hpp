/*
 * wxInfoBar.hpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#ifndef WXINFOBAR_HPP_
#define WXINFOBAR_HPP_

#include "wxInfoBarBase.hpp"

extern VALUE rb_cWXInfoBar;

void Init_WXInfoBar(VALUE rb_mWX);

#if wxUSE_INFOBAR
#include <wx/infobar.h>
#endif

#endif /* WXINFOBAR_HPP_ */
