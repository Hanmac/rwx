/*
 * wxSpinCtrl.cpp
 *
 *  Created on: 23.04.2012
 *      Author: hanmac
 */


#include "wxSpinCtrl.hpp"


VALUE rb_cWXSpinCtrl;

#if wxUSE_SPINCTRL
#define _self unwrap<wxSpinCtrl*>(self)

namespace RubyWX {
namespace SpinCtrl {

macro_attr(Value,int)
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
	long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT;
	int value = 0, min = 0, max = 100;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"value",value);
		set_hash_option(hash,"min",min);
		set_hash_option(hash,"max",max);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,
		wxEmptyString,wxDefaultPosition,wxDefaultSize,
		style,min,max,value
	);
	

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif

/* Document-attr: value
 * the value of the SpinCtrl. Integer
 */
/* Document-attr: mix
 * the minimum value of the SpinCtrl. Integer
 */
/* Document-attr: max
 * the maximum value of the SpinCtrl. Integer
 */

DLL_LOCAL void Init_WXSpinCtrl(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXSpinCtrl,"value",1,1);
	rb_define_attr(rb_cWXSpinCtrl,"min",1,1);
	rb_define_attr(rb_cWXSpinCtrl,"max",1,1);
#endif

#if wxUSE_SPINCTRL
	using namespace RubyWX::SpinCtrl;
	rb_cWXSpinCtrl = rb_define_class_under(rb_mWX,"SpinCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinCtrl,_alloc);

	rb_define_method(rb_cWXSpinCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinCtrl,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinCtrl,"min",_GetMin,_setMin);
	rb_define_attr_method(rb_cWXSpinCtrl,"max",_GetMax,_setMax);

	registerInfo<wxSpinCtrl>(rb_cWXSpinCtrl);
#endif

}




