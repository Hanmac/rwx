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

APP_PROTECT(wxContextHelpButton)

/*
 * call-seq:
 *   ContextHelpButton.new(parent, [options])
 *
 * creates a new ContextHelpButton widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!_created) {
		wxWindowID id(wxID_CONTEXT_HELP);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
		}

		_self->Create(unwrap<wxWindow*>(parent),id);
		_created = true;
	}

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

