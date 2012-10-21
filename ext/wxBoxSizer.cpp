/*
 * wxBoxSizer.cpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#include "wxSizer.hpp"

#define _self unwrap<wxBoxSizer*>(self)

VALUE rb_cWXBoxSizer;

namespace RubyWX {
namespace BoxSizer {

VALUE _alloc(VALUE self)
{
	return wrapPtr(new wxBoxSizer(wxHORIZONTAL),self);
}

VALUE _getOrientation(VALUE self)
{
	return wrap(_self->GetOrientation() == wxVERTICAL);
}

VALUE _setOrientation(VALUE self,VALUE val)
{
	_self->SetOrientation(RTEST(val) ? wxVERTICAL : wxHORIZONTAL );
	return val;
}


}
}



void Init_WXBoxSizer(VALUE rb_mWX)
{
	using namespace RubyWX::BoxSizer;
	rb_cWXBoxSizer = rb_define_class_under(rb_mWX,"BoxSizer",rb_cWXSizer);
	rb_define_alloc_func(rb_cWXBoxSizer,_alloc);

	rb_define_attr_method(rb_cWXBoxSizer,"orientation",_getOrientation,_setOrientation);

	registerInfo<wxBoxSizer>(rb_cWXBoxSizer);
}
