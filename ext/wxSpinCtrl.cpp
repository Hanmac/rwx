/*
 * wxSpinCtrl.cpp
 *
 *  Created on: 23.04.2012
 *      Author: hanmac
 */


#include "wxSpinCtrl.hpp"
#include "wxSpinButton.hpp"


VALUE rb_cWXSpinCtrl;

#if wxUSE_SPINCTRL
#define _self unwrap<wxSpinCtrl*>(self)

namespace RubyWX {
namespace SpinCtrl {

macro_attr(Value,int)
macro_attr(Increment,int)
macro_attr(SnapToTicks, bool)
macro_attr(Base,int)

singlereturn(GetMax)
singlereturn(GetMin)

DLL_LOCAL VALUE _setMin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetRange(NUM2INT(val),_self->GetMax());
	return val;
}

DLL_LOCAL VALUE _setMax(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetRange(_self->GetMin(), NUM2INT(val));
	return val;
}

APP_PROTECT(wxSpinCtrl)

/*
 * call-seq:
 *   SpinCtrl.new(parent, name, [options])
 *   SpinCtrl.new(parent, [options])
 *
 * creates a new SpinCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * min Integer
 *   * max Integer
 *   * value Integer
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(hash,rb_cHash))
	{
		wxWindowID id(wxID_ANY);
		int value(0), min(0), max(100);

		int style(wxSP_ARROW_KEYS | wxALIGN_RIGHT);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"min",min);
			set_hash_option(hash,"max",max);

			set_hash_option(hash,"style",style);

			SpinButton::set_style_flags(hash, style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxEmptyString,wxDefaultPosition,wxDefaultSize,
				style,min,max,value
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"increment",&wxSpinCtrl::SetIncrement,_self);
	}

	return self;
}

}
}
#endif

/*
 * Document-class: WX::SpinCtrl
 *
 * This class represents a SpinCtrl for Integer.
*/

/* Document-attr: value
 * the value of the SpinCtrl. Integer
 */
/* Document-attr: min
 * the minimum value of the SpinCtrl. Integer
 */
/* Document-attr: max
 * the maximum value of the SpinCtrl. Integer
 */
/* Document-attr: increment
 * the increment value of the SpinCtrl. Integer
 */

/* Document-const: ARROW_KEYS
 *   The user can use arrow keys to change the value.
 */
/* Document-const: WRAP
 *   The value wraps at the minimum and maximum.
 */

DLL_LOCAL void Init_WXSpinCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_SPINCTRL
	using namespace RubyWX::SpinCtrl;
	rb_cWXSpinCtrl = rb_define_class_under(rb_mWX,"SpinCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXSpinCtrl,"value",1,1);
	rb_define_attr(rb_cWXSpinCtrl,"increment",1,1);
	rb_define_attr(rb_cWXSpinCtrl,"min",1,1);
	rb_define_attr(rb_cWXSpinCtrl,"max",1,1);
#endif

	rb_define_method(rb_cWXSpinCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinCtrl,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinCtrl,"increment",_getIncrement,_setIncrement);
	rb_define_attr_method(rb_cWXSpinCtrl,"min",_GetMin,_setMin);
	rb_define_attr_method(rb_cWXSpinCtrl,"max",_GetMax,_setMax);

	//from SpinButton
	rb_define_const(rb_cWXSpinCtrl,"ARROW_KEYS",INT2NUM(wxSP_ARROW_KEYS));
	rb_define_const(rb_cWXSpinCtrl,"WRAP",INT2NUM(wxSP_WRAP));

	registerInfo<wxSpinCtrl>(rb_cWXSpinCtrl);

	registerEventType("spinctrl", wxEVT_SPINCTRL,rb_cWXSpinEvent);
#endif

}




