/*
 * wxNumberEntryDialog.hpp
 *
 *  Created on: 05.03.2012
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
