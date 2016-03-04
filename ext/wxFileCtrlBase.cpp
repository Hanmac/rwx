/*
 * wxFileCtrlBase.cpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#include "wxFileCtrlBase.hpp"

#include "wxFileCtrl.hpp"
#include "wxFileCtrlGeneric.hpp"

#include "wxCommandEvent.hpp"

VALUE rb_cWXFileCtrlBase,rb_cWXFileCtrlEvent;

wxString unwrapWildCard(const VALUE &val)
{
	VALUE tmp = val;
	if(rb_obj_is_kind_of(tmp, rb_cArray)) {
		tmp = rb_ary_join(tmp, rb_str_new2("|"));
	}

	wxString wildcard(unwrap<wxString>(tmp));


	wxArrayString wild,desc;

	if(!wxParseCommonDialogsFilter(wildcard,wild,desc)){
		rb_raise(rb_eArgError,
			"'%" PRIsVALUE "' is not a valid wildcard",
			RB_OBJ_STRING(tmp)
		);
		return wxEmptyString;
	}
	return wildcard;
}

bool check_filter_index(int& filter,const wxString& wildcard)
{
	wxArrayString wild,desc;
	return check_index(filter,wxParseCommonDialogsFilter(wildcard,wild,desc));
}

#if wxUSE_FILECTRL
template <>
wxFileCtrlBase* unwrap<wxFileCtrlBase*>(const VALUE &arg)
{
	if(rb_obj_is_kind_of(arg,rb_cWXFileCtrl))
		return unwrap<wxFileCtrl*>(arg);
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
	if(rb_obj_is_kind_of(arg,rb_cWXFileCtrlGeneric))
		return unwrap<wxGenericFileCtrl*>(arg);
#endif
	return unwrapTypedPtr<wxFileCtrlBase>(arg,rb_cWXFileCtrlBase);
}

#define _self unwrap<wxFileCtrlBase*>(self)

namespace RubyWX {
namespace FileCtrlBase {

macro_attr_with_func(Wildcard,wrap,unwrapWildCard)
macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr(Path,wxString)

singlereturn_array(GetPaths,wxArrayString)
singlereturn_array(GetFilenames,wxArrayString)

singlereturn(GetFilterIndex)


void _set_options(VALUE hash,
		wxWindowID &id,
		int &style,
		wxString &defaultDirectory,
		wxString &defaultFilename,
		wxString &defaultWildCard
)
{
	set_hash_option(hash,"id",id,unwrapID);
	set_hash_option(hash,"style",style);

	set_hash_option(hash,"default_directory",defaultDirectory);
	set_hash_option(hash,"default_filename",defaultFilename);
	set_hash_option(hash,"default_wildcard",defaultWildCard,unwrapWildCard);

}

void set_style_flags(VALUE hash,int& flags)
{

	set_hash_flag_option(hash,"open",wxFC_OPEN,flags);
	set_hash_flag_option(hash,"save",wxFC_SAVE,flags);
	set_hash_flag_option(hash,"multiple",wxFC_MULTIPLE,flags);
	set_hash_flag_option(hash,"no_show_hidden",wxFC_NOSHOWHIDDEN,flags);

}

void check_style_flags(const int& style)
{
	if((style & wxFC_SAVE) && (style & wxFC_MULTIPLE))
		rb_raise(rb_eArgError,"style can't have both MULTIPLE and SAVE flags");
}

VALUE _setFilterIndex(VALUE self,VALUE other)
{
	rb_check_frozen(self);
	int filter(NUM2INT(other));

	if(check_filter_index(filter,_self->GetWildcard()))
		_self->SetFilterIndex(filter);

	return other;
}


/*
 * call-seq:
 *   FileCtrlBase.new(parent, [options])
 *
 * creates a new FileCtrlBase widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * wildcard String default WildCard
 *   * directory String default directory
 *   * filename String default filename
 *   * path String default path
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(name,rb_cString) && rb_obj_is_kind_of(hash,rb_cHash))
	{

		set_obj_option(hash,"wildcard", &wxFileCtrlBase::SetWildcard, _self, unwrapWildCard);

		set_obj_option(hash,"directory", &wxFileCtrlBase::SetDirectory, _self);
		set_obj_option(hash,"filename", &wxFileCtrlBase::SetFilename, _self);
		set_obj_option(hash,"path", &wxFileCtrlBase::SetPath, _self);

		set_ruby_option(hash, "filter_index", _setFilterIndex, self);
	}
	return self;
}

namespace Event
{
#undef _self
#define _self unwrap<wxFileCtrlEvent*>(self)

macro_attr(Directory,wxString)
macro_attr(Files,wxArrayString)
macro_attr(FilterIndex,int)

singlereturn(GetFile)

}

}
}
#endif

/* Document-attr: wildcard
 * the wildcard of the FileCtrl. String, raise an ArgumentError if wildcard has wrong format
 */
