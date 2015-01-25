/*
 * wxMessageDialog.hpp
 *
 *  Created on: 25.11.2012
 *      Author: hanmac
 */

#ifndef WXRICHMESSAGEDIALOG_HPP_
#define WXRICHMESSAGEDIALOG_HPP_

#include "wxMessageDialogBase.hpp"

extern VALUE rb_cWXRichMessageDialog;

void Init_WXRichMessageDialog(VALUE rb_mWX);

#if wxUSE_RICHMSGDLG
#include <wx/richmsgdlg.h>
#endif

#endif /* WXRICHMESSAGEDIALOG_HPP_ */
