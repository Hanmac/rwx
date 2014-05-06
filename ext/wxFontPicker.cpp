/*
 * wxFontPicker.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */


#include "wxFontPicker.hpp"
#include "wxPickerBase.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"

VALUE rb_cWXFontPicker,rb_cWXFontPickerEvent;

#if wxUSE_FONTPICKERCTRL
#define _self unwrap<wxFontPickerCtrl*>(self)

namespace RubyWX {
namespace FontPicker {

macro_attr(SelectedFont,wxFont)

#if HAVE_WXFONTPICKERCTRL_GETSELECTEDCOLOUR
macro_attr(SelectedColour,wxColor)
#endif

APP_PROTECT(wxFontPickerCtrl)

/*
 * call-seq:
 *   FontPicker.new(parent, name, [options])
 *   FontPicker.new(parent, [options])
 *
 * creates a new FontPicker widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * selected_font WX::Font default font
 *   * selected_color WX::Color default color
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxFont font(wxNullFont);
		int style(wxFNTP_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"font",font);
			set_hash_option(hash,"style",style);

			PickerBase::set_style_flags(hash,style);

			set_hash_flag_option(hash,"fontdesc_as_label",wxFNTP_FONTDESC_AS_LABEL,style);
			set_hash_flag_option(hash,"font_for_label",wxFNTP_USEFONT_FOR_LABEL,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,font,wxDefaultPosition,wxDefaultSize,style);
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		set_option(selected_font,SelectedFont,wxFont)
		set_option(selected_color,SelectedColour,wxColor)
	}
	return self;
}

namespace Event
{
#undef _self
#define _self unwrap<wxFontPickerEvent*>(self)
macro_attr(Font,wxFont)
}

}
}
#endif


/* Document-attr: selected_font
 * the current font of the WX::FontPicker. WX::Font
 */
/* Document-attr: selected_color
 * the current color of the WX::FontPicker. WX::Color
 */

/* Document-attr: font
 * the font of the WX::Event::FontPicker. WX::Font
 */

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */
/* Document-const: FONTDESC_AS_LABEL
 *  Keeps the label of the button updated with the fontface name and the font size.
 *  E.g. choosing "Times New Roman bold, italic with size 10" from the fontdialog,
 *  will update the label (overwriting any previous label) with the "Times New Roman, 10" text.
 */
/* Document-const: USEFONT_FOR_LABEL
 *  Uses the currently selected font to draw the label of the button.
 */

DLL_LOCAL void Init_WXFontPicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);

	rb_define_attr(rb_cWXFontPicker,"selected_font",1,1);
	rb_define_attr(rb_cWXFontPicker,"selected_color",1,1);
	rb_define_attr(rb_cWXFontPickerEvent,"font",1,1);
#endif

#if wxUSE_FONTPICKERCTRL
	using namespace RubyWX::FontPicker;
	rb_cWXFontPicker = rb_define_class_under(rb_mWX,"FontPicker",rb_cWXPickerBase);
	rb_define_alloc_func(rb_cWXFontPicker,_alloc);

	rb_define_method(rb_cWXFontPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontPicker,"selected_font",_getSelectedFont,_setSelectedFont);

#if HAVE_WXFONTPICKERCTRL_GETSELECTEDCOLOUR
	rb_define_attr_method(rb_cWXFontPicker,"selected_color",_getSelectedColour,_setSelectedColour);
#endif

	rb_cWXFontPickerEvent = rb_define_class_under(rb_cWXEvent,"FontPicker",rb_cWXEvent);
	registerEventType("fontpicker_changed",wxEVT_FONTPICKER_CHANGED,rb_cWXFontPickerEvent);
	rb_define_attr_method(rb_cWXFontPickerEvent,"font",Event::_getFont,Event::_setFont);

	rb_define_const(rb_cWXFontPicker,"DEFAULT_STYLE",INT2NUM(wxFNTP_DEFAULT_STYLE));
	rb_define_const(rb_cWXFontPicker,"FONTDESC_AS_LABEL",INT2NUM(wxFNTP_FONTDESC_AS_LABEL));
	rb_define_const(rb_cWXFontPicker,"USEFONT_FOR_LABEL",INT2NUM(wxFNTP_USEFONT_FOR_LABEL));

	registerInfo<wxFontPickerCtrl>(rb_cWXFontPicker);
#endif

}




