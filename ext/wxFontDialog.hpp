/*
 * wxFontDialog.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXFONTDIALOG_HPP_
#define WXFONTDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXFontDialog;

void Init_WXFontDialog(VALUE rb_mWX);

#if wxUSE_FONTDLG
#include <wx/fontdlg.h>
#endif

#endif /* WXFONTDIALOG_HPP_ */
