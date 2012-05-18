/*
 * wxDirDialog.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */


#include "wxDirDialog.hpp"
#include "wxPoint.hpp"

VALUE rb_cWXDirDialog;
#if wxUSE_DIRDLG
#define _self wrap<wxDirDialog*>(self)

namespace RubyWX {
namespace DirDialog {

APP_PROTECT(wxDirDialog)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent));
	_created = true;
	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("message")))))
			_self->SetMessage(wrap<wxString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("path")))))
			_self->SetPath(wrap<wxString>(temp));

	}

	return self;
}

macro_attr(Path,wxString)
macro_attr(Message,wxString)


VALUE _getUserDir(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "02",&parent,&hash);
	wxString message = wxDirSelectorPromptStr;
	wxString defaultPath = wxEmptyString;
	long style = wxDD_DEFAULT_STYLE;
	wxPoint pos = wxDefaultPosition;

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("message")))))
			message = wrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("path")))))
			defaultPath =wrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("style")))))
			style = NUM2LONG(temp);
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("pos")))))
			pos =wrap<wxPoint>(temp);
	}

	return wrap(wxDirSelector(message,
			defaultPath,style,pos,
			wrap<wxWindow*>(parent)));
}
#endif
}
}

void Init_WXDirDialog(VALUE rb_mWX)
{
#if wxUSE_DIRDLG
	using namespace RubyWX::DirDialog;
	rb_cWXDirDialog = rb_define_class_under(rb_mWX,"DirDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXDirDialog,_alloc);
	rb_define_method(rb_cWXDirDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDirDialog,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXDirDialog,"path",_getPath,_setPath);

	rb_define_module_function(rb_mWX,"dir_dialog",RUBY_METHOD_FUNC(_getUserDir),-1);

	registerInfo<wxDirDialog>(rb_cWXDirDialog);
#endif
}
