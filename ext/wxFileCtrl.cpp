/*
 * wxFileCtrl.cpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

VALUE rb_cWXFileCtrl,rb_cWXFileCtrlEvent;

#if wxUSE_FONTPICKERCTRL
#define _self wrap<wxFileCtrl*>(self)

namespace RubyWX {
namespace FileCtrl {

macro_attr(Wildcard,wxString)
macro_attr(Directory,wxString)
macro_attr(Filename,wxString)
macro_attr(Path,wxString)

macro_attr(FilterIndex,int)


VALUE _alloc(VALUE self)
{
	return wrap(new wxFileCtrl(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	rb_call_super(argc,argv);
	return self;
}

namespace Event
{
#undef _self
#define _self unwrapPtr<wxFileCtrlEvent>(self,rb_cWXFileCtrlEvent)

macro_attr(Directory,wxString)
macro_attr(Files,wxArrayString)
macro_attr(FilterIndex,int)

singlereturn(GetFile)

}

}
}
#endif
void Init_WXFileCtrl(VALUE rb_mWX)
{
#if wxUSE_FONTPICKERCTRL
	using namespace RubyWX::FileCtrl;
	rb_cWXFileCtrl = rb_define_class_under(rb_mWX,"FileCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXFileCtrl,_alloc);

	rb_define_method(rb_cWXFileCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFileCtrl,"wildcard",_getWildcard,_setWildcard);
	rb_define_attr_method(rb_cWXFileCtrl,"directory",_getDirectory,_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrl,"filename",_getFilename,_setFilename);
	rb_define_attr_method(rb_cWXFileCtrl,"filter_index",_getFilterIndex,_setFilterIndex);

	rb_cWXFileCtrlEvent = rb_define_class_under(rb_cWXEvent,"FileCtrl",rb_cWXEvent);
	registerEventType("filectrl_selectionchanged",wxEVT_FILECTRL_SELECTIONCHANGED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_fileactivated",wxEVT_FILECTRL_FILEACTIVATED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_folderchanged",wxEVT_FILECTRL_FOLDERCHANGED,rb_cWXFileCtrlEvent);
	registerEventType("filectrl_filterchanged",wxEVT_FILECTRL_FILTERCHANGED,rb_cWXFileCtrlEvent);

	rb_define_attr_method(rb_cWXFileCtrlEvent,"directory",Event::_getDirectory,Event::_setDirectory);
	rb_define_attr_method(rb_cWXFileCtrlEvent,"filter_index",Event::_getFilterIndex,Event::_setFilterIndex);

	rb_define_attr_method(rb_cWXFileCtrlEvent,"files",Event::_getFiles,Event::_setFiles);
#endif

}




