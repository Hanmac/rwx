/*
 * wxFontPickerWidget.cpp
 *
 *  Created on: 24.05.2014
 *      Author: hanmac
 */

#include "wxFontPickerWidget.hpp"
#include "wxButton.hpp"
#include "wxFontPicker.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"

VALUE rb_cWXFontPickerWidget;
#if wxUSE_FONTPICKERCTRL
#define _self unwrap<wxFontPickerWidget*>(self)

namespace RubyWX {
namespace FontPickerWidget {

APP_PROTECT(wxFontPickerWidget)

macro_attr(Font,wxFont)

#ifdef HAVE_WXFONTPICKERCTRL_GETSELECTEDCOLOUR
macro_attr(SelectedColour,wxColor)
#endif

/*
 * call-seq:
 *   FontPickerWidget.new(parent, name, [options])
 *   FontPickerWidget.new(parent, [options])
 *
 * creates a new FontPickerWidget widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		int style(wxFONTBTN_DEFAULT_STYLE);

		wxFont font(wxNullFont);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"font",font);

			set_hash_flag_option(hash,"fontdesc_as_label",wxFNTP_FONTDESC_AS_LABEL,style);
			set_hash_flag_option(hash,"font_for_label",wxFNTP_USEFONT_FOR_LABEL,style);

		}

		_self->Create(
			unwrap<wxWindow*>(parent),id,font,
			wxDefaultPosition,wxDefaultSize,style
		);

	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

/* Document-attr: font
 * the font of the FontPickerWidget. WX::Font
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


DLL_LOCAL void Init_WXFontPickerWidget(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);

	rb_define_attr(rb_cWXFontPickerWidget,"font",1,1);
	rb_define_attr(rb_cWXFontPickerWidget,"selected_color",1,1);

#endif

#if wxUSE_FONTPICKERCTRL
	using namespace RubyWX::FontPickerWidget;
	rb_cWXFontPickerWidget = rb_define_class_under(rb_mWX,"FontPickerWidget",rb_cWXButton);
	rb_define_alloc_func(rb_cWXFontPickerWidget,_alloc);

	rb_define_method(rb_cWXFontPickerWidget,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontPickerWidget,"font",_getFont,_setFont);
#ifdef HAVE_WXFONTPICKERCTRL_GETSELECTEDCOLOUR
	rb_define_attr_method(rb_cWXFontPickerWidget,"selected_color",_getSelectedColour,_setSelectedColour);
#endif

	rb_define_const(rb_cWXFontPickerWidget,"DEFAULT_STYLE",INT2NUM(wxFNTP_DEFAULT_STYLE));
	rb_define_const(rb_cWXFontPickerWidget,"FONTDESC_AS_LABEL",INT2NUM(wxFNTP_FONTDESC_AS_LABEL));

	rb_define_const(rb_cWXFontPickerWidget,"DEFAULT_STYLE",INT2NUM(wxFONTBTN_DEFAULT_STYLE));

	registerInfo<wxFontPickerWidget>(rb_cWXFontPickerWidget);
#endif
}
