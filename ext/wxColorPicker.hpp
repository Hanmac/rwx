/*
 * wxColorPicker.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXCOLORPICKER_HPP_
#define WXCOLORPICKER_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXColorPicker;

void Init_WXColorPicker(VALUE rb_mWX);
#if wxUSE_COLOURPICKERCTRL
#include <wx/clrpicker.h>
#endif

#endif /* WXCOLORPICKER_HPP_ */
