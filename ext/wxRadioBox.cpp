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
		bool selset(false);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			selset = set_hash_option(hash,"selection",selection);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,label,
			wxDefaultPosition,wxDefaultSize,
			choices,0,style
		);
		if(selset && check_index(selection,_self->GetCount()))
			_self->SetSelection(selection);

	}
	rb_call_super(argc,argv);
	return self;
}

/*
 * call-seq:
 *   get_item_string(pos) -> String
 *
 * returns the String of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * String
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _getItemString(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		return wrap(_self->GetString(cidx));
	return Qnil;
}


/*
 * call-seq:
 *   set_item_string(pos,text) -> self
 *
 * sets the String of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * * text of the item. String
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _setItemString(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->SetString(cidx,unwrap<wxString>(val));

	return self;
}

/*
 * call-seq:
 *   get_item_shown(pos) -> true/false
 *
 * returns if the item at the given position is shown.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _getItemShown(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		return wrap(_self->IsItemShown(cidx));
	return Qnil;
}


/*
 * call-seq:
 *   set_item_shown(pos, val) -> self
 *
 * sets if the item at the given position is shown.
 * ===Arguments
 * * pos of the item. Integer
 * * val if item should be shown true/false
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _setItemShown(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->Show(cidx,RTEST(val));

	return self;
}


/*
 * call-seq:
 *   get_item_enabled(pos) -> true/false
 *
 * returns if the item at the given position is enabled.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _getItemEnabled(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		return wrap(_self->IsItemEnabled(cidx));
	return Qnil;
}


/*
 * call-seq:
 *   set_item_enabled(pos, val) -> self
 *
 * sets if the item at the given position is enabled.
 * ===Arguments
 * * pos of the item. Integer
 * * val if item should be shown true/false
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _setItemEnabled(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->Enable(cidx,RTEST(val));

	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXRadioBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

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

	rb_define_method(rb_cWXRadioBox,"get_item_shown",RUBY_METHOD_FUNC(_getItemShown),1);
	rb_define_method(rb_cWXRadioBox,"set_item_shown",RUBY_METHOD_FUNC(_setItemShown),2);

	rb_define_alias(rb_cWXRadioBox,"item_shown?","get_item_shown");

	rb_define_method(rb_cWXRadioBox,"get_item_enabled",RUBY_METHOD_FUNC(_getItemEnabled),1);
	rb_define_method(rb_cWXRadioBox,"set_item_enabled",RUBY_METHOD_FUNC(_setItemEnabled),2);

	rb_define_alias(rb_cWXRadioBox,"item_enabled?","get_item_enabled");

	rb_define_attr_method(rb_cWXRadioBox,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_cWXRadioBox,"string_selection",_getStringSelection,_setStringSelection);

	registerEventType("radiobox", wxEVT_RADIOBOX,rb_cWXCommandEvent);

	registerInfo<wxRadioBox>(rb_cWXRadioBox);
#endif

}


