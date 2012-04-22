/*
 * wxColorDialog.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXCOLORDIALOG_HPP_
#define WXCOLORDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXColorDialog;

void Init_WXColorDialog(VALUE rb_mWX);
#if wxUSE_COLOURDLG
#include <wx/colordlg.h>

template <>
inline VALUE wrap< wxColourDialog >(wxColourDialog* window)
{
	return wrap(window,rb_cWXColorDialog);
}


template <>
inline wxColourDialog* wrap< wxColourDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxColourDialog>(vwindow, rb_cWXColorDialog);
}
#endif
#endif /* WXCOLORDIALOG_HPP_ */
