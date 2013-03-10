/*
 * wxInfobarBase.hpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#ifndef WXINFOBARBASE_HPP_
#define WXINFOBARBASE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXInfoBarBase;

void Init_WXInfoBarBase(VALUE rb_mWX);

#if wxUSE_INFOBAR
#include <wx/infobar.h>
#endif

#endif /* WXINFOBARBASE_HPP_ */
