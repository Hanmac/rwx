/*
 * wxColorPickerWidget.cpp
 *
 *  Created on: 24.05.2014
 *      Author: hanmac
 */

#include "wxColorPickerWidget.hpp"
#include "wxButton.hpp"
#include "wxColorPicker.hpp"
#include "wxColor.hpp"

VALUE rb_cWXColorPickerWidget;
#if wxUSE_COLOURPICKERCTRL
#define _self unwrap<wxColourPickerWidget*>(self)

namespace RubyWX {
namespace ColorPickerWidget {

APP_PROTECT(wxColourPickerWidget)

macro_attr(Colour,wxColour)

/*
 * call-seq:
 *   ColorPickerWidget.new(parent, name, [options])
 *   ColorPickerWidget.new(parent, [options])
 *
 * creates a new ColorPickerWidget widget.
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
		int style(wxCLRBTN_DEFAULT_STYLE);

		wxColor color(*wxBLACK);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"color",color);

			set_hash_flag_option(hash,"show_label",wxCLRBTN_SHOW_LABEL,style);

		}

		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,color,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

/* Document-attr: color
 * the color of the ColorPickerWidget. WX::Color
 */

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

DLL_LOCAL void Init_WXColorPickerWidget(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);
#endif

#if wxUSE_COLOURPICKERCTRL
	using namespace RubyWX::ColorPickerWidget;
	rb_cWXColorPickerWidget = rb_define_class_under(rb_mWX,"ColorPickerWidget",rb_cWXButton);
	rb_define_alloc_func(rb_cWXColorPickerWidget,_alloc);

#if 0
	rb_define_attr(rb_cWXColorPickerWidget,"color",1,1);
#endif

	rb_define_method(rb_cWXColorPickerWidget,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXColorPickerWidget,"color",_getColour,_setColour);

	rb_define_const(rb_cWXColorPickerWidget,"DEFAULT_STYLE",INT2NUM(wxCLRBTN_DEFAULT_STYLE));
	rb_define_const(rb_cWXColorPickerWidget,"SHOW_LABEL",INT2NUM(wxCLRBTN_SHOW_LABEL));

	registerInfo<wxColourPickerWidget>(rb_cWXColorPickerWidget);
#endif
}
