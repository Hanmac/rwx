/*
 * wxColorPicker.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */
#include "wxColorPicker.hpp"
#include "wxPickerBase.hpp"
#include "wxColor.hpp"

VALUE rb_cWXColorPicker,rb_cWXColorPickerEvent;

#if wxUSE_COLOURPICKERCTRL

namespace RubyWX {
namespace ColorPicker {
#define _self unwrap<wxColourPickerCtrl*>(self)
macro_attr(Colour,wxColour)

APP_PROTECT(wxColourPickerCtrl)

/*
 * call-seq:
 *   ColorPicker.new(parent, name, [options])
 *   ColorPicker.new(parent, [options])
 *
 * creates a new ColorPicker widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * color WX::Color default color
 *   * use_textctrl true/false
 *   * small true/false
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxColor color(*wxBLACK);
		int style(wxCLRP_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"color",color);
			set_hash_option(hash,"style",style);

			PickerBase::set_style_flags(hash,style);

			set_hash_flag_option(hash,"show_label",wxCLRP_SHOW_LABEL,style);

		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,color,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		set_option(color,Colour,wxColour)
	}

	return self;
}

namespace Event {
#undef _self
#define _self unwrapTypedPtr<wxColourPickerEvent>(self,rb_cWXColorPickerEvent)
macro_attr(Colour,wxColour)
}

}
}

#endif

/* Document-attr: color
 * the color of the ColorPicker. WX::Color
 */
/* Document-attr: WX::Event::ColorPicker#color
 * the color of the ColorPicker. WX::Color
 */
DLL_LOCAL void Init_WXColorPicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);
#endif

#if wxUSE_COLOURPICKERCTRL
	using namespace RubyWX::ColorPicker;
	rb_cWXColorPicker = rb_define_class_under(rb_mWX,"ColorPicker",rb_cWXPickerBase);
	rb_define_alloc_func(rb_cWXColorPicker,_alloc);
	rb_cWXColorPickerEvent = rb_define_class_under(rb_cWXEvent,"ColorPicker",rb_cWXEvent);

#if 0
	rb_define_attr(rb_cWXColorPicker,"color",1,1);
	rb_define_attr(rb_cWXColorPickerEvent,"color",1,1);
#endif

	rb_define_method(rb_cWXColorPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXColorPicker,"color",_getColour,_setColour);

	rb_define_const(rb_cWXColorPicker,"DEFAULT_STYLE",INT2NUM(wxCLRP_DEFAULT_STYLE));
	rb_define_const(rb_cWXColorPicker,"SHOW_LABEL",INT2NUM(wxCLRP_SHOW_LABEL));

	registerEventType("colorpicker_changed",wxEVT_COLOURPICKER_CHANGED,rb_cWXColorPickerEvent);

	rb_define_attr_method(rb_cWXColorPickerEvent,"color",
			Event::_getColour,Event::_setColour);

	registerInfo<wxColourPickerCtrl>(rb_cWXColorPicker);
#endif

}


