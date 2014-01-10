/*
 * wxContextHelpButton.cpp
 *
 *  Created on: 22.12.2013
 *      Author: hanmac
 */

#include "wxContextHelpButton.hpp"

VALUE rb_cWXContextHelpButton;
#if wxUSE_HELP && wxUSE_BMPBUTTON
#define _self unwrap<wxContextHelpButton*>(self)

namespace RubyWX {
namespace ContextHelpButton {

#ifdef HAVE_WXCONTEXTHELPBUTTON
APP_PROTECT(wxContextHelpButton)
#else
APP_PROTECT_NULL
#endif

/*
 * call-seq:
 *   ContextHelpButton.new(parent, name, [options])
 *   ContextHelpButton.new(parent, [options])
 *
 * creates a new ContextHelpButton widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

#ifdef HAVE_WXCONTEXTHELPBUTTON
	if(!_created && rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_CONTEXT_HELP);
		int style(wxBU_AUTODRAW);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);
		
	}
#else
	wxWindowID id(wxID_CONTEXT_HELP);
	int style(wxBU_AUTODRAW);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_hash_option(hash,"id",id,unwrapID);
		set_hash_option(hash,"style",style);
	}

	DATA_PTR(self) = new wxContextHelpButton(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);
	
#endif


	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXContextHelpButton(VALUE rb_mWX)
{
#if 0
	rb_cWXButton = rb_define_class_under(rb_mWX,"Button",rb_cWXAnyButton);
	rb_cWXBitmapButton = rb_define_class_under(rb_mWX,"BitmapButton",rb_cWXButton);
#endif

#if wxUSE_HELP && wxUSE_BMPBUTTON
	using namespace RubyWX::ContextHelpButton;
	rb_cWXContextHelpButton = rb_define_class_under(rb_mWX,"ContextHelpButton",rb_cWXBitmapButton);
	rb_define_alloc_func(rb_cWXContextHelpButton,_alloc);

	rb_define_method(rb_cWXContextHelpButton,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxContextHelpButton>(rb_cWXContextHelpButton);
#endif
}

