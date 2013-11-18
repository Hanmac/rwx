/*
 * wxComboBox.cpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */



#include "wxEvtHandler.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXComboBox;

#if wxUSE_COMBOBOX
#define _self unwrap<wxComboBox*>(self)

namespace RubyWX {
namespace ComboBox {

APP_PROTECT(wxComboBox)

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
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("items")))))
			_self->Set(unwrap<wxArrayString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("select")))))
					_self->SetSelection(NUM2INT(temp));
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXComboBox(VALUE rb_mWX)
{
#if wxUSE_COMBOBOX
	using namespace RubyWX::ComboBox;
	rb_cWXComboBox = rb_define_class_under(rb_mWX,"ComboBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXComboBox,_alloc);

	rb_define_method(rb_cWXComboBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXComboBox,rb_mWXItemContainer);

	registerInfo<wxComboBox>(rb_cWXComboBox);
#endif

}

