/*
 * wxStaticBitmapBase.hpp
 *
 *  Created on: 12.05.2012
 *      Author: hanmac
 */

#ifndef WXSTATICBITMAPBASE_HPP_
#define WXSTATICBITMAPBASE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXStaticBitmapBase;

void Init_WXStaticBitmapBase(VALUE rb_mWX);

#if wxUSE_STATBMP
#include <wx/statbmp.h>
#include <wx/generic/statbmpg.h>
#endif

#endif /* WXSTATICBITMAPBASE_HPP_ */
