/*
 * wxFileDialog.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXFILEDIALOG_HPP_
#define WXFILEDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXFileDialog;

void Init_WXFileDialog(VALUE rb_mWX);
#if wxUSE_FILEDLG
#include <wx/filedlg.h>
#endif
#endif /* WXFILEDIALOG_HPP_ */
