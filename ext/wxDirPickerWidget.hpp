/*
 * wxDirPickerWidget.hpp
 *
 *  Created on: 05.05.2014
 *      Author: hanmac
 */

#ifndef WXDIRPICKERWIDGET_HPP_
#define WXDIRPICKERWIDGET_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXDirPickerWidget;

void Init_WXDirPickerWidget(VALUE rb_mWX);
#if wxUSE_DIRPICKERCTRL
#include <wx/filepicker.h>
#endif

#endif /* WXDIRPICKERWIDGET_HPP_ */
