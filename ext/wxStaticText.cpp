/*
 * wxStaticText.cpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */


#include "wxStaticText.hpp"

VALUE rb_cWXStaticText;

#if wxUSE_STATLINE
#define _self unwrap<wxStaticText*>(self)

namespace RubyWX {
namespace StaticText {

APP_PROTECT(wxStaticText)

/*
 * call-seq:
 *   StaticText.new(parent, [options])
 *
 * creates a new StaticText widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXStaticText(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif
#if wxUSE_STATLINE
	using namespace RubyWX::StaticText;
	rb_cWXStaticText = rb_define_class_under(rb_mWX,"StaticText",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticText,_alloc);

	rb_define_method(rb_cWXStaticText,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxStaticText>(rb_cWXStaticText);
#endif

}



