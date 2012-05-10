/*
 * wxProgressDialog.hpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#ifndef WXPROGRESSDIALOG_HPP_
#define WXPROGRESSDIALOG_HPP_

#include "wxDialog.hpp"

extern VALUE rb_cWXProgressDialog;

DLL_LOCAL void Init_WXProgressDialog(VALUE rb_mWX);
#if wxUSE_PROGRESSDLG
#include <wx/progdlg.h>

class RubyProgressDialog : public wxProgressDialog
{
public:
	RubyProgressDialog() : wxProgressDialog(wxEmptyString,wxEmptyString){}

#if wxUSE_TIMER
	wxTimer* mTimer;
	void onTimer(wxTimerEvent &evt);
#endif
};

#endif

#endif /* WXPROGRESSDIALOG_HPP_ */
