/*
 * wxFileCtrlGeneric.cpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#include "wxFileCtrlGeneric.hpp"
#include "wxFileCtrlBase.hpp"
#include "wxFileCtrl.hpp"

VALUE rb_cWXFileCtrlGeneric;

#if wxUSE_FILECTRL
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
#define _self unwrap<wxGenericFileCtrl*>(self)

namespace RubyWX {
namespace FileCtrlGeneric {

APP_PROTECT(wxFileCtrl)

/*
 * call-seq:
 *   FileCtrlGeneric.new(parent, name, [options])
 *   FileCtrlGeneric.new(parent, [options])
 *
 * creates a new FileCtrlGeneric widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * default_wildcard String default WildCard
 *   * default_directory String default directory
 *   * default_filename String default filename
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxFC_DEFAULT_STYLE);

		wxString defaultDirectory(wxEmptyString);
		wxString defaultFilename(wxEmptyString);
		wxString defaultWildCard(wxFileSelectorDefaultWildcardStr);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			FileCtrlBase::_set_options(hash, id, style, defaultDirectory, defaultFilename, defaultWildCard);

			FileCtrlBase::set_style_flags(hash,style);
		}
		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent), id,
				defaultDirectory, defaultFilename,
				defaultWildCard, style
			);

	}

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
#endif


DLL_LOCAL void Init_WXFileCtrlGeneric(VALUE rb_mWX)
{

#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXFileCtrlBase = rb_define_class_under(rb_mWX,"FileCtrlBase",rb_cWXControl);
#endif

#if wxUSE_FILECTRL
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
	using namespace RubyWX::FileCtrlGeneric;
	rb_cWXFileCtrlGeneric = rb_define_class_under(rb_mWX,"FileCtrlGeneric",rb_cWXFileCtrlBase);
	rb_define_alloc_func(rb_cWXFileCtrlGeneric,_alloc);

	rb_define_method(rb_cWXFileCtrlGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericFileCtrl>(rb_cWXFileCtrlGeneric);
#else
	rb_define_const(rb_mWX,"FileCtrlGeneric",rb_cWXFileCtrl);
#endif
#endif

}

