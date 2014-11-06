/*
 * wxBitmapComboBox.cpp
 *
 *  Created on: 21.06.2014
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"
#include "wxTextEntry.hpp"
#include "wxBitmapComboBox.hpp"
#include "wxChoice.hpp"
#include "wxTextCtrl.hpp"

VALUE rb_cWXBitmapComboBox;

#if wxUSE_BITMAPCOMBOBOX
#define _self unwrap<wxBitmapComboBox*>(self)

namespace RubyWX {
namespace BitmapComboBox {

APP_PROTECT(wxBitmapComboBox)

//need to define it again to prevent Shadowing
singlefunc(Clear)

singlefunc(Popup)
singlefunc(Dismiss)

#if defined(wxBITMAPCOMBOBOX_OWNERDRAWN_BASED) && !defined(HAVE_WXOWNERDRAWNCOMBOBOX_ISLISTEMPTY)
DLL_LOCAL VALUE _IsListEmpty(VALUE self)
{
	return wrap(dynamic_cast<wxItemContainer*>(_self)->IsEmpty());
}
DLL_LOCAL VALUE _IsTextEmpty(VALUE self)
{
	return wrap(dynamic_cast<wxTextEntry*>(_self)->IsEmpty());
}
#else
singlereturn(IsListEmpty)
singlereturn(IsTextEmpty)
#endif

/*
 * call-seq:
 *   BitmapComboBox.new(parent, name, [options])
 *   BitmapComboBox.new(parent, [options])
 *
 * creates a new BitmapComboBox widget.
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

		bool selflag(false);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"items",choices);
			set_hash_option(hash,"style",style);
			selflag = set_hash_option(hash,"selection",selection);

			Choice::set_style_flags(hash,style);
			TextCtrl::set_style_flags(hash,style);
		}
		
		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,value,
				wxDefaultPosition,wxDefaultSize,
				choices,style
			);

			if(selflag && check_index(selection,_self->GetCount()))
				_self->SetSelection(selection);
		}
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

singlereturn(GetCount)


/*
 * call-seq:
 *   each_item_bitmap -> Enumerator
 *   each_item_bitmap { | text, bitmap| } -> self
 *
 * iterates the items of this control.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_bitmap(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetCount));
	for(std::size_t i = 0; i < _self->GetCount(); ++i)
		rb_yield_values(2,wrap(_self->GetString(i)),wrap(_self->GetItemBitmap(i)));
	return self;
}

macro_attr_item(ItemBitmap,GetItemBitmap, SetItemBitmap, GetCount, wxBitmap)


}
}
#endif

/* Document-method: get_item_bitmap
 * call-seq:
 *   get_item_bitmap(pos) -> WX::Bitmap
 *
 * returns the Bitmap of the item at the given position.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * WX::Bitmap
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-method: set_item_bitmap
 * call-seq:
 *   set_item_bitmap(pos,text) -> self
 *
 * sets the Bitmap of the item at the given position.
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
DLL_LOCAL void Init_WXBitmapComboBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");
#endif
#if wxUSE_COMBOBOX
	using namespace RubyWX::BitmapComboBox;
	rb_cWXBitmapComboBox = rb_define_class_under(rb_mWX,"BitmapComboBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXBitmapComboBox,_alloc);

	rb_define_method(rb_cWXBitmapComboBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXBitmapComboBox,"clear",RUBY_METHOD_FUNC(_Clear),0);
	rb_define_method(rb_cWXBitmapComboBox,"popup",RUBY_METHOD_FUNC(_Popup),0);
	rb_define_method(rb_cWXBitmapComboBox,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);


	rb_include_module(rb_cWXBitmapComboBox,rb_mWXItemContainer);
	rb_include_module(rb_cWXBitmapComboBox,rb_mWXTextEntry);

	rb_undef_method(rb_cWXBitmapComboBox,"empty?");

	rb_define_method(rb_cWXBitmapComboBox,"list_empty?",RUBY_METHOD_FUNC(_IsListEmpty),0);
	rb_define_method(rb_cWXBitmapComboBox,"text_empty?",RUBY_METHOD_FUNC(_IsTextEmpty),0);

	rb_define_method(rb_cWXBitmapComboBox,"get_item_bitmap",RUBY_METHOD_FUNC(_getItemBitmap),1);
	rb_define_method(rb_cWXBitmapComboBox,"set_item_bitmap",RUBY_METHOD_FUNC(_setItemBitmap),2);
	rb_define_method(rb_cWXBitmapComboBox,"each_item_bitmap",RUBY_METHOD_FUNC(_each_bitmap),0);


	//define Choice constants
	rb_define_const(rb_cWXBitmapComboBox,"SORT",INT2NUM(wxCB_SORT));
	rb_define_const(rb_cWXBitmapComboBox,"READONLY",INT2NUM(wxCB_READONLY));
	rb_define_const(rb_cWXBitmapComboBox,"DROPDOWN",INT2NUM(wxCB_DROPDOWN));

	//define TextCtrl constants
	rb_define_const(rb_cWXBitmapComboBox,"MULTILINE",INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXBitmapComboBox,"PROCESS_TAB",INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXBitmapComboBox,"PROCESS_ENTER",INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXBitmapComboBox,"PASSWORD",INT2NUM(wxTE_PASSWORD));

	registerInfo<wxBitmapComboBox>(rb_cWXBitmapComboBox);

#endif

}

