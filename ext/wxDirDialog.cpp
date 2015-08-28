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


DLL_LOCAL void set_style_flags(VALUE hash, int &style)
{
	set_hash_flag_option(hash,"must_exist",wxDD_DIR_MUST_EXIST,style);
	set_hash_flag_option(hash,"change_dir",wxDD_CHANGE_DIR,style);
}

DLL_LOCAL void set_default_values(VALUE hash, wxString &message, wxString &path, int &style)
{
	set_hash_option(hash,"message",message);
	set_hash_option(hash,"path",path);
	set_hash_option(hash,"style",style);
}

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
 *   * must_exist Style Flag does set MUST_EXIST
 *   * change_dir Style Flag does set CHANGE_DIR
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
			set_default_values(hash,message,path,style);

			TopLevel::set_style_flags(hash,style);
			set_style_flags(hash,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),message,path,style);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name, rb_cString) &&
		rb_obj_is_kind_of(hash, rb_cHash))
	{
		set_obj_option(hash, "message", &wxDirDialog::SetMessage, _self);
		set_obj_option(hash, "path", &wxDirDialog::SetPath, _self);
	}

	return self;
}

macro_attr(Path,wxString)
macro_attr(Message,wxString)

/*
 * call-seq:
 *   dir_dialog([parent], [options]) -> String
 *
 * shows an DirDialog.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
 *   * style Integer
 *   * pos WX::Point

 *   * must_exist Style Flag does set MUST_EXIST
 *   * change_dir Style Flag does set CHANGE_DIR
 * ===Return value
 * selected path
*/
DLL_LOCAL VALUE _getUserDir(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "01:",&parent,&hash);

	app_protected();

	wxString message(wxDirSelectorPromptStr);
	wxString defaultPath(wxEmptyString);
	int style(wxDD_DEFAULT_STYLE);
	wxPoint pos(wxDefaultPosition);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_default_values(hash,message,defaultPath,style);

		set_hash_option(hash,"pos",pos);

		set_style_flags(hash,style);

	}

	return wrap(wxDirSelector(message,
			defaultPath,style,pos,
			unwrap<wxWindow*>(parent)));
}

}
}

#endif

/* Document-attr: message
 * the message of the DirDialog. String
 */
/* Document-attr: path
 * the selected path of the DirDialog. String
 */

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */
/* Document-const: MUST_EXIST
 *  The dialog will allow the user to choose only an existing folder.
 *  When this style is not given, a "Create new directory" button is added
 *  to the dialog (on Windows) or some other way is provided to the user to type the name of a new folder.
 */
/* Document-const: CHANGE_DIR
 *  Change the current working directory to the directory chosen by the user.
 */

/* Document-const: DEFAULT_NAME
 *  default name of this widget
 */
/* Document-const: DEFAULT_FOLDER
 *  default folder for the dir dialog
 */
/* Document-const: DEFAULT_PROMPT
 *  default prompt for the dir dialog
 */

DLL_LOCAL void Init_WXDirDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
#endif

#if wxUSE_DIRDLG
	using namespace RubyWX::DirDialog;
	rb_cWXDirDialog = rb_define_class_under(rb_mWX,"DirDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXDirDialog,_alloc);
	rb_define_method(rb_cWXDirDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if 0
	rb_define_attr(rb_cWXDirDialog,"message",1,1);
	rb_define_attr(rb_cWXDirDialog,"path",1,1);
#endif

	rb_define_attr_method(rb_cWXDirDialog,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXDirDialog,"path",_getPath,_setPath);

	rb_define_module_function(rb_mWX,"dir_dialog",RUBY_METHOD_FUNC(_getUserDir),-1);

	rb_define_const(rb_cWXDirDialog,"DEFAULT_STYLE",INT2NUM(wxDD_DEFAULT_STYLE));
	rb_define_const(rb_cWXDirDialog,"MUST_EXIST",INT2NUM(wxDD_DIR_MUST_EXIST));
	rb_define_const(rb_cWXDirDialog,"CHANGE_DIR",INT2NUM(wxDD_CHANGE_DIR));

	rb_define_const(rb_cWXDirDialog,"DEFAULT_NAME",wrap(wxString(wxDirDialogNameStr)));
	rb_define_const(rb_cWXDirDialog,"DEFAULT_FOLDER",wrap(wxString(wxDirDialogDefaultFolderStr)));
	rb_define_const(rb_cWXDirDialog,"DEFAULT_PROMPT",wrap(wxString(wxDirSelectorPromptStr)));

	registerInfo<wxDirDialog>(rb_cWXDirDialog);
#endif
}
