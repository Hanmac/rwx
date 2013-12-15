/*
 * wxStaticBitmapGeneric.cpp
 *
 *  Created on: 12.05.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxStaticBitmapGeneric.hpp"

VALUE rb_cWXStaticBitmapGeneric;

#if wxUSE_STATBMP
#define _self unwrap<wxGenericStaticBitmap*>(self)

namespace RubyWX {
namespace StaticBitmapGeneric {

APP_PROTECT(wxGenericStaticBitmap)

/*
 * call-seq:
 *   StaticBitmapGeneric.new(parent, [options])
 *
 * creates a new StaticBitmapGeneric widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * *bitmap WX::Bitmap
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	wxBitmap bitmap(wxNullBitmap);
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"bitmap",bitmap);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,bitmap);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXStaticBitmapGeneric(VALUE rb_mWX)
{
#if wxUSE_STATBMP
	using namespace RubyWX::StaticBitmapGeneric;
	rb_cWXStaticBitmapGeneric = rb_define_class_under(rb_mWX,"StaticBitmapGeneric",rb_cWXStaticBitmapBase);
	rb_define_alloc_func(rb_cWXStaticBitmapGeneric,_alloc);

	rb_define_method(rb_cWXStaticBitmapGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericStaticBitmap>(rb_cWXStaticBitmapGeneric);
#endif

}



