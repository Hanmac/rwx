/*
 * wxAuiBook.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXAUIBOOK_HPP_
#define WXAUIBOOK_HPP_

#include "main.hpp"

extern VALUE rb_cWXAuiNotebook;

void Init_WXAuiNoteBookCtrl(VALUE rb_mWX);

#if wxUSE_AUI
#include <wx/aui/auibook.h>
template <>
inline VALUE wrap< wxAuiNotebook >(wxAuiNotebook* window)
{
	return wrap(window,rb_cWXAuiNotebook);
}

template <>
inline wxAuiNotebook* wrap< wxAuiNotebook* >(const VALUE &vwindow)
{
	return unwrapPtr<wxAuiNotebook>(vwindow, rb_cWXAuiNotebook);
}
#endif

#endif /* WXAUIBOOK_HPP_ */
