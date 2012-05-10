/*
 * wxColorDialog.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXCOLORDIALOG_HPP_
#define WXCOLORDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXColorDialog;

void Init_WXColorDialog(VALUE rb_mWX);
#if wxUSE_COLOURDLG
#include <wx/colordlg.h>
#endif
#endif /* WXCOLORDIALOG_HPP_ */
