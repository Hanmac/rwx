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
	return unwrapPtr<wxPickerBase>(arg,rb_cWXPickerBase);
}

#define _self unwrap<wxPickerBase*>(self)

namespace RubyWX {
namespace PickerBase {

macro_attr(InternalMargin,int)
macro_attr_con(TextCtrlProportion,int,HasTextCtrl)
macro_attr(PickerCtrlProportion,int)
macro_attr_bool_con(TextCtrlGrowable,HasTextCtrl)
macro_attr_bool(PickerCtrlGrowable)

macro_attr(TextCtrl,wxTextCtrl*)
macro_attr(PickerCtrl,wxControl*)

}
}

#endif
DLL_LOCAL void Init_WXPickerBase(VALUE rb_mWX)
{
#if 0
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

#endif

}


