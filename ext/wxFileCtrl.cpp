/*
 * wxFileCtrl.cpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#include "wxFileCtrl.hpp"

VALUE rb_cWXFileCtrl;

#if wxUSE_FILECTRL
#define _self unwrap<wxFileCtrl*>(self)

namespace RubyWX {
namespace FileCtrl {

APP_PROTECT(wxFileCtrl)

/*
 * call-seq:
 *   FileCtrl.new(parent, name, [options])
 *   FileCtrl.new(parent, [options])
 *
 * creates a new FileCtrl widget.
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
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_option(hash,"default_directory",defaultDirectory);
			set_hash_option(hash,"default_filename",defaultFilename);
			set_hash_option(hash,"default_wildcard",defaultWildCard,unwrapWildCard);

			FileCtrlBase::set_style_flags(hash,style);

		}

		_self->Create(unwrap<wxWindow*>(parent),id,defaultDirectory,defaultFilename,defaultWildCard,style);

	}

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXFileCtrl(VALUE rb_mWX)
{
#if wxUSE_FILECTRL
	using namespace RubyWX::FileCtrl;
	rb_cWXFileCtrl = rb_define_class_under(rb_mWX,"FileCtrl",rb_cWXFileCtrlBase);
	rb_define_alloc_func(rb_cWXFileCtrl,_alloc);

	rb_define_method(rb_cWXFileCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxFileCtrl>(rb_cWXFileCtrl);
#endif

}




