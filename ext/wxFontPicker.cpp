/*
 * wxFontPicker.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */



#include "wxWindow.hpp"
#include "wxFont.hpp"
#include "wxEvtHandler.hpp"

VALUE rb_cWXFontPicker,rb_cWXFontPickerEvent;

#if wxUSE_FONTPICKERCTRL
#define _self wrap<wxFontPickerCtrl*>(self)

namespace RubyWX {
namespace FontPicker {

macro_attr(SelectedFont,wxFont)


VALUE _alloc(VALUE self)
{
	return getEvtObj(new wxFontPickerCtrl(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	rb_call_super(argc,argv);
	return self;
}

namespace Event
{
#undef _self
#define _self unwrapPtr<wxFontPickerEvent>(self,rb_cWXFontPickerEvent)
macro_attr(Font,wxFont)
}

}
}
#endif
void Init_WXFontPicker(VALUE rb_mWX)
{
#if wxUSE_FONTPICKERCTRL
	using namespace RubyWX::FontPicker;
	rb_cWXFontPicker = rb_define_class_under(rb_mWX,"FontPicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXFontPicker,_alloc);

	rb_define_method(rb_cWXFontPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontPicker,"selected_font",_getSelectedFont,_setSelectedFont);

	rb_cWXFontPickerEvent = rb_define_class_under(rb_cWXEvent,"FontPicker",rb_cWXEvent);
	registerEventType("fontpicker_changed",wxEVT_COMMAND_FONTPICKER_CHANGED,rb_cWXFontPickerEvent);
	rb_define_attr_method(rb_cWXFontPickerEvent,"font",Event::_getFont,Event::_setFont);
#endif

}




