/*
 * wxProgressDialog.hpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#ifndef WXPROGRESSDIALOG_HPP_
#define WXPROGRESSDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXProgressDialog;

void Init_WXProgressDialog(VALUE rb_mWX);
#if wxUSE_PROGRESSDLG
#include <wx/progdlg.h>

class RubyProgressDialog : public wxProgressDialog
{
public:
	RubyProgressDialog() : wxProgressDialog(wxEmptyString,wxEmptyString){}

#if wxUSE_TIMER
	wxTimer* mTimer;
#endif
};


template <>
inline VALUE wrap< wxProgressDialog >(wxProgressDialog* window)
{
	return wrap(window,rb_cWXProgressDialog);
}


template <>
inline wxProgressDialog* wrap< wxProgressDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxProgressDialog>(vwindow, rb_cWXProgressDialog);
}


template <>
inline RubyProgressDialog* wrap< RubyProgressDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<RubyProgressDialog>(vwindow, rb_cWXProgressDialog);
}

#endif

#endif /* WXPROGRESSDIALOG_HPP_ */
