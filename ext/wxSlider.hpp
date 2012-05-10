/*
 * wxSlider.hpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#ifndef WXSLIDER_HPP_
#define WXSLIDER_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXSlider;

void Init_WXSlider(VALUE rb_mWX);
#if wxUSE_SLIDER
#include <wx/slider.h>
#endif


#endif /* WXSLIDER_HPP_ */
