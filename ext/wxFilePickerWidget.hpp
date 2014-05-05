/*
 * wxFilePickerWidget.hpp
 *
 *  Created on: 05.05.2014
 *      Author: hanmac
 */

#ifndef WXFILEPICKERWIDGET_HPP_
#define WXFILEPICKERWIDGET_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXFilePickerWidget;

void Init_WXFilePickerWidget(VALUE rb_mWX);
#if wxUSE_FILEPICKERCTRL
#include <wx/filepicker.h>
#endif

#endif /* WXFILEPICKERWIDGET_HPP_ */
