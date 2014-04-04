/*
 * wxComboBox.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"
#include "wxTextEntry.hpp"

VALUE rb_cWXComboBox;

#if wxUSE_COMBOBOX
#define _self unwrap<wxComboBox*>(self)

namespace RubyWX {
namespace ComboBox {

APP_PROTECT(wxComboBox)

//need to define it again to prevent Shadowing
singlefunc(Clear)

singlereturn(IsListEmpty)
singlereturn(IsTextEmpty)

/*
 * call-seq:
 *   ComboBox.new(parent, name, [options])
 *   ComboBox.new(parent, [options])
 *
 * creates a new ComboBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * items [String]
 *   * select Integer
 *   * value String
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created  && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString value;
		wxArrayString choices;
		int style(0);
		int selection(-1);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"selection",selection);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,value,
			wxDefaultPosition,wxDefaultSize,
			choices,style
		);
		
		if(selection != -1)
			_self->SetSelection(selection);

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		VALUE temp;
		set_option(items,,wxArrayString)
		set_option(select,Selection,int)
		set_option(value,Value,wxString)

	}

	return self;
}

}
}
#endif

DLL_LOCAL void Init_WXComboBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");
#endif
#if wxUSE_COMBOBOX
	using namespace RubyWX::ComboBox;
	rb_cWXComboBox = rb_define_class_under(rb_mWX,"ComboBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXComboBox,_alloc);

	rb_define_method(rb_cWXComboBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXComboBox,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_define_method(rb_cWXComboBox,"list_empty?",RUBY_METHOD_FUNC(_IsListEmpty),0);
	rb_define_method(rb_cWXComboBox,"text_empty?",RUBY_METHOD_FUNC(_IsTextEmpty),0);

	rb_include_module(rb_cWXComboBox,rb_mWXItemContainer);
	rb_include_module(rb_cWXComboBox,rb_mWXTextEntry);

	registerInfo<wxComboBox>(rb_cWXComboBox);
#endif

}

