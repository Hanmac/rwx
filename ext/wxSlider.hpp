/*
 * wxSlider.hpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#ifndef WXSLIDER_HPP_
#define WXSLIDER_HPP_

#include "main.hpp"

extern VALUE rb_cWXSlider;

void Init_WXSlider(VALUE rb_mWX);
#if wxUSE_SLIDER
#include <wx/slider.h>
template <>
inline VALUE wrap< wxSlider >(wxSlider* window)
{
	return wrap(window,rb_cWXSlider);
}


template <>
inline wxSlider* wrap< wxSlider* >(const VALUE &vwindow)
{
	return unwrapPtr<wxSlider>(vwindow, rb_cWXSlider);
}

#endif


#endif /* WXSLIDER_HPP_ */
