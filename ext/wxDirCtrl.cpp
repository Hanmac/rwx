/*
 * wxDirCtrl.cpp
 *
 *  Created on: 29.09.2014
 *      Author: hanmac
 */

#include "wxDirCtrl.hpp"
#include "wxFileCtrlBase.hpp"

VALUE rb_cWXDirCtrl;

#if wxUSE_DIRDLG

#if !defined(HAVE_WXDIRCTRL)
#define wxDirCtrl wxGenericDirCtrl
#endif

#define _self unwrap<wxDirCtrl*>(self)

namespace RubyWX {
namespace DirCtrl {

APP_PROTECT(wxDirCtrl)

macro_attr_with_func(Filter,wrap,unwrapWildCard)

macro_attr(Path,wxString)

singlereturn(GetFilterIndex)
singlereturn_array(GetFilePaths,wxArrayString)

/*
 * call-seq:
 *   DirCtrl.new(parent, name, [options])
 *   DirCtrl.new(parent, [options])
 *
 * creates a new DirCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * default_directory String default directory
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
#ifdef HAVE_CONST_WXDIRCTRL_DEFAULT_STYLE
		int style(wxDIRCTRL_DEFAULT_STYLE);
#else
		int style(wxDIRCTRL_3D_INTERNAL);
#endif
		wxString defaultDirectory(wxDirDialogDefaultFolderStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"default_directory",defaultDirectory);

			set_hash_flag_option(hash, "dir_only", wxDIRCTRL_DIR_ONLY, style);
			set_hash_flag_option(hash, "select_first", wxDIRCTRL_SELECT_FIRST, style);
			set_hash_flag_option(hash, "show_filters", wxDIRCTRL_SHOW_FILTERS, style);
			set_hash_flag_option(hash, "internal_3d", wxDIRCTRL_3D_INTERNAL, style);
			set_hash_flag_option(hash, "edit_labels", wxDIRCTRL_EDIT_LABELS, style);
			set_hash_flag_option(hash, "multiple", wxDIRCTRL_MULTIPLE, style);

		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				defaultDirectory,
				wxDefaultPosition,
				wxDefaultSize,style
			);
		}

	}

	rb_call_super(argc,argv);
	return self;
}



VALUE _setFilterIndex(VALUE self,VALUE other)
{
	rb_check_frozen(self);
	int filter(RB_NUM2INT(other));

	if(check_filter_index(filter,_self->GetFilter()))
		_self->SetFilterIndex(filter);

	return other;
}


}
}
#endif

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

/* Document-const: DIR_ONLY
 * Only allow directory viewing/selection, no files
 */
/* Document-const: SELECT_FIRST
 * When setting the default path, select the first file in the directory
 */
/* Document-const: SHOW_FILTERS
 * Show the filter list
 */
/* Document-const: INTERNAL_3D
 * Use 3D borders on internal controls
 */
/* Document-const: EDIT_LABELS
 * Editable labels
 */
/* Document-const: MULTIPLE
 * Allow multiple selection
 */

/* Document-attr: filter
 * the filter of the DirCtrl. String, raise an ArgumentError if filter has wrong format
 */
/* Document-attr: filter_index
 * the filter_index of the DirCtrl.
 * Can't be higher than filters in filter. Integer
 */
/* Document-attr: path
 * the path of the DirCtrl. String
 */
/* Document-attr: file_paths
 * the file paths of the DirCtrl. Array<String>
 */

DLL_LOCAL void Init_WXDirCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_DIRDLG
	using namespace RubyWX::DirCtrl;
	rb_cWXDirCtrl = rb_define_class_under(rb_mWX,"DirCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXDirCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXDirCtrl,"filter",1,1);
	rb_define_attr(rb_cWXDirCtrl,"path",1,1);
	rb_define_attr(rb_cWXDirCtrl,"filter_index",1,1);

	rb_define_attr(rb_cWXDirCtrl,"file_paths",1,0);
#endif

	rb_define_method(rb_cWXDirCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDirCtrl,"filter",_getFilter,_setFilter);
	rb_define_attr_method(rb_cWXDirCtrl,"path",_getPath,_setPath);
	rb_define_attr_method(rb_cWXDirCtrl,"filter_index",_GetFilterIndex,_setFilterIndex);

	rb_define_attr_method(rb_cWXDirCtrl,"file_paths",_GetFilePaths,NULL);

#ifndef HAVE_CONST_WXDIRCTRL_DEFAULT_STYLE
	rb_define_const(rb_cWXDirCtrl,"DEFAULT_STYLE",RB_INT2NUM(wxDIRCTRL_3D_INTERNAL));
#else
	rb_define_const(rb_cWXDirCtrl,"DEFAULT_STYLE",RB_INT2NUM(wxDIRCTRL_DEFAULT_STYLE));
#endif

	rb_define_const(rb_cWXDirCtrl,"DIR_ONLY",RB_INT2NUM(wxDIRCTRL_DIR_ONLY));
	rb_define_const(rb_cWXDirCtrl,"SELECT_FIRST",RB_INT2NUM(wxDIRCTRL_SELECT_FIRST));
	rb_define_const(rb_cWXDirCtrl,"SHOW_FILTERS",RB_INT2NUM(wxDIRCTRL_SHOW_FILTERS));
	rb_define_const(rb_cWXDirCtrl,"INTERNAL_3D",RB_INT2NUM(wxDIRCTRL_3D_INTERNAL));
	rb_define_const(rb_cWXDirCtrl,"EDIT_LABELS",RB_INT2NUM(wxDIRCTRL_EDIT_LABELS));
	rb_define_const(rb_cWXDirCtrl,"MULTIPLE",RB_INT2NUM(wxDIRCTRL_MULTIPLE));

	registerInfo<wxDirCtrl>(rb_cWXDirCtrl);
#endif

}




