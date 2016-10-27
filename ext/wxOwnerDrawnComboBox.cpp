/*
 * wxComboBox.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxOwnerDrawnComboBox.hpp"
#include "wxItemContainer.hpp"
#include "wxTextEntry.hpp"
#include "wxChoice.hpp"
#include "wxComboCtrl.hpp"
#include "wxTextCtrl.hpp"

VALUE rb_cWXOwnerDrawnComboBox;

#if wxUSE_ODCOMBOBOX
#define _self unwrap<wxOwnerDrawnComboBox*>(self)

namespace RubyWX {
namespace OwnerDrawnComboBox {

APP_PROTECT(wxOwnerDrawnComboBox)

//need to define it again to prevent Shadowing
singlefunc(Clear)

#ifdef HAVE_WXOWNERDRAWNCOMBOBOX_ISLISTEMPTY
singlereturn(IsListEmpty)
singlereturn(IsTextEmpty)
#else
DLL_LOCAL VALUE _IsListEmpty(VALUE self)
{
	return wrap(dynamic_cast<wxItemContainer*>(_self)->IsEmpty());
}
DLL_LOCAL VALUE _IsTextEmpty(VALUE self)
{
	return wrap(dynamic_cast<wxTextEntry*>(_self)->IsEmpty());
}
#endif
/*
 * call-seq:
 *   OwnerDrawnComboBox.new(parent, name, [options])
 *   OwnerDrawnComboBox.new(parent, [options])
 *
 * creates a new OwnerDrawnComboBox widget.
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

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"items",choices);
			set_hash_option(hash,"style",style);

			Choice::set_style_flags(hash,style);
			TextCtrl::set_style_flags(hash,style);
		}

		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,value,
				wxDefaultPosition,wxDefaultSize,
				choices,style
			);
		}

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {

		if(rb_obj_is_kind_of(name,rb_cString)) {
			set_ruby_option(hash, "items", ItemContainer::_setItems, self);
			set_obj_option(hash, "value", &wxOwnerDrawnComboBox::SetValue,_self);
		}

		set_ruby_option(hash, "selection", ItemContainer::_setSelection, self);
	}


	return self;
}

}
}
#endif

/* Document-method: clear
 * call-seq:
 *   clear -> self
 *
 * Removes all items from the control.
 * ===Return value
 * self
*/
/* Document-method: popup
 * call-seq:
 *   popup -> self
 *
 * Shows the list box portion of the combo box.
 * ===Return value
 * self
*/
/* Document-method: dismiss
 * call-seq:
 *   dismiss -> self
 *
 * Hides the list box portion of the combo box.
 * ===Return value
 * self
*/
/* Document-method: list_empty?
 * call-seq:
 *   list_empty? -> true/false
 *
 * Returns true if the list of combobox choices is empty.
 * ===Return value
 * true/false
*/
/* Document-method: text_empty?
 * call-seq:
 *   text_empty? -> true/false
 *
 * Returns true if the text of the combobox is empty.
 * ===Return value
 * true/false
*/

/* Document-attr: list_selection
 * Integer/nil returns the index of the current selected item, or nil if none is selected.
 */
/* Document-attr: list_string_selection
 * String returns the string of the current selected item.
 */

/* Document-attr: text_selection
 * Range/nil returns the index of the current selected item,
 * or nil if none is selected.
 */
/* Document-attr: text_string_selection
 * String returns the string of the current selected item.
 */

/* Document-const: SORT
 *   Sorts the entries alphabetically.
 */
/* Document-const: READONLY
 *   The text will not be user-editable.
 */
/* Document-const: DROPDOWN
 *   Creates a combobox with a drop-down list.
 */

/* Document-const: MULTILINE
 *   The text control allows multiple lines. If this style is not specified, line break characters should not be used in the controls value.
 */
/* Document-const: PROCESS_TAB
 *   The control will receive :char events for TAB pressed - normally, TAB is used for passing to the next control in a dialog instead. For the control created with this style, you can still use Ctrl-Enter to pass to the next control from the keyboard.
 */
