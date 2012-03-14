/*
 * wxDirDialog.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXDIRDIALOG_HPP_
#define WXDIRDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXDirDialog;

void Init_WXDirDialog(VALUE rb_mWX);
#if wxUSE_DIRDLG
#include <wx/dirdlg.h>

template <>
inline VALUE wrap< wxDirDialog >(wxDirDialog* window)
{
	return getEvtObj(window,rb_cWXDirDialog);
}


template <>
inline wxDirDialog* wrap< wxDirDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxDirDialog>(vwindow, rb_cWXDirDialog);
}
#endif

#endif /* WXDIRDIALOG_HPP_ */
