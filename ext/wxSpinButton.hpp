/*
 * wxSpinButton.hpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#ifndef WXSPINBUTTON_HPP_
#define WXSPINBUTTON_HPP_

#include "main.hpp"

extern VALUE rb_cWXSpinButton;

void Init_WXSpinButton(VALUE rb_mWX);
#if wxUSE_SPINBTN
#include <wx/spinbutt.h>
template <>
inline VALUE wrap< wxSpinButton >(wxSpinButton* window)
{
	return wrap(window,rb_cWXSpinButton);
}


template <>
inline wxSpinButton* wrap< wxSpinButton* >(const VALUE &vwindow)
{
	return unwrapPtr<wxSpinButton>(vwindow, rb_cWXSpinButton);
}

#endif

#endif /* WXSPINBUTTON_HPP_ */