/* Document-const: PROCESS_ENTER
 *  The control will generate the event :text_enter (otherwise pressing Enter key is either processed internally by the control or used for navigation between dialog controls).
 */
/* Document-const: PASSWORD
 *   The text will be echoed as asterisks.
 */
DLL_LOCAL void Init_WXOwnerDrawnComboBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXComboCtrl = rb_define_class_under(rb_mWX,"ComboCtrl",rb_cWXControl);

	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");

	rb_define_attr(rb_cWXOwnerDrawnComboBox,"list_selection",1,1);
	rb_define_attr(rb_cWXOwnerDrawnComboBox,"list_string_selection",1,1);

	rb_define_attr(rb_cWXOwnerDrawnComboBox,"text_selection",1,1);
	rb_define_attr(rb_cWXOwnerDrawnComboBox,"text_string_selection",1,1);

#endif
#if wxUSE_COMBOBOX
	using namespace RubyWX::OwnerDrawnComboBox;
	rb_cWXOwnerDrawnComboBox = rb_define_class_under(rb_mWX,"OwnerDrawnComboBox",rb_cWXComboCtrl);
	rb_define_alloc_func(rb_cWXOwnerDrawnComboBox,_alloc);

	rb_define_method(rb_cWXOwnerDrawnComboBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_include_module(rb_cWXOwnerDrawnComboBox,rb_mWXItemContainer);

	rb_define_method(rb_cWXOwnerDrawnComboBox,"clear",RUBY_METHOD_FUNC(_Clear),0);

	rb_undef_method(rb_cWXOwnerDrawnComboBox,"empty?");

	rb_undef_method(rb_cWXOwnerDrawnComboBox,"selection");
	rb_undef_method(rb_cWXOwnerDrawnComboBox,"selection=");
	rb_undef_method(rb_cWXOwnerDrawnComboBox,"string_selection");
	rb_undef_method(rb_cWXOwnerDrawnComboBox,"string_selection=");

	rb_define_method(rb_cWXOwnerDrawnComboBox,"list_empty?",RUBY_METHOD_FUNC(_IsListEmpty),0);
	rb_define_method(rb_cWXOwnerDrawnComboBox,"text_empty?",RUBY_METHOD_FUNC(_IsTextEmpty),0);

	// import selection methods from ItemContainer
	{
		using namespace RubyWX::ItemContainer;
		rb_define_attr_method(rb_cWXOwnerDrawnComboBox,"list_selection",_getSelection,_setSelection);
		rb_define_attr_method(rb_cWXOwnerDrawnComboBox,"list_string_selection",_getStringSelection,_setStringSelection);
	}

	// import selection methods from TextEntry
	{
		using namespace RubyWX::TextEntry;
		rb_define_attr_method(rb_cWXOwnerDrawnComboBox,"text_selection",_getSelection,_setSelection);
		rb_define_attr_method(rb_cWXOwnerDrawnComboBox,"text_string_selection",_GetStringSelection,_setStringSelection);
	}

	//define Choice constants
	rb_define_const(rb_cWXOwnerDrawnComboBox,"SORT",RB_INT2NUM(wxCB_SORT));
	rb_define_const(rb_cWXOwnerDrawnComboBox,"READONLY",RB_INT2NUM(wxCB_READONLY));
	rb_define_const(rb_cWXOwnerDrawnComboBox,"DROPDOWN",RB_INT2NUM(wxCB_DROPDOWN));

	//define TextCtrl constants
	rb_define_const(rb_cWXOwnerDrawnComboBox,"MULTILINE",RB_INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXOwnerDrawnComboBox,"PROCESS_TAB",RB_INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXOwnerDrawnComboBox,"PROCESS_ENTER",RB_INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXOwnerDrawnComboBox,"PASSWORD",RB_INT2NUM(wxTE_PASSWORD));

	registerInfo<wxOwnerDrawnComboBox>(rb_cWXOwnerDrawnComboBox);

	registerEventType("combobox",wxEVT_COMBOBOX);
	registerEventType("combobox_dropdown",wxEVT_COMBOBOX_DROPDOWN);
	registerEventType("combobox_closeup",wxEVT_COMBOBOX_CLOSEUP);

#endif

}

