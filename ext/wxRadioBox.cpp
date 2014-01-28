/*
 * wxRadioBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxRadioBox.hpp"

VALUE rb_cWXRadioBox;

#if wxUSE_RADIOBOX

namespace RubyWX {
namespace RadioBox {
#define _self unwrap<wxRadioBox*>(self)

APP_PROTECT(wxRadioBox)

macro_attr_selection(Selection,GetCount)

macro_attr(StringSelection,wxString)

/*
 * call-seq:
 *   RadioBox.new(parent, name, [options])
 *   RadioBox.new(parent, [options])
 *
 * creates a new RadioBox widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString label;
		wxArrayString choices;
		int style(wxRA_SPECIFY_COLS);
		int selection(-1);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"selection",selection);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,label,
			wxDefaultPosition,wxDefaultSize,
			choices,0,style
		);
		if(check_index(selection,_self->GetCount()))
			_self->SetSelection(selection);

	}
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _getItemString(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(idx,_self->GetCount()))
		return wrap(_self->GetString(cidx));
	return Qnil;
}

DLL_LOCAL VALUE _setItemString(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(idx,_self->GetCount()))
		_self->SetString(cidx,unwrap<wxString>(val));

	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRadioBox(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXRadioBox,"selection",1,1);
	rb_define_attr(rb_cWXRadioBox,"string_selection",1,1);

#endif
#if wxUSE_RADIOBOX
	using namespace RubyWX::RadioBox;
	rb_cWXRadioBox = rb_define_class_under(rb_mWX,"RadioBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioBox,_alloc);

	rb_define_method(rb_cWXRadioBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXRadioBox,"get_item_string",RUBY_METHOD_FUNC(_getItemString),1);
	rb_define_method(rb_cWXRadioBox,"set_item_string",RUBY_METHOD_FUNC(_setItemString),2);

	rb_define_attr_method(rb_cWXRadioBox,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_cWXRadioBox,"string_selection",_getStringSelection,_setStringSelection);

	registerEventType("radiobox", wxEVT_RADIOBOX,rb_cWXCommandEvent);

	registerInfo<wxRadioBox>(rb_cWXRadioBox);
#endif

}


