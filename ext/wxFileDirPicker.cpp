/*
 * wxFileDirPicker.cpp
 *
 *  Created on: 09.01.2013
 *      Author: hanmac
 */

#include "wxFileDirPicker.hpp"
#include "wxPickerBase.hpp"


VALUE rb_cWXFileDirPicker;
VALUE rb_cWXFileDirPickerEvent;

#if wxUSE_FILEPICKERCTRL || wxUSE_DIRPICKERCTRL


template <>
wxFileDirPickerCtrlBase* unwrap<wxFileDirPickerCtrlBase*>(const VALUE &arg)
{
	return unwrapPtr<wxFileDirPickerCtrlBase>(arg,rb_cWXFileDirPicker);
}

namespace RubyWX {
namespace FileDirPicker {
#define _self unwrap<wxFileDirPickerCtrlBase*>(self)

void check_style_flags(const int& style)
{
	if((style & wxFLP_OPEN) && (style & wxFLP_SAVE))
		rb_raise(rb_eArgError,"style can't have both OPEN and SAVE flags");

	if((style & wxFLP_OPEN) && (style & wxFLP_OVERWRITE_PROMPT))
		rb_raise(rb_eArgError,"style can't have both OVERWRITE_PROMPT and OPEN flags");

	if((style & wxFLP_SAVE) && (style & wxFLP_FILE_MUST_EXIST))
		rb_raise(rb_eArgError,"style can't have both MUST_EXIST and SAVE flags");

}

macro_attr(Path,wxString)

namespace Event {
#undef _self
#define _self unwrapPtr<wxFileDirPickerEvent>(self,rb_cWXFileDirPickerEvent)
macro_attr(Path,wxString)
}

}
}

#endif


/* Document-attr: path
 * the path of the FilePicker. String
 */

DLL_LOCAL void Init_WXFileDirPicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);


	rb_define_attr(rb_cWXFileDirPicker,"path",1,1);
	rb_define_attr(rb_cWXFileDirPickerEvent,"path",1,1);
#endif

#if wxUSE_FILEPICKERCTRL || wxUSE_DIRPICKERCTRL
	using namespace RubyWX::FileDirPicker;
	rb_cWXFileDirPicker = rb_define_class_under(rb_mWX,"FileDirPicker",rb_cWXPickerBase);
	rb_undef_alloc_func(rb_cWXFileDirPicker);

	rb_define_attr_method(rb_cWXFileDirPicker,"path",_getPath,_setPath);

	rb_cWXFileDirPickerEvent = rb_define_class_under(rb_cWXEvent,"FileDirPicker",rb_cWXEvent);

	rb_define_attr_method(rb_cWXFileDirPickerEvent,"path",
			Event::_getPath,Event::_setPath);

	registerInfo<wxFileDirPickerCtrlBase>(rb_cWXFileDirPicker);
#endif

}


