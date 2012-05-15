/*
 * wxFileDialog.cpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */


#include "wxFileDialog.hpp"

VALUE rb_cWXFileDialog;

#if wxUSE_FILEDLG
#define _self wrap<wxFileDialog*>(self)

namespace RubyWX {
namespace FileDialog {

APP_PROTECT(wxFileDialog)

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
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("wildcard")))))
			_self->SetWildcard(wrap<wxString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("message")))))
			_self->SetMessage(wrap<wxString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("directory")))))
			_self->SetDirectory(wrap<wxString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("filename")))))
			_self->SetFilename(wrap<wxString>(temp));
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("path")))))
			_self->SetPath(wrap<wxString>(temp));

	}

	return self;
}

macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr(Wildcard,wxString)
macro_attr(Message,wxString)
macro_attr(Path,wxString)

VALUE _getFilenames(VALUE self)
{
	wxArrayString result;
	_self->GetFilenames(result);
	return wrap(result);
}

VALUE _getPaths(VALUE self)
{
	wxArrayString result;
	_self->GetPaths(result);
	return wrap(result);
}


VALUE _saveFileSelector(int argc,VALUE *argv,VALUE self)
{
	VALUE what,extension,default_name,parent;
	rb_scan_args(argc, argv, "22",&what,&extension,&default_name,&parent);
	return wrap(wxSaveFileSelector(wrap<wxString>(what),
			wrap<wxString>(extension),
			wrap<wxString>(default_name),
			wrap<wxWindow*>(parent)));
}
VALUE _loadFileSelector(int argc,VALUE *argv,VALUE self)
{
	VALUE what,extension,default_name,parent;
	rb_scan_args(argc, argv, "22",&what,&extension,&default_name,&parent);
	return wrap(wxLoadFileSelector(wrap<wxString>(what),
			wrap<wxString>(extension),
			wrap<wxString>(default_name),
			wrap<wxWindow*>(parent)));
}

}
}
#endif
void Init_WXFileDialog(VALUE rb_mWX)
{
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



