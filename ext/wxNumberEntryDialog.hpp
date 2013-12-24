/*
 * wxNumberEntryDialog.hpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */

#ifndef WXNUMBERENTRYDIALOG_HPP_
#define WXNUMBERENTRYDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXNumberEntryDialog;

void Init_WXNumberEntryDialog(VALUE rb_mWX);
#if wxUSE_NUMBERDLG
#include <wx/numdlg.h>
#endif

#endif /* WXNUMBERENTRYDIALOG_HPP_ */
