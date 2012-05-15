/*
 * wxStaticLine.cpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#include "wxStaticLine.hpp"

VALUE rb_cWXStaticLine;

#if wxUSE_STATLINE
#define _self wrap<wxStaticLine*>(self)

namespace RubyWX {
namespace StaticLine {

APP_PROTECT(wxStaticLine)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXStaticLine(VALUE rb_mWX)
{
#if wxUSE_STATLINE
	using namespace RubyWX::StaticLine;
	rb_cWXStaticLine = rb_define_class_under(rb_mWX,"StaticLine",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticLine,_alloc);

	rb_define_method(rb_cWXStaticLine,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxStaticLine>(rb_cWXStaticLine);
#endif

}
