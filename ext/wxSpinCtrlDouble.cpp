/*
 * wxSpinCtrlDouble.cpp
 *
 *  Created on: 18.10.2014
 *      Author: hanmac
 */


#include "wxSpinCtrlDouble.hpp"
#include "wxSpinButton.hpp"
#include "wxNotifyEvent.hpp"

VALUE rb_cWXSpinCtrlDouble, rb_cWXSpinDoubleEvent;

#if wxUSE_SPINCTRL
#define _self unwrap<wxSpinCtrlDouble*>(self)

namespace RubyWX {
namespace SpinCtrlDouble {

macro_attr(Value,double)
macro_attr(Increment,double)
macro_attr(Digits,int)
macro_attr(SnapToTicks, bool)


singlereturn(GetMax)
singlereturn(GetMin)

DLL_LOCAL VALUE _setMin(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetRange(NUM2DBL(val),_self->GetMax());
	return val;
}

DLL_LOCAL VALUE _setMax(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetRange(_self->GetMin(), NUM2DBL(val));
	return val;
}

APP_PROTECT(wxSpinCtrlDouble)

/*
 * call-seq:
 *   SpinCtrlDouble.new(parent, name, [options])
 *   SpinCtrlDouble.new(parent, [options])
 *
 * creates a new SpinCtrlDouble widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * min Float
 *   * max Float
 *   * value Float
 *   * increment Float
 *   * digits Integer
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(hash,rb_cHash))
	{
		wxWindowID id(wxID_ANY);
		double value(0), min(0), max(100), inc(1);

		int style(wxSP_ARROW_KEYS | wxALIGN_RIGHT);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"min",min);
			set_hash_option(hash,"max",max);
			set_hash_option(hash,"increment",inc);
			set_hash_option(hash,"style",style);

			SpinButton::set_style_flags(hash, style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxEmptyString,wxDefaultPosition,wxDefaultSize,
				style,min,max,value,inc
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"digits",&wxSpinCtrlDouble::SetDigits,_self);
	}

	return self;
}


namespace Event
{
#undef _self
#define _self unwrap<wxSpinDoubleEvent*>(self)
macro_attr(Value,double)
}


}
}
#endif

/*
 * Document-class: WX::SpinCtrlDouble
 *
 * This class represents a SpinCtrl for Float.
*/

/* Document-attr: value
 * the value of the SpinCtrlDouble. Float
 */
/* Document-attr: min
 * the minimum value of the SpinCtrlDouble. Float
 */
/* Document-attr: max
 * the maximum value of the SpinCtrlDouble. Float
 */


/* Document-attr: increment
 * the increment value of the SpinCtrlDouble. Float
 */
/* Document-attr: digits
 * the digits value of the SpinCtrlDouble. Integer
 */


/* Document-const: ARROW_KEYS
 *   The user can use arrow keys to change the value.
 */
/* Document-const: WRAP
 *   The value wraps at the minimum and maximum.
 */

DLL_LOCAL void Init_WXSpinCtrlDouble(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_SPINCTRL
	using namespace RubyWX::SpinCtrlDouble;
	rb_cWXSpinCtrlDouble = rb_define_class_under(rb_mWX,"SpinCtrlDouble",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinCtrlDouble,_alloc);

	rb_cWXSpinDoubleEvent = rb_define_class_under(rb_cWXEvent,"SpinDouble",rb_cWXNotifyEvent);


#if 0
	rb_define_attr(rb_cWXSpinCtrlDouble,"value",1,1);
	rb_define_attr(rb_cWXSpinCtrlDouble,"increment",1,1);
	rb_define_attr(rb_cWXSpinCtrlDouble,"digits",1,1);
	rb_define_attr(rb_cWXSpinCtrlDouble,"min",1,1);
	rb_define_attr(rb_cWXSpinCtrlDouble,"max",1,1);

	rb_define_attr(rb_cWXSpinDoubleEvent,"value",1,1);

#endif

	rb_define_method(rb_cWXSpinCtrlDouble,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinCtrlDouble,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinCtrlDouble,"increment",_getIncrement,_setIncrement);
	rb_define_attr_method(rb_cWXSpinCtrlDouble,"digits",_getDigits,_setDigits);
	rb_define_attr_method(rb_cWXSpinCtrlDouble,"min",_GetMin,_setMin);
	rb_define_attr_method(rb_cWXSpinCtrlDouble,"max",_GetMax,_setMax);

	//from SpinButton
	rb_define_const(rb_cWXSpinCtrlDouble,"ARROW_KEYS",RB_INT2NUM(wxSP_ARROW_KEYS));
	rb_define_const(rb_cWXSpinCtrlDouble,"WRAP",RB_INT2NUM(wxSP_WRAP));

	rb_define_attr_method(rb_cWXSpinDoubleEvent,"value",Event::_getValue,Event::_setValue);

	registerInfo<wxSpinCtrlDouble>(rb_cWXSpinCtrlDouble);
#endif

}




