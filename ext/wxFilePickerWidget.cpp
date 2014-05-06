/*
 * wxFilePickerWidget.cpp
 *
 *  Created on: 05.05.2014
 *      Author: hanmac
 */

#include "wxFilePickerWidget.hpp"
#include "wxButton.hpp"
#include "wxFileCtrlBase.hpp"
#include "wxFilePicker.hpp"
#include "wxFileDirPicker.hpp"

VALUE rb_cWXFilePickerWidget;
#if wxUSE_FILEPICKERCTRL
#define _self unwrap<wxFilePickerWidget*>(self)

namespace RubyWX {
namespace FilePickerWidget {

APP_PROTECT(wxFilePickerWidget)

macro_attr(Path,wxString)

/*
 * call-seq:
 *   FilePickerWidget.new(parent, name, [options])
 *   FilePickerWidget.new(parent, [options])
 *
 * creates a new FilePickerWidget widget.
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
		int style(wxFILEBTN_DEFAULT_STYLE);

		wxString label(wxFilePickerWidgetLabel);
		wxString path(wxEmptyString);
		wxString message(wxFileSelectorPromptStr);
		wxString wildCard(wxFileSelectorDefaultWildcardStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"path",path);
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"wildcard",wildCard,unwrapWildCard);

			FilePicker::set_style_flags(hash,style);

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

/* Document-const: OPEN
 * creates control in "open" mode
 */
/* Document-const: SAVE
 * creates control in "save" mode
 */
/* Document-const: OVERWRITE_PROMPT
 *  Can be combined with SAVE only: ask confirmation to the user before selecting a file.
 */
/* Document-const: MUST_EXIST
 * Can be combined with OPEN only: the selected file must be an existing file.
 */
/* Document-const: CHANGE_DIR
 *  Change current working directory on each user file selection change.
 */
/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

DLL_LOCAL void Init_WXFilePickerWidget(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXAnyButton = rb_define_class_under(rb_mWX,"AnyButton",rb_cWXControl);

	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);

	rb_define_attr(rb_cWXFilePickerWidget,"path",1,1);

#endif

#if wxUSE_BMPBUTTON
	using namespace RubyWX::FilePickerWidget;
	rb_cWXFilePickerWidget = rb_define_class_under(rb_mWX,"FilePickerWidget",rb_cWXButton);
	rb_define_alloc_func(rb_cWXFilePickerWidget,_alloc);

	rb_define_method(rb_cWXFilePickerWidget,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFilePickerWidget,"path",_getPath,_setPath);

	rb_define_const(rb_cWXFilePickerWidget,"OPEN",INT2NUM(wxFLP_OPEN));
	rb_define_const(rb_cWXFilePickerWidget,"SAVE",INT2NUM(wxFLP_SAVE));
	rb_define_const(rb_cWXFilePickerWidget,"OVERWRITE_PROMPT",INT2NUM(wxFLP_OVERWRITE_PROMPT));
	rb_define_const(rb_cWXFilePickerWidget,"MUST_EXIST",INT2NUM(wxFLP_FILE_MUST_EXIST));
	rb_define_const(rb_cWXFilePickerWidget,"CHANGE_DIR",INT2NUM(wxFLP_CHANGE_DIR));

	rb_define_const(rb_cWXFilePickerWidget,"DEFAULT_STYLE",INT2NUM(wxFILEBTN_DEFAULT_STYLE));

	registerInfo<wxFilePickerWidget>(rb_cWXFilePickerWidget);
#endif
}
