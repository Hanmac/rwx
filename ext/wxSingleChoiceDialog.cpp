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

	wxString message(wxEmptyString);

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

			if(!NIL_P(temp=rb_hash_aref(name,ID2SYM(rb_intern("message")))))
				message = unwrap<wxString>(temp);

		}
		_self->Create(unwrap<wxWindow*>(parent),message,wxEmptyString,choices,(void **)NULL,style);
		_created = true;

		_self->SetSelection(selection);
	}
	rb_call_super(argc,argv);
	return self;

}


VALUE _GetSingleChoice(int argc,VALUE *argv,VALUE self)
{
	VALUE message,caption,choices,hash;
	rb_scan_args(argc, argv, "31",&message,&caption,&choices,&hash);

	wxWindow *parent = NULL;

	int x = wxDefaultCoord;
	int y = wxDefaultCoord;
	bool centre = true;
	int width = wxCHOICE_WIDTH;
	int height = wxCHOICE_HEIGHT;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE tmp;
		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("parent")))))
			parent = unwrap<wxWindow*>(tmp);


		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("x")))))
			x = NUM2INT(tmp);
		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("y")))))
			y = NUM2INT(tmp);

		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("center")))))
			centre = RTEST(tmp);

		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("width")))))
			width = NUM2INT(tmp);
		if(!NIL_P(tmp = rb_hash_aref(hash,ID2SYM(rb_intern("height")))))
			height = NUM2INT(tmp);

	}

	return wrap(wxGetSingleChoice(
			unwrap<wxString>(message), unwrap<wxString>(caption),
			unwrap<wxArrayString>(choices),
			parent, x, y, centre, width, height
	));
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

	rb_define_module_function(rb_mWX,"single_choice",RUBY_METHOD_FUNC(_GetSingleChoice),-1);

	registerInfo<wxSingleChoiceDialog>(rb_cWXSingleChoiceDialog);
#endif
}



