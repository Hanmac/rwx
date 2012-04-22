/*
 * wxToolBook.hpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#ifndef WXTOOLBOOK_HPP_
#define WXTOOLBOOK_HPP_


#include "main.hpp"

extern VALUE rb_cWXToolbook;

void Init_WXToolBookCtrl(VALUE rb_mWX);

#if wxUSE_NOTEBOOK
#include <wx/toolbook.h>
template <>
inline VALUE wrap< wxToolbook >(wxToolbook* window)
{
	return wrap(window,rb_cWXToolbook);
}

template <>
inline wxToolbook* wrap< wxToolbook* >(const VALUE &vwindow)
{
	return unwrapPtr<wxToolbook>(vwindow, rb_cWXToolbook);
}
#endif


#endif /* WXTOOLBOOK_HPP_ */
