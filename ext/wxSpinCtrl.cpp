/*
 * wxSpinCtrl.cpp
 *
 *  Created on: 23.04.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"


VALUE rb_cWXSpinCtrl;

#if wxUSE_SPINCTRL
#define _self wrap<wxSpinCtrl*>(self)

namespace RubyWX {
namespace SpinCtrl {

macro_attr(Value,int)
singlereturn(GetMax)
singlereturn(GetMin)

VALUE _setMin(VALUE self,VALUE val)
{
	_self->SetRange(NUM2INT(val),_self->GetMax());
	return val;
}

VALUE _setMax(VALUE self,VALUE val)
{
	_self->SetRange(_self->GetMin(), NUM2INT(val));
	return val;
}


VALUE _alloc(VALUE self)
{
	return wrap(new wxSpinCtrl(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;


	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("range")))))
		{
			_self->SetRange(NUM2INT(rb_funcall(temp,rb_intern("begin"),0)),
			NUM2INT(rb_funcall(temp,rb_intern("end"),0)));
		}
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("value")))))
			_self->SetValue(NUM2INT(temp));
	}

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif

DLL_LOCAL void Init_WXSpinCtrl(VALUE rb_mWX)
{
#if wxUSE_SLIDER
	using namespace RubyWX::SpinCtrl;
	rb_cWXSpinCtrl = rb_define_class_under(rb_mWX,"SpinCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSpinCtrl,_alloc);

	rb_define_method(rb_cWXSpinCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSpinCtrl,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSpinCtrl,"min",_GetMin,_setMin);
	rb_define_attr_method(rb_cWXSpinCtrl,"max",_GetMax,_setMax);
#endif

}




