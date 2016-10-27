/*
 * wxPickerBase.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */
#include "wxPickerBase.hpp"
#include "wxTextCtrl.hpp"

VALUE rb_cWXPickerBase;

#if wxUSE_CONTROLS

template <>
wxPickerBase* unwrap<wxPickerBase*>(const VALUE &arg)
{
	return unwrapTypedPtr<wxPickerBase>(arg,rb_cWXPickerBase);
}

#define _self unwrap<wxPickerBase*>(self)

namespace RubyWX {
namespace PickerBase {

macro_attr_con(InternalMargin,int,_self->HasTextCtrl())
macro_attr_con(TextCtrlProportion,int,_self->HasTextCtrl())
macro_attr(PickerCtrlProportion,int)
macro_attr_bool_con(TextCtrlGrowable,HasTextCtrl)
macro_attr_bool(PickerCtrlGrowable)

macro_attr(TextCtrl,wxTextCtrl*)
macro_attr(PickerCtrl,wxControl*)


void set_style_flags(VALUE hash,int& flags)
{

	set_hash_flag_option(hash,"use_textctrl",wxPB_USE_TEXTCTRL,flags);
	set_hash_flag_option(hash,"small",wxPB_SMALL,flags);

}


}
}

#endif


/* Document-attr: text_ctrl_growable
 * true if the text control is growable. true/false
 */
/* Document-attr: picker_ctrl_growable
 * true if the picker control is growable. true/false
 */
/* Document-attr: text_ctrl_proportion
 * the proportion value of the text control. Integer
 */
/* Document-attr: picker_ctrl_proportion
 * the proportion value of the picker. Integer
 */
/* Document-attr: text_ctrl
 * the text control. WX::TextCtrl / nil
 */
/* Document-attr: picker_ctrl
 * the picker control. WX::Control / nil
 */
/* Document-attr: internal_margin
 * the margin (in pixel) between the picker and the text control. Integer
 */

/* Document-const: USE_TEXTCTRL
 *   Creates a text control to the left of the picker which is completely managed by this WX::PickerBase class.
 */
/* Document-const: SMALL
 *   Use smaller version of the control with a small "..." button
 */
DLL_LOCAL void Init_WXPickerBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXPickerBase,"internal_margin",1,1);

	rb_define_attr(rb_cWXPickerBase,"text_ctrl",1,1);
	rb_define_attr(rb_cWXPickerBase,"picker_ctrl",1,1);

	rb_define_attr(rb_cWXPickerBase,"text_ctrl_proportion",1,1);
	rb_define_attr(rb_cWXPickerBase,"picker_ctrl_proportion",1,1);

	rb_define_attr(rb_cWXPickerBase,"text_ctrl_growable",1,1);
	rb_define_attr(rb_cWXPickerBase,"picker_ctrl_growable",1,1);

#endif

#if wxUSE_CONTROLS
	using namespace RubyWX::PickerBase;
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXPickerBase);

	rb_define_attr_method(rb_cWXPickerBase,"internal_margin",_getInternalMargin,_setInternalMargin);

	rb_define_attr_method(rb_cWXPickerBase,"text_ctrl",_getTextCtrl,_setTextCtrl);
	rb_define_attr_method(rb_cWXPickerBase,"picker_ctrl",_getPickerCtrl,_setPickerCtrl);

	rb_define_attr_method(rb_cWXPickerBase,"text_ctrl_proportion",_getTextCtrlProportion,_setTextCtrlProportion);
	rb_define_attr_method(rb_cWXPickerBase,"picker_ctrl_proportion",_getPickerCtrlProportion,_setPickerCtrlProportion);

	rb_define_attr_method(rb_cWXPickerBase,"text_ctrl_growable",_getTextCtrlGrowable,_setTextCtrlGrowable);
	rb_define_attr_method(rb_cWXPickerBase,"picker_ctrl_growable",_getPickerCtrlGrowable,_setPickerCtrlGrowable);

	rb_define_const(rb_cWXPickerBase,"USE_TEXTCTRL",RB_INT2NUM(wxPB_USE_TEXTCTRL));
	rb_define_const(rb_cWXPickerBase,"SMALL",RB_INT2NUM(wxPB_SMALL));
#endif

}


