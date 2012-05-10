/*
 * wxCategoryProperty.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */


#include "wxCategoryProperty.hpp"
#include "wxApp.hpp"


VALUE rb_cWXCategoryProperty;
#if wxUSE_PROPGRID
#define _self wrap<wxPropertyCategory*>(self)

namespace RubyWX {
namespace CategoryProperty {

APP_PROTECT(wxPropertyCategory);

}
}
#endif

DLL_LOCAL void Init_WXCategoryProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::CategoryProperty;
	rb_cWXCategoryProperty = rb_define_class_under(rb_mWX,"CategoryProperty",rb_cWXProperty);
	rb_define_alloc_func(rb_cWXCategoryProperty,_alloc);

	registerType<wxPropertyCategory>(rb_cWXCategoryProperty);
#endif
}


