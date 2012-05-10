/*
 * wxFontPicker.hpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#ifndef WXFONTPICKER_HPP_
#define WXFONTPICKER_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXFontPicker;

void Init_WXFontPicker(VALUE rb_mWX);

#if wxUSE_FONTPICKERCTRL
#include <wx/fontpicker.h>
#endif

#endif /* WXFONTPICKER_HPP_ */
