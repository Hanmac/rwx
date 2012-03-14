/*
 * wxFontDialog.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXFONTDIALOG_HPP_
#define WXFONTDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXFontDialog;

void Init_WXFontDialog(VALUE rb_mWX);

#if wxUSE_FONTDLG
#include <wx/fontdlg.h>

template <>
inline VALUE wrap< wxFontDialog >(wxFontDialog* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXFontDialog);
}


template <>
inline wxFontDialog* wrap< wxFontDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFontDialog>(vwindow, rb_cWXFontDialog);
}

#endif

#endif /* WXFONTDIALOG_HPP_ */
