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

macro_attr_func(Modal,ShowModal(),EndModal,wrapID,unwrapID, true)

singlereturn(IsModal)

singlereturn(GetContentWindow)

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

/* Document-method: modal?
 * call-seq:
 *   modal? -> true/false
 *
 * Returns true if this dialog does run modal, false if otherwise.
 * ===Return value
 * true/false
*/


/*
 * Document-class: WX::Dialog
 *
 * This class represents a dialog window.
*/

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */


/* Document-attr: return_code
 * Integer/Symbol
 * Modal dialogs have a return code -
 * usually the id of the last pressed button
 */
/* Document-attr: affirmative_id
 * Integer/Symbol
 * Set the identifier for the affirmative button: this button will close
 * the dialog
 */
/* Document-attr: escape_id
 * Integer/Symbol
 * Set identifier for Esc key translation: the button with this id will
 * close the dialog without doing anything else
 */


/* Document-attr: content_window
 * WX::Window
 * Returns a content window if there is one. This can be used by the layout adapter, for
 * example to make the pages of a book control into scrolling windows
 */

DLL_LOCAL void Init_WXDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
#endif

	using namespace RubyWX::Dialog;
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXDialog,_alloc);

	rb_define_method(rb_cWXDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if 0
	rb_define_attr(rb_cWXDialog, "return_code",1,1);
	rb_define_attr(rb_cWXDialog, "escape_id",1,1);
	rb_define_attr(rb_cWXDialog, "affirmative_id",1,1);

	rb_define_attr(rb_cWXDialog, "content_window",1,0);
#endif

	rb_define_attr_method(rb_cWXDialog, "return_code",_getReturnCode,_setReturnCode);
	rb_define_attr_method(rb_cWXDialog, "escape_id",_getEscapeId,_setEscapeId);
	rb_define_attr_method(rb_cWXDialog, "affirmative_id",_getAffirmativeId,_setAffirmativeId);

	rb_define_attr_method(rb_cWXDialog, "content_window",_GetContentWindow,NULL);

	rb_define_method(rb_cWXDialog,"show_modal",RUBY_METHOD_FUNC(_getModal),0);
	rb_define_method(rb_cWXDialog,"end_modal",RUBY_METHOD_FUNC(_setModal),1);
	rb_define_method(rb_cWXDialog,"modal?",RUBY_METHOD_FUNC(_IsModal),0);

	rb_define_method(rb_cWXDialog,"create_button_sizer",RUBY_METHOD_FUNC(_CreateButtonSizer),-2);
#if wxUSE_STATTEXT
	rb_define_method(rb_cWXDialog,"create_text_sizer",RUBY_METHOD_FUNC(_CreateTextSizer),1);
#endif
	rb_define_const(rb_cWXDialog,"DEFAULT_STYLE",RB_INT2NUM(wxDEFAULT_DIALOG_STYLE));


	registerInfo<wxDialog>(rb_cWXDialog);
}
