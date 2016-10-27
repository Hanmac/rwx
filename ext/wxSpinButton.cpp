/*
 * wxSpinButton.cpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */

#include "wxSpinButton.hpp"
#include "wxNotifyEvent.hpp"


VALUE rb_cWXSpinButton;
VALUE rb_cWXSpinEvent;


namespace RubyWX {
namespace SpinButton {

// that style flags can be used for SpinCtrl even if SpinButton is not defined
void set_style_flags(VALUE hash,int& flags)
{
	set_hash_flag_option(hash,"arrow_keys",wxSP_ARROW_KEYS,flags);
	set_hash_flag_option(hash,"wrap",wxSP_WRAP,flags);
}

#if wxUSE_SPINBTN
#define _self unwrap<wxSpinButton*>(self)

APP_PROTECT(wxSpinButton)

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)

singlereturn(IsVertical)


/*
 * call-seq:
 *   SpinButton.new(parent, name, [options])
 *   SpinButton.new(parent, [options])
 *
 * creates a new SpinButton widget.
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
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxSP_VERTICAL);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_flag_option(hash,"vertical",wxSP_VERTICAL,style);

			set_style_flags(hash, style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"value", &wxSpinButton::SetValue, _self);
		set_obj_option(hash,"min", &wxSpinButton::SetMin, _self);
		set_obj_option(hash,"max", &wxSpinButton::SetMax, _self);
	}


	return self;
}

namespace Event
{
#undef _self
#define _self unwrap<wxSpinEvent*>(self)
macro_attr(Value,int)
}

#endif

}
}



/* Document-attr: value
 * the value of the SpinButton. Integer
 */
/* Document-attr: min
 * the minimum value of the SpinButton. Integer
 */
/* Document-attr: max
 * the maximum value of the SpinButton. Integer
 */

/* Document-method: vertical?
 * call-seq:
 *   vertical? -> true/false
 *
 * returns true if this control is vertical.
 * ===Return value
 * true/false
*/

/* Document-const: VERTICAL
 *   Specifies a vertical spin button. 
 */
/* Document-const: ARROW_KEYS
 *   The user can use arrow keys to change the value.
 */
/* Document-const: WRAP
 *   The value wraps at the minimum and maximum.
 */

DLL_LOCAL void Init_WXSpinButton(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_SPINBTN
	using namespace RubyWX::SpinButton;
	rb_cWXSpinButton = rb_define_class_under(rb_mWX,"SpinButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinButton,_alloc);

	rb_cWXSpinEvent = rb_define_class_under(rb_cWXEvent,"Spin",rb_cWXNotifyEvent);

#if 0
	rb_define_attr(rb_cWXSpinButton,"value",1,1);
	rb_define_attr(rb_cWXSpinButton,"min",1,1);
	rb_define_attr(rb_cWXSpinButton,"max",1,1);

	rb_define_attr(rb_cWXSpinEvent,"value",1,1);
#endif

	rb_define_method(rb_cWXSpinButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinButton,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinButton,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSpinButton,"max",_getMax,_setMax);

	rb_define_method(rb_cWXSpinButton,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);

	rb_define_const(rb_cWXSpinButton,"VERTICAL",RB_INT2NUM(wxSP_VERTICAL));
	rb_define_const(rb_cWXSpinButton,"ARROW_KEYS",RB_INT2NUM(wxSP_ARROW_KEYS));
	rb_define_const(rb_cWXSpinButton,"WRAP",RB_INT2NUM(wxSP_WRAP));

	rb_define_attr_method(rb_cWXSpinEvent,"value",Event::_getValue,Event::_setValue);

	registerEventType("spin", wxEVT_SPIN,rb_cWXSpinEvent);
	registerEventType("spin_up", wxEVT_SPIN_UP,rb_cWXSpinEvent);
	registerEventType("spin_down", wxEVT_SPIN_DOWN,rb_cWXSpinEvent);

	registerInfo<wxSpinButton>(rb_cWXSpinButton);
#endif

}
