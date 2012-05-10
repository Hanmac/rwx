/*
 * wxStaticBitmap.hpp
 *
 *  Created on: 09.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICBITMAP_HPP_
#define WXSTATICBITMAP_HPP_


#include "wxControl.hpp"

extern VALUE rb_cWXStaticBitmap;

void Init_WXStaticBitmap(VALUE rb_mWX);

#if wxUSE_STATBMP
#include <wx/statbmp.h>
#endif


#endif /* WXSTATICBITMAP_HPP_ */
