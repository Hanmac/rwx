/*
 * wxBitmap.cpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */


#include "wxBitmap.hpp"

#define _self wrap<wxBitmap*>(self)

VALUE rb_cWXBitmap;


namespace RubyWX {
namespace Bitmap {
VALUE _alloc(VALUE self) {
	return wrap(new wxBitmap);
}


}
}


void Init_WXBitmap(VALUE rb_mWX)
{
	using namespace RubyWX::Bitmap;
	rb_cWXBitmap = rb_define_class_under(rb_mWX,"Bitmap",rb_cObject);
	rb_define_alloc_func(rb_cWXBitmap,_alloc);
}
