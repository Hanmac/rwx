/*
 * wxFilePicker.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxFilePicker.hpp"
#include "wxFileDirPicker.hpp"
#include "wxFileCtrlBase.hpp"
#include "wxPickerBase.hpp"

VALUE rb_cWXFilePicker;

#if wxUSE_FILEPICKERCTRL

namespace RubyWX {
namespace FilePicker {
#define _self unwrap<wxFilePickerCtrl*>(self)

APP_PROTECT(wxFilePickerCtrl)

void set_style_flags(VALUE hash,int& style)
{
	if(set_hash_flag_option(hash,"open",wxFLP_OPEN,style))
		style &= ~(wxFLP_SAVE & wxFLP_OVERWRITE_PROMPT);
	if(set_hash_flag_option(hash,"save",wxFLP_SAVE,style))
		style &= ~(wxFLP_OPEN & wxFLP_FILE_MUST_EXIST);

	if(set_hash_flag_option(hash,"overwrite_prompt",wxFLP_OVERWRITE_PROMPT,style))
		style &= ~wxFLP_OPEN & wxFLP_SAVE;

	if(set_hash_flag_option(hash,"must_exist",wxFLP_FILE_MUST_EXIST,style))
		style &= ~wxFLP_SAVE & wxFLP_OPEN;

	set_hash_flag_option(hash,"change_dir",wxFLP_CHANGE_DIR,style);
}

/*
 * call-seq:
 *   FilePicker.new(parent, name, [options])
 *   FilePicker.new(parent, [options])
 *
 * creates a new FilePicker widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
 *   * wildcard String
 *   * use_textctrl true/false
 *   * small true/false
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxFLP_DEFAULT_STYLE);

		wxString path(wxEmptyString);
		wxString message(wxFileSelectorPromptStr);
		wxString wildCard(wxFileSelectorDefaultWildcardStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"path",path);
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"wildcard",wildCard,unwrapWildCard);

			PickerBase::set_style_flags(hash,style);

			set_style_flags(hash,style);

		}

		FileDirPicker::check_style_flags(style);

		_self->Create(
			unwrap<wxWindow*>(parent),id,path,
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

DLL_LOCAL void Init_WXFilePicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);

	rb_cWXFileDirPicker = rb_define_class_under(rb_mWX,"FileDirPicker",rb_cWXPickerBase);

#endif

#if wxUSE_FILEPICKERCTRL
	using namespace RubyWX::FilePicker;
	rb_cWXFilePicker = rb_define_class_under(rb_mWX,"FilePicker",rb_cWXFileDirPicker);
	rb_define_alloc_func(rb_cWXFilePicker,_alloc);

	rb_define_method(rb_cWXFilePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_const(rb_cWXFilePicker,"OPEN",INT2NUM(wxFLP_OPEN));
	rb_define_const(rb_cWXFilePicker,"SAVE",INT2NUM(wxFLP_SAVE));
	rb_define_const(rb_cWXFilePicker,"OVERWRITE_PROMPT",INT2NUM(wxFLP_OVERWRITE_PROMPT));
	rb_define_const(rb_cWXFilePicker,"MUST_EXIST",INT2NUM(wxFLP_FILE_MUST_EXIST));
	rb_define_const(rb_cWXFilePicker,"CHANGE_DIR",INT2NUM(wxFLP_CHANGE_DIR));

	rb_define_const(rb_cWXFilePicker,"DEFAULT_STYLE",INT2NUM(wxFLP_DEFAULT_STYLE));


	registerEventType("filepicker_changed",wxEVT_FILEPICKER_CHANGED,rb_cWXFileDirPickerEvent);

	registerInfo<wxFilePickerCtrl>(rb_cWXFilePicker);
#endif

}