/* Document-attr: filter_index
 * the filter_index of the FileCtrl.
 * Can't be higher than filters in wildcard. Integer
 */
/* Document-attr: directory
 * the directory of the FileCtrl. String
 */
/* Document-attr: filename
 * the filename of the FileCtrl. String
 */
/* Document-attr: path
 * the path of the FileCtrl. String
 */
/* Document-attr: filenames
 * the filenames of the FileCtrl. Array<String>
 */
/* Document-attr: paths
 * the paths of the FileCtrl. Array<String>
 */

/* Document-const: OPEN
 * creates control in "open" mode
 */
/* Document-const: SAVE
 * creates control in "save" mode
 */
/* Document-const: MULTIPLE
 * allows to select multiple files
 */
/* Document-const: NOSHOWHIDDEN
 * hides files that are marked hidden.
 */
/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */
/* Document-const: DEFAULT_WILDCARD
 * default wildcard for any files.
 */

DLL_LOCAL void Init_WXFileCtrlBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_cWXCommandEvent = rb_define_class_under(rb_cWXEvent,"Command",rb_cWXEvent);
#endif


#if wxUSE_FILECTRL
	using namespace RubyWX::FileCtrlBase;
	rb_cWXFileCtrlBase = rb_define_class_under(rb_mWX,"FileCtrlBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXFileCtrlBase);

	rb_cWXFileCtrlEvent = rb_define_class_under(rb_cWXEvent,"FileCtrl",rb_cWXCommandEvent);

#if 0
	rb_define_attr(rb_cWXFileCtrlBase,"wildcard",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"directory",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"filename",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"path",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"filter_index",1,1);

	rb_define_attr(rb_cWXFileCtrlBase,"filenames",1,0);
	rb_define_attr(rb_cWXFileCtrlBase,"paths",1,0);


	rb_define_attr(rb_cWXFileCtrlEvent,"directory",1,1);
	rb_define_attr(rb_cWXFileCtrlEvent,"filter_index",1,1);

	rb_define_attr(rb_cWXFileCtrlEvent,"files",1,1);

#endif

	rb_define_method(rb_cWXFileCtrlBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFileCtrlBase,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileCtrlBase,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileCtrlBase,"path",_getPath,_setPath);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filter_index",_GetFilterIndex,_setFilterIndex);

	rb_define_attr_method(rb_cWXFileCtrlBase,"filenames",_GetFilenames,NULL);
	rb_define_attr_method(rb_cWXFileCtrlBase,"paths",_GetPaths,NULL);

	rb_define_const(rb_cWXFileCtrlBase,"OPEN",INT2NUM(wxFC_OPEN));
	rb_define_const(rb_cWXFileCtrlBase,"SAVE",INT2NUM(wxFC_SAVE));
	rb_define_const(rb_cWXFileCtrlBase,"MULTIPLE",INT2NUM(wxFC_MULTIPLE));
	rb_define_const(rb_cWXFileCtrlBase,"NOSHOWHIDDEN",INT2NUM(wxFC_NOSHOWHIDDEN));

	rb_define_const(rb_cWXFileCtrlBase,"DEFAULT_STYLE",INT2NUM(wxFC_DEFAULT_STYLE));
	rb_define_const(rb_cWXFileCtrlBase,"DEFAULT_WILDCARD",wrap(wxString(wxFileSelectorDefaultWildcardStr)));

	registerEventType("filectrl_selectionchanged",wxEVT_FILECTRL_SELECTIONCHANGED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_fileactivated",wxEVT_FILECTRL_FILEACTIVATED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_folderchanged",wxEVT_FILECTRL_FOLDERCHANGED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_filterchanged",wxEVT_FILECTRL_FILTERCHANGED,rb_cWXFileCtrlEvent);

	rb_define_attr_method(rb_cWXFileCtrlEvent,"directory",Event::_getDirectory,Event::_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrlEvent,"filter_index",Event::_getFilterIndex,Event::_setFilterIndex);

	rb_define_attr_method(rb_cWXFileCtrlEvent,"files",Event::_getFiles,Event::_setFiles);

	registerType<wxFileCtrlBase>(rb_cWXFileCtrlBase);
#endif

}



