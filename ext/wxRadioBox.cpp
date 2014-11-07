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

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
		}

		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,label,
				wxDefaultPosition,wxDefaultSize,
				choices,0,style
			);

		}
	}
	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_ruby_option(hash, "selection", _setSelection, self);
	}

	return self;
}

singlereturn(GetCount)

/*
 * call-seq:
 *   each_item -> Enumerator
 *   each_item { | text| } -> self
 *
 * iterates the items of this control.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetCount));
	std::size_t count = _self->GetCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetString(i)));
	return self;
}

macro_attr_item(ItemString,GetString, SetString, GetCount, wxString)
macro_attr_item(ItemShown,IsItemShown, Show, GetCount, bool)
macro_attr_item(ItemEnabled,IsItemEnabled, Enable, GetCount, bool)
#if wxUSE_TOOLTIPS
macro_attr_item(ItemToolTip,GetItemToolTip, SetItemToolTip, GetCount, wxString)
#endif
#if wxUSE_HELP
macro_attr_item(ItemHelpText,GetItemHelpText, SetItemHelpText, GetCount, wxString)
#endif

}
}

#endif

/* Document-method: get_item_string
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

/* Document-method: set_item_string
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

/* Document-method: get_item_shown
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

/* Document-method: set_item_shown
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

/* Document-method: get_item_enabled
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

/* Document-method: set_item_enabled
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

/* Document-method: get_item_tooltip
 * call-seq:
 *   get_item_tooltip(pos) -> String/nil
 *
 * returns the tool tip of the item or nil if none is set.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * String/nil
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-method: set_item_tooltip
 * call-seq:
 *   set_item_tooltip(pos, val) -> self
 *
 * sets the tool tip of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * * val is the tool tip of the item. String
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-method: get_item_helptext
 * call-seq:
 *   get_item_helptext(pos) -> String
 *
 * returns the help text of the item.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * String
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/


/* Document-method: set_item_helptext
 * call-seq:
 *   set_item_helptext(pos, val) -> self
 *
 * sets the help text of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * * val is the help text of the item. String
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-attr: selection
 * Integer/nil returns the index of the current selected item, or nil if none is selected.
 */
/* Document-attr: string_selection
 * String returns the string of the current selected item.
 */


DLL_LOCAL void Init_WXRadioBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif
#if wxUSE_RADIOBOX
	using namespace RubyWX::RadioBox;
	rb_cWXRadioBox = rb_define_class_under(rb_mWX,"RadioBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXRadioBox,_alloc);

#if 0
	rb_define_attr(rb_cWXRadioBox,"selection",1,1);
	rb_define_attr(rb_cWXRadioBox,"string_selection",1,1);
#endif

	rb_define_method(rb_cWXRadioBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXRadioBox,"each_item",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXRadioBox,"get_item_string",RUBY_METHOD_FUNC(_getItemString),1);
	rb_define_method(rb_cWXRadioBox,"set_item_string",RUBY_METHOD_FUNC(_setItemString),2);

	rb_define_method(rb_cWXRadioBox,"get_item_shown",RUBY_METHOD_FUNC(_getItemShown),1);
	rb_define_method(rb_cWXRadioBox,"set_item_shown",RUBY_METHOD_FUNC(_setItemShown),2);

	rb_define_alias(rb_cWXRadioBox,"item_shown?","get_item_shown");

	rb_define_method(rb_cWXRadioBox,"get_item_enabled",RUBY_METHOD_FUNC(_getItemEnabled),1);
	rb_define_method(rb_cWXRadioBox,"set_item_enabled",RUBY_METHOD_FUNC(_setItemEnabled),2);

	rb_define_alias(rb_cWXRadioBox,"item_enabled?","get_item_enabled");

#if wxUSE_TOOLTIPS
	rb_define_method(rb_cWXRadioBox,"get_item_tooltip",RUBY_METHOD_FUNC(_getItemToolTip),1);
	rb_define_method(rb_cWXRadioBox,"set_item_tooltip",RUBY_METHOD_FUNC(_setItemToolTip),2);
#endif

#if wxUSE_HELP
	rb_define_method(rb_cWXRadioBox,"get_item_helptext",RUBY_METHOD_FUNC(_getItemHelpText),1);
	rb_define_method(rb_cWXRadioBox,"set_item_helptext",RUBY_METHOD_FUNC(_setItemHelpText),2);
#endif

	rb_define_attr_method(rb_cWXRadioBox,"selection",_getSelection,_setSelection);
	rb_define_attr_method(rb_cWXRadioBox,"string_selection",_getStringSelection,_setStringSelection);

	registerEventType("radiobox", wxEVT_RADIOBOX,rb_cWXCommandEvent);

	registerInfo<wxRadioBox>(rb_cWXRadioBox);
#endif

}


