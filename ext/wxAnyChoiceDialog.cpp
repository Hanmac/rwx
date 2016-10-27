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

/*
 * call-seq:
 *   AnyChoiceDialog.new(parent, name, [options])
 *   AnyChoiceDialog.new(parent, [options])
 *
 * creates a new SingleChoiceDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * choices [string]
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);


	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		int style(wxCHOICEDLG_STYLE);
		wxArrayString choices;

		wxString message(wxEmptyString);
		wxString caption(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash)){
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);
		}

		_self->Create(unwrap<wxWindow*>(parent),message,caption,choices,style);
		
	}
	rb_call_super(argc,argv);
	return self;

}

}
}
#endif

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */
DLL_LOCAL void Init_WXAnyChoiceDialog(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

#endif

#if wxUSE_CHOICEDLG
	using namespace RubyWX::AnyChoiceDialog;
	rb_cWXAnyChoiceDialog = rb_define_class_under(rb_mWX,"AnyChoiceDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXAnyChoiceDialog,_alloc);

	rb_define_method(rb_cWXAnyChoiceDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_const(rb_cWXAnyChoiceDialog,"DEFAULT_STYLE",RB_INT2NUM(wxCHOICEDLG_STYLE));


	registerInfo<wxAnyChoiceDialog>(rb_cWXAnyChoiceDialog);
#endif
}



