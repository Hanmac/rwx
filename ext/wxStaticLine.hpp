/*
 * wxStaticLine.hpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICLINE_HPP_
#define WXSTATICLINE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXStaticLine;

void Init_WXStaticLine(VALUE rb_mWX);

#if wxUSE_STATLINE
#include <wx/statline.h>

#endif

#endif /* WXSTATICLINE_HPP_ */
