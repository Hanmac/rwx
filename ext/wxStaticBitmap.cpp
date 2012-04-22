/*
 * wxStaticBitmap.cpp
 *
 *  Created on: 09.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXStaticBitmap;

#if wxUSE_STATBMP
#define _self wrap<wxStaticBitmap*>(self)

namespace RubyWX {
namespace StaticBitmap {

macro_attr(Bitmap,wxBitmap)


VALUE _alloc(VALUE self)
{
	return wrap(new wxStaticBitmap(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxNullBitmap);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
void Init_WXStaticBitmap(VALUE rb_mWX)
{
#if wxUSE_STATBMP
	using namespace RubyWX::StaticBitmap;
	rb_cWXStaticBitmap = rb_define_class_under(rb_mWX,"StaticBitmap",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticBitmap,_alloc);

	rb_define_method(rb_cWXStaticBitmap,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXStaticBitmap,"bitmap",_getBitmap,_setBitmap);

#endif

}
