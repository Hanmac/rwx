/*
 * wxListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxListBox.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXListBox;

#if wxUSE_LISTBOX

namespace RubyWX {
namespace ListBox {
#define _self unwrap<wxListBox*>(self)

APP_PROTECT(wxListBox)

/*
 * call-seq:
 *   ListBox.new(parent, name, [options])
 *   ListBox.new(parent, [options])
 *
 * creates a new ListBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set:
 *   * sort true/false adds the SortStyle to make this
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxArrayString choices;
		int style(0);
		int selection(-1);


		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"selection",selection);

			set_hash_flag_option(hash,"sort",wxLB_SORT,style);
			set_hash_flag_option(hash,"multiple",wxLB_MULTIPLE,style);

		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,
			wxDefaultPosition,wxDefaultSize,
			choices,style
		);
		
		_self->SetSelection(selection);

	}
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _getSelections(VALUE self)
{
	if(_self->HasMultipleSelection())
	{
		wxArrayInt data;
		_self->GetSelections(data);
		return wrap(data);
	} else {
		return wrap(_self->GetSelection());
	}

}


DLL_LOCAL VALUE _setSelections(VALUE self,VALUE val)
{
	rb_check_frozen(self);

	if(NIL_P(val)) {
		_self->SetSelection(wxNOT_FOUND);
	} else {
		wxArrayInt data(unwrap<wxArrayInt>(val));

		for(wxArrayInt::iterator it = data.begin();it != data.end();++it)
			if(check_index(*it,_self->GetCount()))
				_self->SetSelection(*it);
	}
	return val;
}


DLL_LOCAL VALUE _getStringSelections(VALUE self)
{
	if(_self->HasMultipleSelection())
	{
		wxArrayInt data;
		wxArrayString result;
		_self->GetSelections(data);
		for(wxArrayInt::iterator it = data.begin(); it != data.end();++it)
			result.push_back(_self->GetString(*it));
		return wrap(result);
	} else {
		return wrap(_self->GetStringSelection());
	}

}


DLL_LOCAL VALUE _setStringSelection(VALUE self,VALUE val)
{
	rb_check_frozen(self);

	if(NIL_P(val)) {
		_self->SetSelection(wxNOT_FOUND);
	} else {
		wxArrayString data(unwrap<wxArrayString>(val));

		for(wxArrayString::iterator it = data.begin();it != data.end();++it)
			_self->SetStringSelection(*it);
	}
	return val;
}

DLL_LOCAL VALUE _each_selection_size(VALUE self)
{
	wxArrayInt data;
	return INT2NUM(_self->GetSelections(data));
}

DLL_LOCAL VALUE _each_selection(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_selection_size);
	wxArrayInt data;
	_self->GetSelections(data);
	for(wxArrayInt::iterator it = data.begin(); it != data.end();++it)
		rb_yield_values(2,INT2NUM(*it),wrap(_self->GetString(*it)));
	return self;
}

}
}

#endif
DLL_LOCAL void Init_WXListBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");

	rb_define_attr(rb_cWXListBox,"selection",1,1);
	rb_define_attr(rb_cWXListBox,"string_selection",1,1);
#endif
#if wxUSE_LISTBOX
	using namespace RubyWX::ListBox;
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXListBox,_alloc);

	rb_define_method(rb_cWXListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXListBox,rb_mWXItemContainer);

	rb_define_method(rb_cWXListBox,"each_selection",RUBY_METHOD_FUNC(_each_selection),0);

	rb_define_attr_method(rb_cWXListBox,"selection",_getSelections,_setSelections);

	rb_define_attr_method(rb_cWXListBox,"string_selection",_getStringSelections,_setStringSelection);

	registerEventType("listbox", wxEVT_LISTBOX,rb_cWXCommandEvent);
	registerEventType("listbox_dclick",  wxEVT_LISTBOX_DCLICK,rb_cWXCommandEvent);

	rb_define_const(rb_cWXListBox,"SORT",INT2NUM(wxLB_SORT));
	rb_define_const(rb_cWXListBox,"SINGLE",INT2NUM(wxLB_SINGLE));
	rb_define_const(rb_cWXListBox,"MULTIPLE",INT2NUM(wxLB_MULTIPLE));

	registerInfo<wxListBox>(rb_cWXListBox);
#endif

}


