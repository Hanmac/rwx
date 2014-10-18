/*
 * wxInfoBarGeneric.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#include "wxInfoBarGeneric.hpp"
#include "wxInfoBar.hpp"

VALUE rb_cWXInfoBarGeneric;

#if wxUSE_INFOBAR && defined(wxHAS_NATIVE_INFOBAR)
#define _self unwrap<wxInfoBarGeneric*>(self)

namespace RubyWX {
namespace GenericInfoBar {

APP_PROTECT(wxInfoBarGeneric)

/*
 * call-seq:
 *   InfoBarGeneric.new(parent, name, [options])
 *   InfoBarGeneric.new(parent, [options])
 *
 * creates a new InfoBarGeneric widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(NIL_P(hash))
		name = hash;

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
		}
		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent),id);

	}
	
	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXInfoBarGeneric(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_cWXInfoBarBase = rb_define_class_under(rb_mWX,"InfoBarBase",rb_cWXControl);
#endif

#if wxUSE_INFOBAR
#ifdef wxHAS_NATIVE_INFOBAR
	using namespace RubyWX::GenericInfoBar;
	rb_cWXInfoBarGeneric = rb_define_class_under(rb_mWX,"InfoBarGeneric",rb_cWXInfoBarBase);
	rb_define_alloc_func(rb_cWXInfoBarGeneric,_alloc);

	rb_define_method(rb_cWXInfoBarGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxInfoBarGeneric>(rb_cWXInfoBarGeneric);
#else
	rb_define_const(rb_mWX,"InfoBarGeneric",rb_cWXInfoBar);
#endif
#endif

}



