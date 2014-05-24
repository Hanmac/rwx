/*
 * wxColorPickerWidget.hpp
 *
 *  Created on: 24.05.2014
 *      Author: hanmac
 */

#ifndef WXCOLORPICKERWIDGET_HPP_
#define WXCOLORPICKERWIDGET_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXColorPickerWidget;

void Init_WXColorPickerWidget(VALUE rb_mWX);
#if wxUSE_COLOURPICKERCTRL
#include <wx/clrpicker.h>
#endif

#endif /* WXCOLORPICKERWIDGET_HPP_ */
