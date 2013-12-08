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


VALUE _GetSelectedChoices(int argc,VALUE *argv,VALUE self)
{
	VALUE message,caption,choices,hash;
	rb_scan_args(argc, argv, "31",&message,&caption,&choices,&hash);

	wxWindow *parent = NULL;

	wxArrayInt selections;

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

	if(wxGetSelectedChoices(selections,
			unwrap<wxString>(message), unwrap<wxString>(caption),
			unwrap<wxArrayString>(choices),
			parent, x, y, centre, width, height
	) != -1)
		return wrap(selections);
	return Qnil;
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

	rb_define_module_function(rb_mWX,"multi_choices",RUBY_METHOD_FUNC(_GetSelectedChoices),-1);


	registerInfo<wxMultiChoiceDialog>(rb_cWXMultiChoiceDialog);
#endif
}



