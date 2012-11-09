/*
 * wxFilePicker.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxFilePicker.hpp"

VALUE rb_cWXFilePicker;
static VALUE rb_cWXFileDirPickerEvent;

#if wxUSE_FILEPICKERCTRL

namespace RubyWX {
namespace FilePicker {
#define _self unwrap<wxFilePickerCtrl*>(self)
macro_attr(Path,wxString)

APP_PROTECT(wxFilePickerCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

namespace Event {
#undef _self
#define _self unwrapPtr<wxFileDirPickerEvent>(self,rb_cWXFileDirPickerEvent)
macro_attr(Path,wxString)
}

}
}

#endif
void Init_WXFilePicker(VALUE rb_mWX)
{
#if wxUSE_FILEPICKERCTRL
	using namespace RubyWX::FilePicker;
	rb_cWXFilePicker = rb_define_class_under(rb_mWX,"FilePicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXFilePicker,_alloc);

	rb_define_method(rb_cWXFilePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFilePicker,"path",_getPath,_setPath);

	rb_cWXFileDirPickerEvent = rb_define_class_under(rb_cWXEvent,"FileDirPicker",rb_cWXEvent);
	registerEventType("filepicker_changed",wxEVT_COMMAND_FILEPICKER_CHANGED,rb_cWXFileDirPickerEvent);

	rb_define_attr_method(rb_cWXFileDirPickerEvent,"path",
			Event::_getPath,Event::_setPath);

	registerInfo<wxFilePickerCtrl>(rb_cWXFilePicker);
#endif

}


