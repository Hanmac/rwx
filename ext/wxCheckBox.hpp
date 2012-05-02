/*
 * wxCheckBox.hpp
 *
 *  Created on: 01.05.2012
 *      Author: hanmac
 */

#ifndef WXCHECKBOX_HPP_
#define WXCHECKBOX_HPP_

#include "main.hpp"

extern VALUE rb_cWXCheckBox;

void Init_WXCheckBox(VALUE rb_mWX);
#if wxUSE_CHECKBOX

template <>
inline VALUE wrap< wxCheckBox >(wxCheckBox* window)
{
	return wrap(window,rb_cWXCheckBox);
}


template <>
inline wxCheckBox* wrap< wxCheckBox* >(const VALUE &vwindow)
{
	return unwrapPtr<wxCheckBox>(vwindow, rb_cWXCheckBox);
}

#endif



#endif /* WXCHECKBOX_HPP_ */
