/*
 * wxComboCtrl.cpp
 *
 *  Created on: 14.10.2014
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxTextEntry.hpp"
#include "wxTextCtrl.hpp"
#include "wxComboCtrl.hpp"

VALUE rb_cWXComboCtrl;

#if wxUSE_COMBOCTRL
#define _self unwrap<wxComboCtrl*>(self)

namespace RubyWX {
namespace ComboCtrl {

APP_PROTECT(wxComboCtrl)

singlefunc(Popup)
singlefunc(Dismiss)

singlereturn(GetPopupWindow)
singlereturn(GetTextCtrl)
singlereturn(GetButton)

singlereturn(GetBitmapNormal)
singlereturn(GetBitmapPressed)
singlereturn(GetBitmapHover)
singlereturn(GetBitmapDisabled)

//macro_attr(PopupMinWidth,int)
macro_attr(CustomPaintWidth,int)


DLL_LOCAL VALUE _set_bitmap_normal(VALUE self, VALUE val)
{
	_self->SetButtonBitmaps(
		unwrap<wxBitmap>(val),
		false,
		_self->GetBitmapPressed(),
		_self->GetBitmapHover(),
		_self->GetBitmapDisabled()
    );
	return val;
}
DLL_LOCAL VALUE _set_bitmap_pressed(VALUE self, VALUE val)
{
	_self->SetButtonBitmaps(
		_self->GetBitmapNormal(),
		false,
		unwrap<wxBitmap>(val),
		_self->GetBitmapHover(),
		_self->GetBitmapDisabled()
    );
	return val;
}
DLL_LOCAL VALUE _set_bitmap_hover(VALUE self, VALUE val)
{
	_self->SetButtonBitmaps(
		_self->GetBitmapNormal(),
		false,
		_self->GetBitmapPressed(),
		unwrap<wxBitmap>(val),
		_self->GetBitmapDisabled()
    );
	return val;
}
DLL_LOCAL VALUE _set_bitmap_disabled(VALUE self, VALUE val)
{
	_self->SetButtonBitmaps(
		_self->GetBitmapNormal(),
		false,
		_self->GetBitmapPressed(),
		_self->GetBitmapHover(),
		unwrap<wxBitmap>(val)
    );
	return val;
}

/*
 * call-seq:
 *   ComboCtrl.new(parent, name, [options])
 *   ComboCtrl.new(parent, [options])
 *
 * creates a new ComboCtrl widget.
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
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"style",style);

			TextCtrl::set_style_flags(hash,style);
		}
		_self->Create(
			unwrap<wxWindow*>(parent),id,value,
			wxDefaultPosition,wxDefaultSize,style
		);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) && rb_obj_is_kind_of(hash,rb_cHash)) {
		set_obj_option(hash, "value", &wxComboCtrl::SetValue,_self);
	}

	return self;
}

}
}
#endif

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

/* Document-attr: popup_window
* the popup window containing the popup control WX::Window
*/
/* Document-attr: text_ctrl
* the text control which is part of the combobox WX::TextCtrl
*/
/* Document-attr: button
* he dropdown button which is part of the combobox WX::Window
*/

/* Document-attr: bitmap_normal
* the bitmap for the normal state WX::Bitmap
*/
/* Document-attr: bitmap_pressed
* the bitmap for the pressed state WX::Bitmap
*/
/* Document-attr: bitmap_hover
* the bitmap for the hover state WX::Bitmap
*/
/* Document-attr: bitmap_disabled
* the bitmap for the disabled state WX::Bitmap
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
DLL_LOCAL void Init_WXComboCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_mWXTextEntry = rb_define_module_under(rb_mWX,"TextEntry");
#endif
#if wxUSE_COMBOCTRL
	using namespace RubyWX::ComboCtrl;
	rb_cWXComboCtrl = rb_define_class_under(rb_mWX,"ComboCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXComboCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXComboCtrl,"popup_window",1,0);
	rb_define_attr(rb_cWXComboCtrl,"text_ctrl",1,0);
	rb_define_attr(rb_cWXComboCtrl,"button",1,0);

	rb_define_attr(rb_cWXComboCtrl,"bitmap_normal",1,1);
	rb_define_attr(rb_cWXComboCtrl,"bitmap_pressed",1,1);
	rb_define_attr(rb_cWXComboCtrl,"bitmap_hover",1,1);
	rb_define_attr(rb_cWXComboCtrl,"bitmap_disabled",1,1);
#endif

	rb_define_attr_method(rb_cWXComboCtrl,"popup_window",_GetPopupWindow,0);
	rb_define_attr_method(rb_cWXComboCtrl,"text_ctrl",_GetTextCtrl,0);
	rb_define_attr_method(rb_cWXComboCtrl,"button",_GetButton,0);

	rb_define_attr_method(rb_cWXComboCtrl,"bitmap_normal",_GetBitmapNormal,_set_bitmap_normal);
	rb_define_attr_method(rb_cWXComboCtrl,"bitmap_pressed",_GetBitmapPressed,_set_bitmap_pressed);
	rb_define_attr_method(rb_cWXComboCtrl,"bitmap_hover",_GetBitmapHover,_set_bitmap_hover);
	rb_define_attr_method(rb_cWXComboCtrl,"bitmap_disabled",_GetBitmapDisabled,_set_bitmap_disabled);

	rb_define_method(rb_cWXComboCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXComboCtrl,"popup",RUBY_METHOD_FUNC(_Popup),0);
	rb_define_method(rb_cWXComboCtrl,"dismiss",RUBY_METHOD_FUNC(_Dismiss),0);

	rb_include_module(rb_cWXComboCtrl,rb_mWXTextEntry);

	//define TextCtrl constants
	rb_define_const(rb_cWXComboCtrl,"MULTILINE",RB_INT2NUM(wxTE_MULTILINE));
	rb_define_const(rb_cWXComboCtrl,"PROCESS_TAB",RB_INT2NUM(wxTE_PROCESS_TAB));
	rb_define_const(rb_cWXComboCtrl,"PROCESS_ENTER",RB_INT2NUM(wxTE_PROCESS_ENTER));
	rb_define_const(rb_cWXComboCtrl,"PASSWORD",RB_INT2NUM(wxTE_PASSWORD));

	registerInfo<wxComboCtrl>(rb_cWXComboCtrl);

#endif

}

