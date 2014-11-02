/*
 * wxFileDialog.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */


#include "wxFileDialog.hpp"
#include "wxFileCtrlBase.hpp"
#include "wxApp.hpp"

VALUE rb_cWXFileDialog;

#if wxUSE_FILEDLG
#define _self unwrap<wxFileDialog*>(self)

namespace RubyWX {
namespace FileDialog {

APP_PROTECT(wxFileDialog)

/*
 * call-seq:
 *   FileDialog.new(parent, name, [options])
 *   FileDialog.new(parent, [options])
 *
 * creates a new FileDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * wildcard String default WildCard
 *   * directory String default directory
 *   * filename String default filename
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
		wxString message(wxFileSelectorPromptStr);
		wxString directory(wxEmptyString);
		wxString filename(wxEmptyString);
		wxString wildcard(wxFileSelectorDefaultWildcardStr);
		int style(wxFD_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"directory",directory);
			set_hash_option(hash,"filename",filename);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"wildcard",wildcard,unwrapWildCard);

			TopLevel::set_style_flags(hash,style);
			set_hash_flag_option(hash,"open",wxFD_OPEN,style);
			set_hash_flag_option(hash,"save",wxFD_SAVE,style);
			set_hash_flag_option(hash,"overwrite_prompt",wxFD_OVERWRITE_PROMPT,style);
#ifdef HAVE_CONST_WXFD_NO_FOLLOW
			set_hash_flag_option(hash,"no_follow",wxFD_NO_FOLLOW,style);
#endif
			set_hash_flag_option(hash,"must_exist",wxFD_FILE_MUST_EXIST,style);
			set_hash_flag_option(hash,"multiple",wxFD_MULTIPLE,style);
			set_hash_flag_option(hash,"change_dir",wxFD_CHANGE_DIR,style);
			set_hash_flag_option(hash,"preview",wxFD_PREVIEW,style);


		}

		_self->Create(unwrap<wxWindow*>(parent),message,directory,filename,wildcard,style);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash,"wildcard", &wxFileDialogBase::SetWildcard, _self, unwrapWildCard);

		set_obj_option(hash,"message", &wxFileDialogBase::SetMessage, _self);
		set_obj_option(hash,"directory", &wxFileDialogBase::SetDirectory, _self);
		set_obj_option(hash,"filename", &wxFileDialogBase::SetFilename, _self);
		set_obj_option(hash,"path", &wxFileDialogBase::SetPath, _self);
		set_obj_option(hash,"wildcard", &wxFileDialogBase::SetPath, _self);

	}

	return self;
}

macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr_with_func(Wildcard,wrap,unwrapWildCard)
macro_attr(Message,wxString)
macro_attr(Path,wxString)

singlereturn(GetFilterIndex)
singlereturn_array(GetFilenames, wxArrayString)
singlereturn_array(GetPaths, wxArrayString)


VALUE _setFilterIndex(VALUE self,VALUE other)
{
	rb_check_frozen(self);
	int filter(NUM2INT(other));

	if(check_filter_index(filter,_self->GetWildcard()))
		_self->SetFilterIndex(filter);

	return other;
}

DLL_LOCAL VALUE _saveFileSelector(int argc,VALUE *argv,VALUE self)
{
	VALUE what,extension,default_name,parent;
	rb_scan_args(argc, argv, "22",&what,&extension,&default_name,&parent);
	app_protected();
	return wrap(wxSaveFileSelector(unwrap<wxString>(what),
			unwrap<wxString>(extension),
			unwrap<wxString>(default_name),
			unwrap<wxWindow*>(parent)));
}
DLL_LOCAL VALUE _loadFileSelector(int argc,VALUE *argv,VALUE self)
{
	VALUE what,extension,default_name,parent;
	rb_scan_args(argc, argv, "22",&what,&extension,&default_name,&parent);
	app_protected();
	return wrap(wxLoadFileSelector(unwrap<wxString>(what),
			unwrap<wxString>(extension),
			unwrap<wxString>(default_name),
			unwrap<wxWindow*>(parent)));
}

}
}
#endif


/* Document-attr: wildcard
 * the wildcard of the FileDialog. String
 */
/* Document-attr: filter_index
 * the filter_index of the FileDialog.
 * Can't be higher than filters in wildcard. Integer
 */
/* Document-attr: directory
 * the directory of the FileDialog. String
 */
/* Document-attr: filename
 * the filename of the FileDialog. String
 */
/* Document-attr: message
 * the message of the FileDialog. String
 */
/* Document-attr: path
 * the path of the FileDialog. String
 */
/* Document-attr: filenames
 * the filenamex of the FileDialog. Array<String>
 */
/* Document-attr: paths
 * the paths of the FileDialog. Array<String>
 */


/* Document-const: OPEN
 * This is an open dialog; usually this means that the default button's label of the dialog is "Open".
 */
/* Document-const: SAVE
 * This is a save dialog; usually this means that the default button's label of the dialog is "Save".
 */
/* Document-const: OVERWRITE_PROMPT
 *  For save dialog only: prompt for a confirmation if a file will be overwritten.
 */
/* Document-const: NO_FOLLOW
 *  Directs the dialog to return the path and file name of the selected shortcut file, not its target as it does by default.
 *  Currently this flag is only implemented in wxMSW and the non-dereferenced link path is always returned, even without this flag, under Unix and so using it there doesn't do anything.
 */
/* Document-const: MUST_EXIST
 *  For open dialog only: the user may only select files that actually exist.
 *  Notice that under OS X the file dialog with wxFD_OPEN style always behaves as if this style was specified,
 *  because it is impossible to choose a file that doesn't exist from a standard OS X file dialog.
 */
/* Document-const: MULTIPLE
 *  For open dialog only: allows selecting multiple files.
 */
/* Document-const: CHANGE_DIR
 *  Change the current working directory (when the dialog is dismissed) to the directory where the file(s) chosen by the user are.
 */
/* Document-const: PREVIEW
 *  Show the preview of the selected files (currently only supported by wxGTK).
 */

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

DLL_LOCAL void Init_WXFileDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
#endif

#if wxUSE_FILEDLG
	using namespace RubyWX::FileDialog;
	rb_cWXFileDialog = rb_define_class_under(rb_mWX,"FileDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXFileDialog,_alloc);
	rb_define_method(rb_cWXFileDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if 0
	rb_define_attr(rb_cWXFileDialog,"directory",1,1);
	rb_define_attr(rb_cWXFileDialog,"filename",1,1);
	rb_define_attr(rb_cWXFileDialog,"filter_index",1,1);
	rb_define_attr(rb_cWXFileDialog,"wildcard",1,1);
	rb_define_attr(rb_cWXFileDialog,"message",1,1);
	rb_define_attr(rb_cWXFileDialog,"path",1,1);
	rb_define_attr(rb_cWXFileDialog,"filenames",1,0);
	rb_define_attr(rb_cWXFileDialog,"paths",1,0);
#endif

	rb_define_attr_method(rb_cWXFileDialog,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileDialog,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileDialog,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileDialog,"filter_index",_GetFilterIndex,_setFilterIndex);
	rb_define_attr_method(rb_cWXFileDialog,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXFileDialog,"path",_getPath,_setPath);

	rb_define_attr_method(rb_cWXFileDialog,"filenames",_GetFilenames,NULL);
	rb_define_attr_method(rb_cWXFileDialog,"paths",_GetPaths,NULL);

	rb_define_module_function(rb_mWX,"load_dialog",RUBY_METHOD_FUNC(_loadFileSelector),-1);
	rb_define_module_function(rb_mWX,"save_dialog",RUBY_METHOD_FUNC(_saveFileSelector),-1);

	rb_define_const(rb_cWXFileDialog,"OPEN",INT2NUM(wxFD_OPEN));
	rb_define_const(rb_cWXFileDialog,"SAVE",INT2NUM(wxFD_SAVE));
	rb_define_const(rb_cWXFileDialog,"OVERWRITE_PROMPT",INT2NUM(wxFD_OVERWRITE_PROMPT));
#ifdef HAVE_CONST_WXFD_NO_FOLLOW
	rb_define_const(rb_cWXFileDialog,"NO_FOLLOW",INT2NUM(wxFD_NO_FOLLOW));
#endif
	rb_define_const(rb_cWXFileDialog,"MUST_EXIST",INT2NUM(wxFD_FILE_MUST_EXIST));
	rb_define_const(rb_cWXFileDialog,"MULTIPLE",INT2NUM(wxFD_MULTIPLE));
	rb_define_const(rb_cWXFileDialog,"CHANGE_DIR",INT2NUM(wxFD_CHANGE_DIR));
	rb_define_const(rb_cWXFileDialog,"PREVIEW",INT2NUM(wxFD_PREVIEW));

	rb_define_const(rb_cWXFileDialog,"DEFAULT_STYLE",INT2NUM(wxFD_DEFAULT_STYLE));

	registerInfo<wxFileDialog>(rb_cWXFileDialog);
#endif
}



