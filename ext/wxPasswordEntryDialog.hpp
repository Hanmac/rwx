/*
 * wxPasswordEntryDialog.hpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */

#ifndef WXPASSWORDENTRYDIALOG_HPP_
#define WXPASSWORDENTRYDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXPasswordEntryDialog;

void Init_WXPasswordEntryDialog(VALUE rb_mWX);
#if wxUSE_TEXTDLG
#include <wx/textdlg.h>
#endif

#endif /* WXPASSWORDENTRYDIALOG_HPP_ */
