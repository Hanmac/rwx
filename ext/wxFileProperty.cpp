/*
 * wxFileProperty.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxFileProperty.hpp"
#include "wxApp.hpp"


VALUE rb_cWXFileProperty;
#if wxUSE_PROPGRID
#define _self unwrap<wxFileProperty*>(self)

namespace RubyWX {
namespace FileProperty {

APP_PROTECT(wxFileProperty);

}
}
#endif

DLL_LOCAL void Init_WXFileProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::FileProperty;
	rb_cWXFileProperty = rb_define_class_under(rb_mWX,"FileProperty",rb_cWXProperty);
	rb_define_alloc_func(rb_cWXFileProperty,_alloc);

	registerInfo<wxFileProperty>(rb_cWXFileProperty);
#endif
}
