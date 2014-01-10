/*
 * wxStaticLine.cpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#include "wxStaticLine.hpp"

VALUE rb_cWXStaticLine;

#if wxUSE_STATLINE
#define _self unwrap<wxStaticLine*>(self)

namespace RubyWX {
namespace StaticLine {

APP_PROTECT(wxStaticLine)

/*
 * call-seq:
 *   StaticLine.new(parent, name, [options])
 *   StaticLine.new(parent, [options])
 *
 * creates a new StaticLine widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxLI_HORIZONTAL);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,wxDefaultPosition,wxDefaultSize,style);
	}
	
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXStaticLine(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif
#if wxUSE_STATLINE
	using namespace RubyWX::StaticLine;
	rb_cWXStaticLine = rb_define_class_under(rb_mWX,"StaticLine",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticLine,_alloc);

	rb_define_method(rb_cWXStaticLine,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxStaticLine>(rb_cWXStaticLine);
#endif

}
