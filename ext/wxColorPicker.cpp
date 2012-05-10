/*
 * wxColorPicker.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */
#include "wxColorPicker.hpp"
#include "wxColor.hpp"

VALUE rb_cWXColorPicker,rb_cWXColorPickerEvent;

#if wxUSE_COLOURPICKERCTRL

namespace RubyWX {
namespace ColorPicker {
#define _self wrap<wxColourPickerCtrl*>(self)
macro_attr(Colour,wxColour)

APP_PROTECT(wxColourPickerCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

namespace Event {
#undef _self
#define _self unwrapPtr<wxColourPickerEvent>(self,rb_cWXColorPickerEvent)
macro_attr(Colour,wxColour)
}

}
}

#endif
void Init_WXColorPicker(VALUE rb_mWX)
{
#if wxUSE_COLOURPICKERCTRL
	using namespace RubyWX::ColorPicker;
	rb_cWXColorPicker = rb_define_class_under(rb_mWX,"ColorPicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXColorPicker,_alloc);

	rb_define_method(rb_cWXColorPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXColorPicker,"color",_getColour,_setColour);

	rb_cWXColorPickerEvent = rb_define_class_under(rb_cWXEvent,"ColorPicker",rb_cWXEvent);
	registerEventType("colorpicker_changed",wxEVT_COMMAND_COLOURPICKER_CHANGED,rb_cWXColorPickerEvent);

	rb_define_attr_method(rb_cWXColorPickerEvent,"color",
			Event::_getColour,Event::_setColour);

	registerType<wxColourPickerCtrl>(rb_cWXColorPicker);
#endif

}


