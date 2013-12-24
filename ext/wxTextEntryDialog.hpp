/*
 * wxTextEntryDialog.hpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */

#ifndef WXTEXTENTRYDIALOG_HPP_
#define WXTEXTENTRYDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXTextEntryDialog;

void Init_WXTextEntryDialog(VALUE rb_mWX);
#if wxUSE_TEXTDLG
#include <wx/textdlg.h>
#endif

#endif /* WXTEXTENTRYDIALOG_HPP_ */
