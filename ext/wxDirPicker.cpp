/*
 * wxDirPicker.cpp
 *
 *  Created on: 13.03.2012
 *      Author: hanmac
 */

#include "wxDirPicker.hpp"
#include "wxFilePicker.hpp"
#include "wxFileDirPicker.hpp"
#include "wxPickerBase.hpp"

VALUE rb_cWXDirPicker;

#if wxUSE_DIRPICKERCTRL

namespace RubyWX {
namespace DirPicker {
#define _self unwrap<wxDirPickerCtrl*>(self)

APP_PROTECT(wxDirPickerCtrl)


void set_style_flags(VALUE hash,int& style)
{
	set_hash_flag_option(hash,"must_exist",wxDIRP_DIR_MUST_EXIST,style);
	set_hash_flag_option(hash,"change_dir",wxDIRP_CHANGE_DIR,style);
}

/*
 * call-seq:
 *   DirPicker.new(parent, name, [options])
 *   DirPicker.new(parent, [options])
 *
 * creates a new DirPicker widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
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
		int style(wxDIRP_DEFAULT_STYLE);

		wxString path(wxEmptyString);
		wxString message(wxDirSelectorPromptStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"path",path);
			set_hash_option(hash,"message",message);

			PickerBase::set_style_flags(hash,style);

			set_style_flags(hash,style);

		}

		FileDirPicker::check_style_flags(style);
		if(nil_check(parent))
			_self->Create(
				unwrap<wxWindow*>(parent),id,path,
				message,wxDefaultPosition,wxDefaultSize,style
			);

	}
	rb_call_super(argc,argv);

	return self;
}


}
}

#endif


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

DLL_LOCAL void Init_WXDirPicker(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXPickerBase = rb_define_class_under(rb_mWX,"PickerBase",rb_cWXControl);

	rb_cWXFileDirPicker = rb_define_class_under(rb_mWX,"FileDirPicker",rb_cWXPickerBase);

#endif

#if wxUSE_DIRPICKERCTRL
	using namespace RubyWX::DirPicker;
	rb_cWXDirPicker = rb_define_class_under(rb_mWX,"DirPicker",rb_cWXFileDirPicker);
	rb_define_alloc_func(rb_cWXDirPicker,_alloc);

	rb_define_method(rb_cWXDirPicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_const(rb_cWXDirPicker,"DEFAULT_STYLE",RB_INT2NUM(wxDIRP_DEFAULT_STYLE));
	rb_define_const(rb_cWXDirPicker,"MUST_EXIST",RB_INT2NUM(wxDIRP_DIR_MUST_EXIST));
	rb_define_const(rb_cWXDirPicker,"CHANGE_DIR",RB_INT2NUM(wxDIRP_CHANGE_DIR));

	registerEventType("dirpicker_changed",wxEVT_DIRPICKER_CHANGED,rb_cWXFileDirPickerEvent);

	registerInfo<wxDirPickerCtrl>(rb_cWXDirPicker);
#endif

}


