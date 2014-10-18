/*
 * wxToggleButton.cpp
 *
 *  Created on: 10.12.2013
 *      Author: hanmac
 */

#include "wxToggleButton.hpp"

VALUE rb_cWXToggleButton;
#if wxUSE_TOGGLEBTN
#define _self unwrap<wxToggleButton*>(self)

namespace RubyWX {
namespace ToggleButton {

APP_PROTECT(wxToggleButton)

macro_attr(Value,bool)

/*
 * call-seq:
 *   ToggleButton.new(parent, name, [options])
 *   ToggleButton.new(parent, [options])
 *
 * creates a new ToggleButton widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * value true/false
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString label(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,label);
		}
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		VALUE temp;
		set_option(id,Value,bool);
	}

	return self;
}

}
}

#endif

/* Document-attr: value
 * the value of the ToggleButton. bool
 */
DLL_LOCAL void Init_WXToggleButton(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_define_attr(rb_cWXToggleButton,"value",1,1);
#endif

#if wxUSE_TOGGLEBTN
	using namespace RubyWX::ToggleButton;
	rb_cWXToggleButton = rb_define_class_under(rb_mWX,"ToggleButton",rb_cWXAnyButton);
	rb_define_alloc_func(rb_cWXToggleButton,_alloc);

	rb_define_method(rb_cWXToggleButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXToggleButton,"value",_getValue,_setValue);

	registerInfo<wxToggleButton>(rb_cWXToggleButton);

	registerEventType<wxCommandEvent>("toogle_button",wxEVT_TOGGLEBUTTON);

#endif
}
