/*
 * wxDirPickerWidget.cpp
 *
 *  Created on: 05.05.2014
 *      Author: hanmac
 */

#include "wxDirPickerWidget.hpp"
#include "wxButton.hpp"
#include "wxFileCtrlBase.hpp"
#include "wxDirPicker.hpp"
#include "wxFileDirPicker.hpp"
#include "wxFilePicker.hpp"

VALUE rb_cWXDirPickerWidget;
#if wxUSE_DIRPICKERCTRL
#define _self unwrap<wxDirPickerWidget*>(self)

namespace RubyWX {
namespace DirPickerWidget {

APP_PROTECT(wxDirPickerWidget)

macro_attr(Path,wxString)

/*
 * call-seq:
 *   DirPickerWidget.new(parent, name, [options])
 *   DirPickerWidget.new(parent, [options])
 *
 * creates a new DirPickerWidget widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		int style(wxDIRBTN_DEFAULT_STYLE);

		wxString label(wxDirPickerWidgetLabel);
		wxString path(wxEmptyString);
		wxString message(wxDirSelectorPromptStr);
		wxString wildCard(wxFileSelectorDefaultWildcardStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"path",path);
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"wildcard",wildCard,unwrapWildCard);

			DirPicker::set_style_flags(hash,style);

		}

		FileDirPicker::check_style_flags(style);

		_self->Create(
			unwrap<wxWindow*>(parent),id,label,path,
			message,wildCard,
			wxDefaultPosition,wxDefaultSize,style
		);

	}

	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

/* Document-attr: path
 * the path of the FilePickerWidget. String
 */

/* Document-const: MUST_EXIST
 *  Creates a picker which allows to select only existing directories.
 *  wxGTK control always adds this flag internally as it does not support its absence.
 */
/* Document-const: CHANGE_DIR
 *   Change current working directory on each user directory selection change.
 */
/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

DLL_LOCAL void Init_WXDirPickerWidget(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);

	rb_define_attr(rb_cWXDirPickerWidget,"path",1,1);

#endif

#if wxUSE_DIRPICKERCTRL
	using namespace RubyWX::DirPickerWidget;
	rb_cWXDirPickerWidget = rb_define_class_under(rb_mWX,"DirPickerWidget",rb_cWXButton);
	rb_define_alloc_func(rb_cWXDirPickerWidget,_alloc);

	rb_define_method(rb_cWXDirPickerWidget,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDirPickerWidget,"path",_getPath,_setPath);

	rb_define_const(rb_cWXDirPickerWidget,"DEFAULT_STYLE",INT2NUM(wxDIRBTN_DEFAULT_STYLE));
	rb_define_const(rb_cWXDirPickerWidget,"MUST_EXIST",INT2NUM(wxDIRP_DIR_MUST_EXIST));
	rb_define_const(rb_cWXDirPickerWidget,"CHANGE_DIR",INT2NUM(wxDIRP_CHANGE_DIR));

	registerInfo<wxDirPickerWidget>(rb_cWXDirPickerWidget);
#endif
}
