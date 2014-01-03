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

#if wxUSE_SPINBTN
#define _self unwrap<wxSpinButton*>(self)

namespace RubyWX {
namespace SpinButton {

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)

APP_PROTECT(wxSpinButton)

/*
 * call-seq:
 *   SpinButton.new(parent, [options])
 *
 * creates a new SpinButton widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * min Integer
 *   * max Integer
 *   * value Integer
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	_created = true;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(value,Value,int)
		set_option(min,Min,int)
		set_option(max,Max,int)
	}

	rb_call_super(argc,argv);
	return self;
}

namespace Event
{
#undef _self
#define _self unwrap<wxSpinEvent*>(self)
macro_attr(Value,int)
}

}
}
#endif



/* Document-attr: value
 * the value of the SpinButton. Integer
 */
/* Document-attr: mix
 * the minimum value of the SpinButton. Integer
 */
/* Document-attr: max
 * the maximum value of the SpinButton. Integer
 */

DLL_LOCAL void Init_WXSpinButton(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXSpinButton,"value",1,1);
	rb_define_attr(rb_cWXSpinButton,"min",1,1);
	rb_define_attr(rb_cWXSpinButton,"max",1,1);

	rb_define_attr(rb_cWXSpinEvent,"value",1,1);
#endif

#if wxUSE_SPINBTN
	using namespace RubyWX::SpinButton;
	rb_cWXSpinButton = rb_define_class_under(rb_mWX,"SpinButton",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinButton,_alloc);

	rb_define_method(rb_cWXSpinButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinButton,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinButton,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSpinButton,"max",_getMax,_setMax);


	rb_cWXSpinEvent = rb_define_class_under(rb_cWXEvent,"Spin",rb_cWXNotifyEvent);

	rb_define_attr_method(rb_cWXSpinEvent,"value",Event::_getValue,Event::_setValue);

	registerEventType("spin", wxEVT_SPIN,rb_cWXSpinEvent);
	registerEventType("spin_up", wxEVT_SPIN_UP,rb_cWXSpinEvent);
	registerEventType("spin_down", wxEVT_SPIN_DOWN,rb_cWXSpinEvent);

	registerInfo<wxSpinButton>(rb_cWXSpinButton);
#endif

}



