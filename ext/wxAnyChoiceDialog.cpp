/*
 * wxAnyChoiceDialog.cpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */


#include "wxAnyChoiceDialog.hpp"

VALUE rb_cWXAnyChoiceDialog;

#if wxUSE_CHOICEDLG
#define _self unwrap<wxAnyChoiceDialog*>(self)

namespace RubyWX {
namespace AnyChoiceDialog {

APP_PROTECT(wxAnyChoiceDialog)


DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name;
	rb_scan_args(argc, argv, "11",&parent,&name);

	int style = wxCHOICEDLG_STYLE;
	wxArrayString choices;

	if(!_created){
		if(rb_obj_is_kind_of(name,rb_cHash))
		{
			VALUE temp;
			if(!NIL_P(temp=rb_hash_aref(name,ID2SYM(rb_intern("style")))))
				style = NUM2INT(temp);

			if(!NIL_P(temp=rb_hash_aref(name,ID2SYM(rb_intern("choices")))))
				choices = unwrap<wxArrayString>(temp);
		}
		_self->Create(unwrap<wxWindow*>(parent),wxEmptyString,wxEmptyString,choices,style);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;

}

}
}
#endif
DLL_LOCAL void Init_WXAnyChoiceDialog(VALUE rb_mWX)
{
#if wxUSE_CHOICEDLG
	using namespace RubyWX::AnyChoiceDialog;
	rb_cWXAnyChoiceDialog = rb_define_class_under(rb_mWX,"AnyChoiceDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXAnyChoiceDialog,_alloc);

	rb_define_method(rb_cWXAnyChoiceDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_const(rb_cWXAnyChoiceDialog,"DEFAULT_STYLE",INT2NUM(wxCHOICEDLG_STYLE));


	registerInfo<wxAnyChoiceDialog>(rb_cWXAnyChoiceDialog);
#endif
}



