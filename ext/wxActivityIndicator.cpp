/*
 * wxActivityIndicator.cpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#include "wxActivityIndicator.hpp"

VALUE rb_cWXActivityIndicator;

#if wxUSE_ACTIVITYINDICATOR
#include <wx/activityindicator.h>

#define _self unwrap<wxActivityIndicator*>(self)

namespace RubyWX {
namespace ActivityIndicator {

APP_PROTECT(wxActivityIndicator)

singlefunc(Start)
singlefunc(Stop)
singlereturn(IsRunning)

/*
 * call-seq:
 *   ActivityIndicator.new(parent, name, [options])
 *   ActivityIndicator.new(parent, [options])
 *
 * creates a new ActivityIndicator widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		if(nil_check(parent))
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);

	}

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXActivityIndicator(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_ACTIVITYINDICATOR
	using namespace RubyWX::ActivityIndicator;
	rb_cWXActivityIndicator = rb_define_class_under(rb_mWX,"ActivityIndicator",rb_cWXControl);
	rb_define_alloc_func(rb_cWXActivityIndicator,_alloc);

	rb_define_method(rb_cWXActivityIndicator,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXActivityIndicator,"start",RUBY_METHOD_FUNC(_Start),0);
	rb_define_method(rb_cWXActivityIndicator,"stop",RUBY_METHOD_FUNC(_Stop),0);
	rb_define_method(rb_cWXActivityIndicator,"running?",RUBY_METHOD_FUNC(_IsRunning),0);

	registerInfo<wxActivityIndicator>(rb_cWXActivityIndicator);
#endif

}




