/*
 * wxSingleChoiceDialog.cpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */


#include "wxAnyChoiceDialog.hpp"
#include "wxSingleChoiceDialog.hpp"

#include "wxApp.hpp"

VALUE rb_cWXSingleChoiceDialog;

#if wxUSE_CHOICEDLG
#define _self unwrap<wxSingleChoiceDialog*>(self)

namespace RubyWX {
namespace SingleChoiceDialog {

APP_PROTECT(wxSingleChoiceDialog)

macro_attr(Selection,int)

singlereturn(GetStringSelection)

/*
 * call-seq:
 *   SingleChoiceDialog.new(parent, [options])
 *
 * creates a new SingleChoiceDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * choices [string]
 *   * selection Integer
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name;
	rb_scan_args(argc, argv, "11",&parent,&name);



	if(!_created){
		int style = wxCHOICEDLG_STYLE,selection = -1;
		wxArrayString choices;

		wxString message(wxEmptyString);

		if(rb_obj_is_kind_of(name,rb_cHash)){
			set_hash_option(name,"style",style);
			set_hash_option(name,"choices",choices);
			set_hash_option(name,"selection",selection);
			set_hash_option(name,"message",message);
		}

		_self->Create(unwrap<wxWindow*>(parent),message,wxEmptyString,choices,(void **)NULL,style);
		

		_self->SetSelection(selection);
	}
	rb_call_super(argc,argv);
	return self;

}


VALUE _GetSingleChoice(int argc,VALUE *argv,VALUE self)
{
	VALUE message,caption,choices,hash;
	rb_scan_args(argc, argv, "31",&message,&caption,&choices,&hash);

	app_protected();

	wxWindow *parent = NULL;

	int x = wxDefaultCoord;
	int y = wxDefaultCoord;
	bool centre = true;
	int width = wxCHOICE_WIDTH;
	int height = wxCHOICE_HEIGHT;

	int selection = -1;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{

		set_hash_option(hash,"parent",parent);
		set_hash_option(hash,"x",x);
		set_hash_option(hash,"y",y);

		set_hash_option(hash,"center",centre);

		set_hash_option(hash,"width",width);
		set_hash_option(hash,"height",height);

		set_hash_option(hash,"selection",selection);

	}

	return wrap(wxGetSingleChoice(
			unwrap<wxString>(message), unwrap<wxString>(caption),
			unwrap<wxArrayString>(choices),
			parent, x, y, centre, width, height, selection
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



