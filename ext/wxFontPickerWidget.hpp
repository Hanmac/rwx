/*
 * wxFontPickerWidget.hpp
 *
 *  Created on: 24.05.2014
 *      Author: hanmac
 */

#ifndef WXFONTPICKERWIDGET_HPP_
#define WXFONTPICKERWIDGET_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXFontPickerWidget;

void Init_WXFontPickerWidget(VALUE rb_mWX);
#if wxUSE_FONTPICKERCTRL
#include <wx/fontpicker.h>
#endif

#endif /* WXFONTPICKERWIDGET_HPP_ */
