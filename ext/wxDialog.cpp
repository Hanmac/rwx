/*
 * wxDialog.cpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxSizer.hpp"

VALUE rb_cWXDialog;

#define _self unwrap<wxDialog*>(self)

namespace RubyWX {
namespace Dialog {

APP_PROTECT(wxDialog)

macro_attr_with_func(EscapeId,wrapID,unwrapID)
macro_attr_with_func(ReturnCode,wrapID,unwrapID)
macro_attr_with_func(AffirmativeId,wrapID,unwrapID)


/*
 * call-seq:
 *   Dialog.new(parent, name, [options])
 *   Dialog.new(parent, [options])
 *
 * creates a new Dialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		wxWindowID id(wxID_ANY);
		wxString title(wxEmptyString);
		int style(wxDEFAULT_DIALOG_STYLE);

		if(rb_obj_is_kind_of(name,rb_cHash))
		{
			set_hash_option(name,"id",id,unwrapID);
			set_hash_option(name,"title",title);
			set_hash_option(name,"style",style);

			TopLevel::set_style_flags(hash,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,title,wxDefaultPosition,wxDefaultSize,style);
		
	}
	rb_call_super(argc,argv);
	return self;

}

DLL_LOCAL VALUE _ShowModal(VALUE self)
{
	return wrapID(_self->ShowModal());
}

DLL_LOCAL VALUE _EndModal(VALUE self,VALUE id)
{
	_self->EndModal(unwrapID(id));
	return self;
}


DLL_LOCAL VALUE _CreateButtonSizer(VALUE self,VALUE flags)
{
	return wrap(_self->CreateButtonSizer(unwrap_buttonflag(flags)));
}

#if wxUSE_STATTEXT

DLL_LOCAL VALUE _CreateTextSizer(VALUE self,VALUE text)
{
	return wrap(_self->CreateTextSizer(unwrap<wxString>(text)));
}
#endif

}
}

DLL_LOCAL void Init_WXDialog(VALUE rb_mWX)
{
	using namespace RubyWX::Dialog;
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXDialog,_alloc);

	rb_define_method(rb_cWXDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDialog, "return_code",_getReturnCode,_setReturnCode);

	rb_define_method(rb_cWXDialog,"show_modal",RUBY_METHOD_FUNC(_ShowModal),0);
	rb_define_method(rb_cWXDialog,"end_modal",RUBY_METHOD_FUNC(_EndModal),1);

	rb_define_method(rb_cWXDialog,"create_button_sizer",RUBY_METHOD_FUNC(_CreateButtonSizer),-2);
#if wxUSE_STATTEXT
	rb_define_method(rb_cWXDialog,"create_text_sizer",RUBY_METHOD_FUNC(_CreateTextSizer),1);
#endif
	rb_define_const(rb_cWXDialog,"DEFAULT_STYLE",INT2NUM(wxDEFAULT_DIALOG_STYLE));


	registerInfo<wxDialog>(rb_cWXDialog);
}
