/*
 * wxAboutDlg.hpp
 *
 *  Created on: 29.02.2012
 *      Author: hanmac
 */

#ifndef WXABOUTDLG_HPP_
#define WXABOUTDLG_HPP_

#include "wxBitmap.hpp"

#if wxUSE_ABOUTDLG
#include <wx/aboutdlg.h>
#include <wx/generic/aboutdlgg.h>
#endif
void Init_WXAboutDlg(VALUE rb_mWX);

#endif /* WXABOUTDLG_HPP_ */
