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

		}

		_self->Create(unwrap<wxWindow*>(parent),id,font,wxDefaultPosition,wxDefaultSize,style);
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
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);

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




