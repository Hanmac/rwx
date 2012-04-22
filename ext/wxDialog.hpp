/*
 * wxDialog.hpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */

#ifndef WXDIALOG_HPP_
#define WXDIALOG_HPP_

#include "main.hpp"

extern VALUE rb_cWXDialog;

void Init_WXDialog(VALUE rb_mWX);

#include "wxColorDialog.hpp"
#include "wxFontDialog.hpp"

#include "wxFileDialog.hpp"
#include "wxDirDialog.hpp"
#include "wxProgressDialog.hpp"

#include "wxWizard.hpp"

template <>
inline VALUE wrap< wxDialog >(wxDialog* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXDialog);
}



template <>
inline wxDialog* wrap< wxDialog* >(const VALUE &vwindow)
{
	return unwrapPtr<wxDialog>(vwindow, rb_cWXDialog);
}


#endif /* WXDIALOG_HPP_ */
