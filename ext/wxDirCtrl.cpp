/*
 * wxDirCtrl.cpp
 *
 *  Created on: 29.09.2014
 *      Author: hanmac
 */

#include "wxDirCtrl.hpp"

VALUE rb_cWXDirCtrl;

#if wxUSE_DIRDLG
#define _self unwrap<wxDirCtrl*>(self)

namespace RubyWX {
namespace DirCtrl {

APP_PROTECT(wxDirCtrl)

/*
 * call-seq:
 *   DirCtrl.new(parent, name, [options])
 *   DirCtrl.new(parent, [options])
 *
 * creates a new DirCtrl widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * default_directory String default directory
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
#ifdef HAVE_CONST_WXDIRCTRL_DEFAULT_STYLE
		int style(wxDIRCTRL_DEFAULT_STYLE);
#else
		int style(wxDIRCTRL_3D_INTERNAL);
#endif
		wxString defaultDirectory(wxDirDialogDefaultFolderStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"default_directory",defaultDirectory);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				defaultDirectory,
				wxDefaultPosition,
				wxDefaultSize,style
			);
		}

	}

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif

DLL_LOCAL void Init_WXDirCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_DIRDLG
	using namespace RubyWX::DirCtrl;
	rb_cWXDirCtrl = rb_define_class_under(rb_mWX,"DirCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXDirCtrl,_alloc);

	rb_define_method(rb_cWXDirCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxFileCtrl>(rb_cWXDirCtrl);
#endif

}




