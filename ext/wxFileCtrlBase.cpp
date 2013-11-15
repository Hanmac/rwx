/*
 * wxFileCtrlBase.cpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#include "wxFileCtrlBase.hpp"

VALUE rb_cWXFileCtrlBase,rb_cWXFileCtrlEvent;

#if wxUSE_FILECTRL
template <>
wxFileCtrlBase* unwrap<wxFileCtrlBase*>(const VALUE &arg)
{
	return unwrapPtr<wxFileCtrlBase>(arg,rb_cWXFileCtrlBase);
}

#define _self unwrap<wxFileCtrlBase*>(self)

namespace RubyWX {
namespace FileCtrlBase {

macro_attr(Wildcard,wxString)
macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr(Path,wxString)

macro_attr(FilterIndex,int)

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
void Init_WXFileCtrlBase(VALUE rb_mWX)
{
#if wxUSE_FILECTRL
	using namespace RubyWX::FileCtrlBase;
	rb_cWXFileCtrlBase = rb_define_class_under(rb_mWX,"FileCtrlBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXFileCtrlBase);

	rb_define_attr_method(rb_cWXFileCtrlBase,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileCtrlBase,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileCtrlBase,"filter_index",_getFilterIndex,_setFilterIndex);

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



