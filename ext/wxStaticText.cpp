/*
 * wxStaticText.cpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXStaticText;

#if wxUSE_STATLINE
#define _self wrap<wxStaticText*>(self)

namespace RubyWX {
namespace StaticText {

VALUE _alloc(VALUE self)
{
	return wrap(new wxStaticText(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXStaticText(VALUE rb_mWX)
{
#if wxUSE_STATLINE
	using namespace RubyWX::StaticText;
	rb_cWXStaticText = rb_define_class_under(rb_mWX,"StaticText",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticText,_alloc);

	rb_define_method(rb_cWXStaticText,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#endif

}



