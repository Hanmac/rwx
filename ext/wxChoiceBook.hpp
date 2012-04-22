/*
 * wxChoiceBook.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXCHOICEBOOK_HPP_
#define WXCHOICEBOOK_HPP_

#include "main.hpp"

extern VALUE rb_cWXChoicebook;

void Init_WXChoiceBookCtrl(VALUE rb_mWX);

#if wxUSE_CHOICEBOOK
#include <wx/choicebk.h>
template <>
inline VALUE wrap< wxChoicebook >(wxChoicebook* window)
{
	return wrap(window,rb_cWXChoicebook);
}

template <>
inline wxChoicebook* wrap< wxChoicebook* >(const VALUE &vwindow)
{
	return unwrapPtr<wxChoicebook>(vwindow, rb_cWXChoicebook);
}
#endif


#endif /* WXCHOICEBOOK_HPP_ */
