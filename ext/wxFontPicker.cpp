/*
 * wxFontPicker.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */


#include "wxFontPicker.hpp"
#include "wxPickerBase.hpp"
#include "wxFont.hpp"

VALUE rb_cWXFontPicker,rb_cWXFontPickerEvent;

#if wxUSE_FONTPICKERCTRL
#define _self unwrap<wxFontPickerCtrl*>(self)

namespace RubyWX {
namespace FontPicker {

macro_attr(SelectedFont,wxFont)

APP_PROTECT(wxFontPickerCtrl)

/*
 * call-seq:
 *   FontPicker.new(parent, [options])
 *
 * creates a new FontPicker widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * *selected_font WX::Font default font
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		set_option(selected_font,SelectedFont,wxFont)
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
DLL_LOCAL void Init_WXFontPicker(VALUE rb_mWX)
{
#if 0
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);

	rb_define_attr(rb_cWXFontPicker,"selected_font",1,1);
	rb_define_attr(rb_cWXFontPickerEvent,"font",1,1);
#endif

#if wxUSE_FONTPICKERCTRL
	using namespace RubyWX::FontPicker;
	rb_cWXFontPicker = rb_define_class_under(rb_mWX,"FontPicker",rb_cWXPickerBase);
	rb_define_alloc_func(rb_cWXFontPicker,_alloc);

	rb_define_method(rb_cWXFontPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontPicker,"selected_font",_getSelectedFont,_setSelectedFont);

	rb_cWXFontPickerEvent = rb_define_class_under(rb_cWXEvent,"FontPicker",rb_cWXEvent);
	registerEventType("fontpicker_changed",wxEVT_FONTPICKER_CHANGED,rb_cWXFontPickerEvent);
	rb_define_attr_method(rb_cWXFontPickerEvent,"font",Event::_getFont,Event::_setFont);

	registerInfo<wxFontPickerCtrl>(rb_cWXFontPicker);
#endif

}




