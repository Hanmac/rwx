/*
 * wxFontProperty.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxFontProperty.hpp"
#include "wxApp.hpp"


VALUE rb_cWXFontProperty;
#if wxUSE_PROPGRID
#define _self unwrap<wxFontProperty*>(self)

namespace RubyWX {
namespace FontProperty {

APP_PROTECT(wxFontProperty);

}
}
#endif

DLL_LOCAL void Init_WXFontProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::FontProperty;
	rb_cWXFontProperty = rb_define_class_under(rb_mWX,"FontProperty",rb_cWXProperty);
	rb_define_alloc_func(rb_cWXFontProperty,_alloc);

	registerInfo<wxFontProperty>(rb_cWXFontProperty);
#endif
}



