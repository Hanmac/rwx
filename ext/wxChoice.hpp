/*
 * wxChoice.hpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */

#ifndef WXCHOICE_HPP_
#define WXCHOICE_HPP_

#include "main.hpp"

extern VALUE rb_cWXChoice;

void Init_WXChoice(VALUE rb_mWX);

#if wxUSE_CHOICE
#include <wx/choice.h>

template <>
inline VALUE wrap< wxChoice >(wxChoice* window)
{
	return wrap(window,rb_cWXChoice);
}


template <>
inline wxChoice* wrap< wxChoice* >(const VALUE &vwindow)
{
	return unwrapPtr<wxChoice>(vwindow, rb_cWXChoice);
}

#endif


#endif /* WXCHOICE_HPP_ */
