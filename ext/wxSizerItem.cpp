/*
 * wxSizerItem.cpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */


#include "wxSizer.hpp"
#include "wxSizerItem.hpp"

#define _self wrap<wxSizerItem*>(self)

VALUE rb_cWXSizerItem;

namespace RubyWX {
namespace SizerItem {


}
}



void Init_WXSizerItem(VALUE rb_mWX)
{
	using namespace RubyWX::SizerItem;
	rb_cWXSizerItem = rb_define_class_under(rb_cWXSizer,"Item",rb_cObject);
	rb_undef_alloc_func(rb_cWXSizerItem);

	//rb_define_attr_method(rb_cWXSizerItem,"orientation",_getOrientation,_setOrientation);
}



