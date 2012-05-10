/*
 * wxDirDialog.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXDIRDIALOG_HPP_
#define WXDIRDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXDirDialog;

void Init_WXDirDialog(VALUE rb_mWX);
#if wxUSE_DIRDLG
#include <wx/dirdlg.h>
#endif

#endif /* WXDIRDIALOG_HPP_ */
