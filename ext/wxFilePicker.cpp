/*
 * wxFilePicker.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxFilePicker.hpp"
#include "wxFileDirPicker.hpp"
#include "wxFileCtrlBase.hpp"

VALUE rb_cWXFilePicker;

#if wxUSE_FILEPICKERCTRL

namespace RubyWX {
namespace FilePicker {
#define _self unwrap<wxFilePickerCtrl*>(self)

APP_PROTECT(wxFilePickerCtrl)

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

			set_hash_flag_option(hash,"use_textctrl",wxPB_USE_TEXTCTRL,style);
			set_hash_flag_option(hash,"small",wxPB_SMALL,style);

		}

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

DLL_LOCAL void Init_WXFilePicker(VALUE rb_mWX)
{
#if 0
	rb_cWXFileDirPicker = rb_define_class_under(rb_mWX,"FileDirPicker",rb_cWXPickerBase);

#endif

#if wxUSE_FILEPICKERCTRL
	using namespace RubyWX::FilePicker;
	rb_cWXFilePicker = rb_define_class_under(rb_mWX,"FilePicker",rb_cWXFileDirPicker);
	rb_define_alloc_func(rb_cWXFilePicker,_alloc);

	rb_define_method(rb_cWXFilePicker,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerEventType("filepicker_changed",wxEVT_FILEPICKER_CHANGED,rb_cWXFileDirPickerEvent);

	registerInfo<wxFilePickerCtrl>(rb_cWXFilePicker);
#endif

}


