/*
 * wxControl.hpp
 *
 *  Created on: 08.02.2012
 *      Author: hanmac
 */

#ifndef WXCONTROL_HPP_
#define WXCONTROL_HPP_

#include "wxAnyButton.hpp"

#include "wxToolBar.hpp"


#include "wxStatusBar.hpp"

#include "wxGauge.hpp"

#include "wxRadioButton.hpp"

#include "wxStaticBitmap.hpp"

#include "wxTimePicker.hpp"
#include "wxFontPicker.hpp"
#include "wxColorPicker.hpp"
#include "wxFilePicker.hpp"
#include "wxDirPicker.hpp"

#include "wxTreeCtrl.hpp"

#include "wxHyperLink.hpp"

extern VALUE rb_cWXControl;
void Init_WXControl(VALUE rb_mWX);
#if wxUSE_CONTROLS
template <>
inline VALUE wrap< wxControl >(wxControl* window)
{
	if(window==NULL)
		return Qnil;
#ifdef wxHAS_ANY_BUTTON
	if(wxAnyButton *anybutton = dynamic_cast<wxAnyButton*>(window))
		return wrap(anybutton);
#endif
#if wxUSE_TOOLBAR
	if(wxToolBar *toolbar = dynamic_cast<wxToolBar*>(window))
		return wrap(toolbar);
#endif
#if wxUSE_STATUSBAR
	if(wxStatusBar *statusbar = dynamic_cast<wxStatusBar*>(window))
		return wrap(statusbar);
#endif
#if wxUSE_GAUGE
	if(wxGauge *gauge = dynamic_cast<wxGauge*>(window))
		return wrap(gauge);
#endif
#if wxUSE_TIMEPICKCTRL
	if(wxTimePickerCtrl *timepicker = dynamic_cast<wxTimePickerCtrl*>(window))
		return wrap(timepicker);
#endif
#if wxUSE_FONTPICKERCTRL
	if(wxFontPickerCtrl *fontpicker = dynamic_cast<wxFontPickerCtrl*>(window))
		return wrap(fontpicker);
#endif
#if wxUSE_COLOURPICKERCTRL
	if(wxColourPickerCtrl *colorpicker = dynamic_cast<wxColourPickerCtrl*>(window))
		return wrap(colorpicker);
#endif
#if wxUSE_FILEPICKERCTRL
	if(wxFilePickerCtrl *filepicker = dynamic_cast<wxFilePickerCtrl*>(window))
		return wrap(filepicker);
#endif
#if wxUSE_DIRPICKERCTRL
	if(wxDirPickerCtrl *dirpicker = dynamic_cast<wxDirPickerCtrl*>(window))
		return wrap(dirpicker);
#endif
#if wxUSE_RADIOBTN
	if(wxRadioButton *radiobtn = dynamic_cast<wxRadioButton*>(window))
		return wrap(radiobtn);
#endif
#if wxUSE_HYPERLINKCTRL
	if(wxHyperlinkCtrl *hyperlink = dynamic_cast<wxHyperlinkCtrl*>(window))
		return wrap(hyperlink);
#endif

	return getEvtObj(window,rb_cWXControl);
}


template <>
inline wxControl* wrap< wxControl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxControl>(vwindow, rb_cWXControl);
}
#endif

#endif /* WXCONTROL_HPP_ */
