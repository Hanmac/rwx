/*
 * wxStringProperty.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxStringProperty.hpp"
#include "wxApp.hpp"


VALUE rb_cWXStringProperty;
#if wxUSE_PROPGRID
#define _self unwrap<wxStringProperty*>(self)

namespace RubyWX {
namespace StringProperty {

APP_PROTECT(wxStringProperty);

}
}
#endif

DLL_LOCAL void Init_WXStringProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::StringProperty;
	rb_cWXStringProperty = rb_define_class_under(rb_mWX,"StringProperty",rb_cWXProperty);
	rb_define_alloc_func(rb_cWXStringProperty,_alloc);

	registerInfo<wxStringProperty>(rb_cWXStringProperty);
#endif
}



