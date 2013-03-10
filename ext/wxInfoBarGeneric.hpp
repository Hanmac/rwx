/*
 * wxInfoBarGeneric.hpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#ifndef WXINFOBARGENERIC_HPP_
#define WXINFOBARGENERIC_HPP_

#include "wxInfoBarBase.hpp"

extern VALUE rb_cWXInfoBarGeneric;

void Init_WXInfoBarGeneric(VALUE rb_mWX);

#if wxUSE_INFOBAR
#include <wx/generic/infobar.h>
#endif

#endif /* WXINFOBARGENERIC_HPP_ */
