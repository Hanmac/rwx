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

		}

		_self->Create(unwrap<wxWindow*>(parent),message,directory,filename,wildcard,style);
		
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cString) &&
		rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		set_option_func(wildcard,Wildcard,unwrapWildCard)
		set_option(message,Message,wxString)
		set_option(directory,Directory,wxString)
		set_option(filename,Filename,wxString)
		set_option(path,Path,wxString)

	}

	return self;
}

macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr_with_func(Wildcard,wrap,unwrapWildCard)
macro_attr(Message,wxString)
macro_attr(Path,wxString)

DLL_LOCAL VALUE _getFilenames(VALUE self)
{
	wxArrayString result;
	_self->GetFilenames(result);
	return wrap(result);
}

DLL_LOCAL VALUE _getPaths(VALUE self)
{
	wxArrayString result;
	_self->GetPaths(result);
	return wrap(result);
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

DLL_LOCAL void Init_WXFileDialog(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXFileDialog,"directory",1,1);
	rb_define_attr(rb_cWXFileDialog,"filename",1,1);
	rb_define_attr(rb_cWXFileDialog,"wildcard",1,1);
	rb_define_attr(rb_cWXFileDialog,"message",1,1);
	rb_define_attr(rb_cWXFileDialog,"path",1,1);

#endif

#if wxUSE_FILEDLG
	using namespace RubyWX::FileDialog;
	rb_cWXFileDialog = rb_define_class_under(rb_mWX,"FileDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXFileDialog,_alloc);
	rb_define_method(rb_cWXFileDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFileDialog,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileDialog,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileDialog,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileDialog,"message",_getMessage,_setMessage);
	rb_define_attr_method(rb_cWXFileDialog,"path",_getPath,_setPath);

	rb_define_method(rb_cWXFileDialog,"filenames",RUBY_METHOD_FUNC(_getFilenames),0);
	rb_define_method(rb_cWXFileDialog,"paths",RUBY_METHOD_FUNC(_getPaths),0);

	rb_define_module_function(rb_mWX,"load_dialog",RUBY_METHOD_FUNC(_loadFileSelector),-1);
	rb_define_module_function(rb_mWX,"save_dialog",RUBY_METHOD_FUNC(_saveFileSelector),-1);

	registerInfo<wxFileDialog>(rb_cWXFileDialog);
#endif
}



