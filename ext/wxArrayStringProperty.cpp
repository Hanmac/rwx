/*
 * wxArrayStringProperty.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxArrayStringProperty.hpp"
#include "wxApp.hpp"

VALUE rb_cWXArrayStringProperty;
#if wxUSE_PROPGRID
#define _self unwrap<wxArrayStringProperty*>(self)

namespace RubyWX {
namespace ArrayStringProperty {

APP_PROTECT(wxArrayStringProperty);

}
}
#endif

DLL_LOCAL void Init_WXArrayStringProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::ArrayStringProperty;
	rb_cWXArrayStringProperty = rb_define_class_under(rb_mWX,"ArrayStringProperty",rb_cWXProperty);
	rb_define_alloc_func(rb_cWXArrayStringProperty,_alloc);

	registerInfo<wxArrayStringProperty>(rb_cWXArrayStringProperty);
#endif
}
