/*
 * wxTextAttr.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxRichTextStyle.hpp"
#include "wxRichTextAttr.hpp"

VALUE rb_cWXRichTextStyle;

#if wxUSE_RICHTEXT

template <>
wxRichTextStyleDefinition* unwrap< wxRichTextStyleDefinition* >(const VALUE &vcolor)
{
	return unwrapPtr<wxRichTextStyleDefinition>(vcolor,rb_cWXRichTextStyle);
}

#define _self unwrap<wxRichTextStyleDefinition*>(self)



namespace RubyWX {
namespace RichTextStyle {

macro_attr(Name,wxString)
macro_attr(Description,wxString)
macro_attr(BaseStyle,wxString)
macro_attr(Style,wxRichTextAttr)


/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->Copy(*unwrap<wxRichTextStyleDefinition*>(other));
	return result;
}

}
}
#endif


/* Document-attr: name
 * the name of the RichTextStyle. String
 */
/* Document-attr: description
 * the description of the RichTextStyle. String
 */
/* Document-attr: base_style
 * the base style of the RichTextStyle. String
 */
/* Document-attr: style
 * the style of the RichTextStyle. WX::RichTextAttr
 */

DLL_LOCAL void Init_WXRichTextStyle(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXRichTextStyle,"name",1,1);
	rb_define_attr(rb_cWXRichTextStyle,"description",1,1);
	rb_define_attr(rb_cWXRichTextStyle,"base_style",1,1);
	rb_define_attr(rb_cWXRichTextStyle,"style",1,1);
#endif

#if wxUSE_RICHTEXT
	using namespace RubyWX::RichTextStyle;
	rb_cWXRichTextStyle = rb_define_class_under(rb_mWX,"RichTextStyle",rb_cObject);
	rb_undef_alloc_func(rb_cWXRichTextStyle);

	rb_define_private_method(rb_cWXRichTextStyle,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXRichTextStyle,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXRichTextStyle,"description",_getDescription,_setDescription);
	rb_define_attr_method(rb_cWXRichTextStyle,"base_style",_getBaseStyle,_setBaseStyle);
	rb_define_attr_method(rb_cWXRichTextStyle,"style",_getStyle,_setStyle);

	registerInfo<wxRichTextStyleDefinition>(rb_cWXRichTextStyle);

#endif

}
