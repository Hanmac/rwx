/*
 * wxFileCtrlBase.cpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#include "wxFileCtrlBase.hpp"

VALUE rb_cWXFileCtrlBase,rb_cWXFileCtrlEvent;

wxString unwrapWildCard(const VALUE &val)
{
	wxString wildcard(unwrap<wxString>(val));
	wxArrayString wild,desc;

	if(!wxParseCommonDialogsFilter(wildcard,wild,desc)){
		rb_raise(rb_eArgError,"'%s' is not a valid wildcard",unwrap<char*>(val));
		return wxEmptyString;
	}
	return wildcard;

}

#if wxUSE_FILECTRL
template <>
wxFileCtrlBase* unwrap<wxFileCtrlBase*>(const VALUE &arg)
{
	return unwrapPtr<wxFileCtrlBase>(arg,rb_cWXFileCtrlBase);
}

#define _self unwrap<wxFileCtrlBase*>(self)

namespace RubyWX {
namespace FileCtrlBase {

macro_attr_with_func(Wildcard,wrap,unwrapWildCard)
macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr(Path,wxString)

macro_attr(FilterIndex,int)


void set_style_flags(VALUE hash,int& flags)
{

	set_hash_flag_option(hash,"open",wxFC_OPEN,flags);
	set_hash_flag_option(hash,"save",wxFC_SAVE,flags);
	set_hash_flag_option(hash,"multiple",wxFC_MULTIPLE,flags);
	set_hash_flag_option(hash,"no_show_hidden",wxFC_NOSHOWHIDDEN,flags);

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
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option_func(wildcard,Wildcard,unwrapWildCard)
		set_option(directory,Directory,wxString)
		set_option(filename,Filename,wxString)
		set_option(path,Path,wxString)

		set_option(filter_index,FilterIndex,int)
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
 * the wildcard of the FileCtrl. String
 */
/* Document-attr: directory
 * the directory of the FileCtrl. String
 */
/* Document-attr: filename
 * the filename of the FileCtrl. String
 */
/* Document-attr: filter_index
 * the filter_index of the FileCtrl. Integer
 */


DLL_LOCAL void Init_WXFileCtrlBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);

	rb_define_attr(rb_cWXFileCtrlBase,"wildcard",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"directory",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"filename",1,1);
	rb_define_attr(rb_cWXFileCtrlBase,"filter_index",1,1);

	rb_define_attr(rb_cWXFileCtrlEvent,"directory",1,1);
	rb_define_attr(rb_cWXFileCtrlEvent,"filter_index",1,1);

	rb_define_attr(rb_cWXFileCtrlEvent,"files",1,1);

#endif

#if wxUSE_FILECTRL
	using namespace RubyWX::FileCtrlBase;
	rb_cWXFileCtrlBase = rb_define_class_under(rb_mWX,"FileCtrlBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXFileCtrlBase);

	rb_define_method(rb_cWXFileCtrlBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFileCtrlBase,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileCtrlBase,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filter_index",_getFilterIndex,_setFilterIndex);

	rb_define_const(rb_cWXFileCtrlBase,"OPEN",INT2NUM(wxFC_OPEN));
	rb_define_const(rb_cWXFileCtrlBase,"SAVE",INT2NUM(wxFC_SAVE));
	rb_define_const(rb_cWXFileCtrlBase,"MULTIPLE",INT2NUM(wxFC_MULTIPLE));
	rb_define_const(rb_cWXFileCtrlBase,"NOSHOWHIDDEN",INT2NUM(wxFC_NOSHOWHIDDEN));

	rb_cWXFileCtrlEvent = rb_define_class_under(rb_cWXEvent,"FileCtrl",rb_cWXEvent);
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



