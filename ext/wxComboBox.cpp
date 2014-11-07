/*
 * wxComboBox.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"
#include "wxTextEntry.hpp"
#include "wxChoice.hpp"
#include "wxTextCtrl.hpp"

VALUE rb_cWXComboBox;

#if wxUSE_COMBOBOX
#define _self unwrap<wxComboBox*>(self)

namespace RubyWX {
namespace ComboBox {

APP_PROTECT(wxComboBox)

//need to define it again to prevent Shadowing
singlefunc(Clear)

singlefunc(Popup)
singlefunc(Dismiss)

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

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"items",choices);
			set_hash_option(hash,"style",style);

			Choice::set_style_flags(hash,style);
			TextCtrl::set_style_flags(hash,style);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,value,
			wxDefaultPosition,wxDefaultSize,
			choices,style
		);

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {

		if(rb_obj_is_kind_of(name,rb_cString)) {
			set_ruby_option(hash, "items", ItemContainer::_setItems, self);
			set_obj_option(hash, "value", &wxComboBox::SetValue,_self);
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

	rb_include_module(rb_cWXComboBox,rb_mWXItemContainer);
	rb_include_module(rb_cWXComboBox,rb_mWXTextEntry);

	rb_define_method(rb_cWXComboBox,"clear",RUBY_METHOD_FUNC(_Clear),0);
	rb_define_method(rb_cWXComboBox,"popup",RUBY_METHOD_FUNC(_Popup),0);
	rb_define_method(rb_cWXComboBox,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);

	rb_undef_method(rb_cWXComboBox,"empty?");

	rb_define_method(rb_cWXComboBox,"list_empty?",RUBY_METHOD_FUNC(_IsListEmpty),0);
	rb_define_method(rb_cWXComboBox,"text_empty?",RUBY_METHOD_FUNC(_IsTextEmpty),0);

	//define Choice constants
	rb_define_const(rb_cWXComboBox,"SORT",INT2NUM(wxCB_SORT));
	rb_define_const(rb_cWXComboBox,"READONLY",INT2NUM(wxCB_READONLY));
	rb_define_const(rb_cWXComboBox,"DROPDOWN",INT2NUM(wxCB_DROPDOWN));

	//define TextCtrl constants
	rb_define_const(rb_cWXComboBox,"MULTILINE",INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXComboBox,"PROCESS_TAB",INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXComboBox,"PROCESS_ENTER",INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXComboBox,"PASSWORD",INT2NUM(wxTE_PASSWORD));

	registerInfo<wxComboBox>(rb_cWXComboBox);

	registerEventType("combobox",wxEVT_COMBOBOX);
	registerEventType("combobox_dropdown",wxEVT_COMBOBOX_DROPDOWN);
	registerEventType("combobox_closeup",wxEVT_COMBOBOX_CLOSEUP);

#endif

}

