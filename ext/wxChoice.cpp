/*
 * wxChoice.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"

VALUE rb_cWXChoice;

#if wxUSE_CHOICE
#define _self unwrap<wxChoice*>(self)

namespace RubyWX {
namespace Choice {

APP_PROTECT(wxChoice)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	_created = true;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("items")))))
			_self->Set(unwrap<wxArrayString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("select")))))
					_self->SetSelection(NUM2INT(temp));

	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXChoice(VALUE rb_mWX)
{
#if wxUSE_CHOICE
	using namespace RubyWX::Choice;
	rb_cWXChoice = rb_define_class_under(rb_mWX,"Choice",rb_cWXControl);
	rb_define_alloc_func(rb_cWXChoice,_alloc);

	rb_define_method(rb_cWXChoice,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxChoice>(rb_cWXChoice);
#endif

}

