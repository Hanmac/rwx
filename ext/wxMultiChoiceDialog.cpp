/*
 * wxMultiChoiceDialog.cpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */


#include "wxAnyChoiceDialog.hpp"
#include "wxMultiChoiceDialog.hpp"

VALUE rb_cWXMultiChoiceDialog;

#if wxUSE_CHOICEDLG
#define _self unwrap<wxMultiChoiceDialog*>(self)

namespace RubyWX {
namespace MultiChoiceDialog {

APP_PROTECT(wxMultiChoiceDialog)

macro_attr(Selections,wxArrayInt)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name;
	rb_scan_args(argc, argv, "11",&parent,&name);

	int style = wxCHOICEDLG_STYLE,selection = -1;
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
DLL_LOCAL void Init_WXMultiChoiceDialog(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXMultiChoiceDialog,"selections",1,1);
#endif

#if wxUSE_CHOICEDLG
	using namespace RubyWX::MultiChoiceDialog;
	rb_cWXMultiChoiceDialog = rb_define_class_under(rb_mWX,"MultiChoiceDialog",rb_cWXAnyChoiceDialog);
	rb_define_alloc_func(rb_cWXMultiChoiceDialog,_alloc);

	rb_define_method(rb_cWXMultiChoiceDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXMultiChoiceDialog,"selections",_getSelections,_setSelections);

	registerInfo<wxMultiChoiceDialog>(rb_cWXMultiChoiceDialog);
#endif
}



