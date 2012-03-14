/*
 * wxFileDialog.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXFILEDIALOG_HPP_
#define WXFILEDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXFileDialog;

void Init_WXFileDialog(VALUE rb_mWX);
#if wxUSE_FILEDLG
#include <wx/filedlg.h>

template <>
inline VALUE wrap< wxFileDialog >(wxFileDialog* window)
{
	return getEvtObj(window,rb_cWXFileDialog);
}


template <>
inline wxFileDialog* wrap< wxFileDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFileDialog>(vwindow, rb_cWXFileDialog);
}
#endif
#endif /* WXFILEDIALOG_HPP_ */
