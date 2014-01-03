/*
 * wxStaticBitmap.cpp
 *
 *  Created on: 09.03.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxStaticBitmap.hpp"

VALUE rb_cWXStaticBitmap;

#if wxUSE_STATBMP
#define _self unwrap<wxStaticBitmap*>(self)

namespace RubyWX {
namespace StaticBitmap {

APP_PROTECT(wxStaticBitmap)

/*
 * call-seq:
 *   StaticBitmap.new(parent, [options])
 *
 * creates a new StaticBitmap widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * bitmap WX::Bitmap
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
DLL_LOCAL void Init_WXStaticBitmap(VALUE rb_mWX)
{
#if 0
	rb_cWXStaticBitmapBase = rb_define_class_under(rb_mWX,"StaticBitmapBase",rb_cWXControl);
#endif
#if wxUSE_STATBMP
	using namespace RubyWX::StaticBitmap;
	rb_cWXStaticBitmap = rb_define_class_under(rb_mWX,"StaticBitmap",rb_cWXStaticBitmapBase);
	rb_define_alloc_func(rb_cWXStaticBitmap,_alloc);

	rb_define_method(rb_cWXStaticBitmap,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxStaticBitmap>(rb_cWXStaticBitmap);
#endif

}
