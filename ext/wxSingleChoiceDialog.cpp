/*
 * wxSingleChoiceDialog.cpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */


#include "wxAnyChoiceDialog.hpp"
#include "wxSingleChoiceDialog.hpp"

VALUE rb_cWXSingleChoiceDialog;

#if wxUSE_CHOICEDLG
#define _self unwrap<wxSingleChoiceDialog*>(self)

namespace RubyWX {
namespace SingleChoiceDialog {

APP_PROTECT(wxSingleChoiceDialog)

macro_attr(Selection,int)

singlereturn(GetStringSelection)

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

			if(!NIL_P(temp=rb_hash_aref(name,ID2SYM(rb_intern("selection")))))
				selection = NUM2INT(temp);

		}
		_self->Create(unwrap<wxWindow*>(parent),wxEmptyString,wxEmptyString,choices,(void **)NULL,style);
		_created = true;

		_self->SetSelection(selection);
	}
	rb_call_super(argc,argv);
	return self;

}

}
}
#endif
DLL_LOCAL void Init_WXSingleChoiceDialog(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXSingleChoiceDialog,"selection",1,1);
#endif

#if wxUSE_CHOICEDLG
	using namespace RubyWX::SingleChoiceDialog;
	rb_cWXSingleChoiceDialog = rb_define_class_under(rb_mWX,"SingleChoiceDialog",rb_cWXAnyChoiceDialog);
	rb_define_alloc_func(rb_cWXSingleChoiceDialog,_alloc);

	rb_define_method(rb_cWXSingleChoiceDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSingleChoiceDialog,"selection",_getSelection,_setSelection);

	rb_define_method(rb_cWXSingleChoiceDialog,"string_selection",RUBY_METHOD_FUNC(_GetStringSelection),0);

	registerInfo<wxSingleChoiceDialog>(rb_cWXSingleChoiceDialog);
#endif
}



