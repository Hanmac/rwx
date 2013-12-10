/*
 * wxChoice.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXChoice;

#if wxUSE_CHOICE
#define _self unwrap<wxChoice*>(self)

namespace RubyWX {
namespace Choice {

APP_PROTECT(wxChoice)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!rb_obj_is_kind_of(hash,rb_cString)) {
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		VALUE temp;
		set_option(items,,wxArrayString)
		set_option(select,Selection,int)
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXChoice(VALUE rb_mWX)
{
#if wxUSE_CHOICE
	using namespace RubyWX::Choice;
	rb_cWXChoice = rb_define_class_under(rb_mWX,"Choice",rb_cWXControl);
	rb_define_alloc_func(rb_cWXChoice,_alloc);

	rb_define_method(rb_cWXChoice,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXChoice,rb_mWXItemContainer);

	registerInfo<wxChoice>(rb_cWXChoice);
#endif

}

