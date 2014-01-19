/*
 * wxDirDialog.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */


#include "wxDirDialog.hpp"
#include "wxPoint.hpp"
#include "wxApp.hpp"

VALUE rb_cWXDirDialog;
#if wxUSE_DIRDLG
#define _self unwrap<wxDirDialog*>(self)

namespace RubyWX {
namespace DirDialog {

APP_PROTECT(wxDirDialog)

/*
 * call-seq:
 *   DirDialog.new(parent, name, [options])
 *   DirDialog.new(parent, [options])
 *
 * creates a new DirDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxString message(wxDirSelectorPromptStr);
		wxString path(wxEmptyString);
		int style(wxDD_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"path",path);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),message,path,style);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(message,Message,wxString)
		set_option(path,Path,wxString)
	}

	return self;
}

macro_attr(Path,wxString)
macro_attr(Message,wxString)


DLL_LOCAL VALUE _getUserDir(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "02",&parent,&hash);

	app_protected();

	wxString message(wxDirSelectorPromptStr);
	wxString defaultPath(wxEmptyString);
	int style(wxDD_DEFAULT_STYLE);
	wxPoint pos(wxDefaultPosition);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"path",defaultPath);
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"pos",pos);

		set_hash_flag_option(hash,"must_exist",wxDD_DIR_MUST_EXIST,style);

	}

	return wrap(wxDirSelector(message,
			defaultPath,style,pos,
			unwrap<wxWindow*>(parent)));
}

}
}

#endif

DLL_LOCAL void Init_WXDirDialog(VALUE rb_mWX)
{
#if wxUSE_DIRDLG
	using namespace RubyWX::DirDialog;
	rb_cWXDirDialog = rb_define_class_under(rb_mWX,"DirDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXDirDialog,_alloc);
	rb_define_method(rb_cWXDirDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDirDialog,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXDirDialog,"path",_getPath,_setPath);

	rb_define_module_function(rb_mWX,"dir_dialog",RUBY_METHOD_FUNC(_getUserDir),-1);

	rb_define_const(rb_cWXDirDialog,"DEFAULT_STYLE",INT2NUM(wxDD_DEFAULT_STYLE));
	rb_define_const(rb_cWXDirDialog,"DIR_MUST_EXIST",INT2NUM(wxDD_DIR_MUST_EXIST));

	registerInfo<wxDirDialog>(rb_cWXDirDialog);
#endif
}
