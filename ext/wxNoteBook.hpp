/*
 * wxNotebook.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXNOTEBOOK_HPP_
#define WXNOTEBOOK_HPP_

#include "main.hpp"

extern VALUE rb_cWXNotebook;

void Init_WXNoteBookCtrl(VALUE rb_mWX);

#if wxUSE_NOTEBOOK
#include <wx/notebook.h>
template <>
inline VALUE wrap< wxNotebook >(wxNotebook* window)
{
	return wrap(window,rb_cWXNotebook);
}

template <>
inline wxNotebook* wrap< wxNotebook* >(const VALUE &vwindow)
{
	return unwrapPtr<wxNotebook>(vwindow, rb_cWXNotebook);
}
#endif

#endif /* WXNOTEBOOK_HPP_ */
