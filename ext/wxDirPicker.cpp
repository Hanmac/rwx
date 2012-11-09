/*
 * wxDirPicker.cpp
 *
 *  Created on: 13.03.2012
 *      Author: hanmac
 */

#include "wxDirPicker.hpp"

VALUE rb_cWXDirPicker;
static VALUE rb_cWXFileDirPickerEvent;

#if wxUSE_DIRPICKERCTRL

namespace RubyWX {
namespace DirPicker {
#define _self unwrap<wxDirPickerCtrl*>(self)
macro_attr(Path,wxString)

APP_PROTECT(wxDirPickerCtrl)

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
void Init_WXDirPicker(VALUE rb_mWX)
{
#if wxUSE_DIRPICKERCTRL
	using namespace RubyWX::DirPicker;
	rb_cWXDirPicker = rb_define_class_under(rb_mWX,"DirPicker",rb_cWXControl);
	rb_define_alloc_func(rb_cWXDirPicker,_alloc);

	rb_define_method(rb_cWXDirPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDirPicker,"path",_getPath,_setPath);

	rb_cWXFileDirPickerEvent = rb_define_class_under(rb_cWXEvent,"FileDirPicker",rb_cWXEvent);
	registerEventType("dirpicker_changed",wxEVT_COMMAND_DIRPICKER_CHANGED,rb_cWXFileDirPickerEvent);

	rb_define_attr_method(rb_cWXFileDirPickerEvent,"path",
			Event::_getPath,Event::_setPath);

	registerInfo<wxDirPickerCtrl>(rb_cWXDirPicker);
#endif

}


