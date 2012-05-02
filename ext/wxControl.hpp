/*
 * wxControl.hpp
 *
 *  Created on: 08.02.2012
 *      Author: hanmac
 */

#ifndef WXCONTROL_HPP_
#define WXCONTROL_HPP_

#include "wxAnyButton.hpp"

#include "wxToolBarBase.hpp"


#include "wxStatusBar.hpp"

#include "wxGauge.hpp"


#include "wxCheckBox.hpp"
#include "wxRadioButton.hpp"

#include "wxStaticLine.hpp"
#include "wxStaticBitmap.hpp"
#include "wxStaticText.hpp"
#include "wxStaticBox.hpp"

#include "wxSlider.hpp"

#include "wxSpinButton.hpp"
#include "wxSpinCtrl.hpp"

#include "wxTextCtrl.hpp"

#include "wxTimePicker.hpp"
#include "wxFontPicker.hpp"
#include "wxColorPicker.hpp"
#include "wxFilePicker.hpp"
#include "wxDirPicker.hpp"

#include "wxTreeCtrl.hpp"

#include "wxFileCtrl.hpp"

#include "wxDataView.hpp"

#include "wxHyperLink.hpp"

#include "wxBookCtrl.hpp"

#include "wxSTC.hpp"

extern VALUE rb_cWXControl;
DLL_LOCAL void Init_WXControl(VALUE rb_mWX);
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
	if(wxToolBarBase *toolbar = dynamic_cast<wxToolBarBase*>(window))
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

#if wxUSE_STATBMP
	if(wxStaticBitmap *bitmap = dynamic_cast<wxStaticBitmap*>(window))
		return wrap(bitmap);
#endif
#if wxUSE_STATLINE
	if(wxStaticLine *line = dynamic_cast<wxStaticLine*>(window))
		return wrap(line);
#endif
#if wxUSE_STATTEXT
	if(wxStaticText *text = dynamic_cast<wxStaticText*>(window))
		return wrap(text);
#endif
#if wxUSE_STATBOX
	if(wxStaticBox *box = dynamic_cast<wxStaticBox*>(window))
		return wrap(box);
#endif

#if wxUSE_SLIDER
	if(wxSlider *slider = dynamic_cast<wxSlider*>(window))
		return wrap(slider);
#endif

#if wxUSE_SPINCTRL
	if(wxSpinCtrl *spinctrl = dynamic_cast<wxSpinCtrl*>(window))
		return wrap(spinctrl);
#endif

#if wxUSE_FILECTRL
	if(wxFileCtrl *filectrl = dynamic_cast<wxFileCtrl*>(window))
		return wrap(filectrl);
#endif

#if wxUSE_TEXTCTRL
	if(wxTextCtrl *textctrl = dynamic_cast<wxTextCtrl*>(window))
		return wrap(textctrl);
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
#if wxUSE_CHECKBOX
	if(wxCheckBox *check = dynamic_cast<wxCheckBox*>(window))
		return wrap(check);
#endif


#if wxUSE_HYPERLINKCTRL
	if(wxHyperlinkCtrl *hyperlink = dynamic_cast<wxHyperlinkCtrl*>(window))
		return wrap(hyperlink);
#endif

#if wxUSE_BOOKCTRL
	if(wxBookCtrlBase *bookctrl = dynamic_cast<wxBookCtrlBase*>(window))
		return wrap(bookctrl);
#endif

#if wxUSE_STC
	if(wxStyledTextCtrl *stc = dynamic_cast<wxStyledTextCtrl*>(window))
		return wrap(stc);
#endif

	return wrap(window,rb_cWXControl);
}


template <>
inline wxControl* wrap< wxControl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxControl>(vwindow, rb_cWXControl);
}
#endif

#endif /* WXCONTROL_HPP_ */
