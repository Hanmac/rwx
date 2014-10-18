/*
 * wxMultiChoiceDialog.cpp
 *
 *  Created on: 08.12.2013
 *      Author: hanmac
 */


#include "wxAnyChoiceDialog.hpp"
#include "wxMultiChoiceDialog.hpp"

#include "wxApp.hpp"

VALUE rb_cWXMultiChoiceDialog;

#if wxUSE_CHOICEDLG
#define _self unwrap<wxMultiChoiceDialog*>(self)

namespace RubyWX {
namespace MultiChoiceDialog {

APP_PROTECT(wxMultiChoiceDialog)

macro_attr(Selections,wxArrayInt)


/*
 * call-seq:
 *   MultiChoiceDialog.new(parent, name, [options])
 *   MultiChoiceDialog.new(parent, [options])
 *
 * creates a new MultiChoiceDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * choices [String]
 *   * selections [Integer]
 *   * message String
 *   * caption String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		int style(wxCHOICEDLG_STYLE);
		wxArrayString choices;
		wxArrayInt selections;

		wxString message(wxEmptyString);
		wxString caption(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"selections",selections);
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);
		}
		_self->Create(unwrap<wxWindow*>(parent),message,caption,choices,style);

		_self->SetSelections(selections);

	}
	rb_call_super(argc,argv);
	return self;

}


VALUE _GetSelectedChoices(int argc,VALUE *argv,VALUE self)
{
	VALUE message,caption,choices,hash;
	rb_scan_args(argc, argv, "31",&message,&caption,&choices,&hash);

	app_protected();

	wxWindow *parent = NULL;

	wxArrayInt selections;

	int x(wxDefaultCoord);
	int y(wxDefaultCoord);
	bool centre(true);
	int width(wxCHOICE_WIDTH);
	int height(wxCHOICE_HEIGHT);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"parent",parent);
		set_hash_option(hash,"x",x);
		set_hash_option(hash,"y",y);

		set_hash_option(hash,"center",centre);

		set_hash_option(hash,"width",width);
		set_hash_option(hash,"height",height);

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
	rb_cWXAnyChoiceDialog = rb_define_class_under(rb_mWX,"AnyChoiceDialog",rb_cWXDialog);
#endif

#if wxUSE_CHOICEDLG
	using namespace RubyWX::MultiChoiceDialog;
	rb_cWXMultiChoiceDialog = rb_define_class_under(rb_mWX,"MultiChoiceDialog",rb_cWXAnyChoiceDialog);
	rb_define_alloc_func(rb_cWXMultiChoiceDialog,_alloc);

#if 0
	rb_define_attr(rb_cWXMultiChoiceDialog,"selections",1,1);
#endif

	rb_define_method(rb_cWXMultiChoiceDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXMultiChoiceDialog,"selections",_getSelections,_setSelections);

	rb_define_module_function(rb_mWX,"multi_choices",RUBY_METHOD_FUNC(_GetSelectedChoices),-1);


	registerInfo<wxMultiChoiceDialog>(rb_cWXMultiChoiceDialog);
#endif
}



