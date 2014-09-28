/*
 * wxTextAttr.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxTextAttr.hpp"
#include "wxRichTextAttr.hpp"

VALUE rb_cWXRichTextAttr;

#if wxUSE_RICHTEXT

//find better way?
template <>
VALUE wrap< wxRichTextAttr >(wxRichTextAttr *attr )
{
	return wrapTypedPtr(attr, rb_cWXRichTextAttr);
}

template <>
wxRichTextAttr unwrap< wxRichTextAttr >(const VALUE &vattr)
{
	return *unwrapTypedPtr<wxRichTextAttr>(vattr,rb_cWXTextAttr);
}

#define _self unwrap<wxRichTextAttr*>(self)



namespace RubyWX {
namespace RichTextAttr {

APP_PROTECT(wxRichTextAttr)

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->Copy(unwrap<wxRichTextAttr>(other));
	return result;
}

}
}
#endif


DLL_LOCAL void Init_WXRichTextAttr(VALUE rb_mWX)
{
#if 0
	rb_cWXTextAttr = rb_define_class_under(rb_mWX,"TextAttr",rb_cObject);

#endif

#if wxUSE_RICHTEXT
	using namespace RubyWX::RichTextAttr;
	rb_cWXRichTextAttr = rb_define_class_under(rb_mWX,"RichTextAttr",rb_cWXTextAttr);
	rb_define_alloc_func(rb_cWXRichTextAttr,_alloc);

	rb_define_private_method(rb_cWXRichTextAttr,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	registerType<wxRichTextAttr>(rb_cWXRichTextAttr);

#endif

}
