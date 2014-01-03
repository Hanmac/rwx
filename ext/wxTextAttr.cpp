/*
 * wxTextAttr.cpp
 *
 *  Created on: 27.04.2012
 *      Author: hanmac
 */

#include "wxTextAttr.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"

VALUE rb_cWXTextAttr;

#if wxUSE_TEXTCTRL

//find better way?
template <>
VALUE wrap< wxTextAttr >(wxTextAttr *color )
{
	return Data_Wrap_Struct(rb_cWXTextAttr, NULL, free, color);
}

template <>
wxTextAttr unwrap< wxTextAttr >(const VALUE &vcolor)
{
	return *unwrapPtr<wxTextAttr>(vcolor,rb_cWXTextAttr);
}

#define _self unwrap<wxTextAttr*>(self)


//use this macro to automatically check the Has Methods
#define macro_textattr2(attr,attr2,type,w) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	return _self->Has##attr2() ? w<type>(_self->Get##attr()) : Qnil;\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	_self->Set##attr(un##w<type>(other));\
	return other;\
}
#define macro_textattr(attr,type,w) macro_textattr2(attr,attr,type,w)


namespace RubyWX {
namespace TextAttr {

APP_PROTECT(wxTextAttr)

macro_textattr(TextColour,wxColour,wrap)
macro_textattr(BackgroundColour,wxColour,wrap)
macro_textattr(Alignment,wxTextAttrAlignment,wrapenum)

macro_textattr(Tabs,wxArrayInt,wrap)
macro_textattr(LeftIndent,int,wrap)
macro_textattr(RightIndent,int,wrap)

macro_textattr(FontSize,int,wrap)
//macro_textattr(FontPointSize,int,wrap)
//macro_textattr(FontPixelSize,int,wrap)
macro_textattr2(FontStyle,Font,wxFontStyle,wrapenum)
macro_textattr(FontWeight,wxFontWeight,wrapenum)
macro_textattr(FontFaceName,wxString,wrap)
macro_textattr(FontUnderlined,bool,wrap)
macro_textattr(FontStrikethrough,bool,wrap)
macro_textattr(FontEncoding,wxFontEncoding,wrapenum)
macro_textattr(FontFamily,wxFontFamily,wrapenum)

macro_textattr(Font,wxFont,wrap)

macro_textattr(CharacterStyleName,wxString,wrap)
macro_textattr(ParagraphStyleName,wxString,wrap)
macro_textattr(ListStyleName,wxString,wrap)
macro_textattr(ParagraphSpacingAfter,int,wrap)
macro_textattr(ParagraphSpacingBefore,int,wrap)
macro_textattr(LineSpacing,int,wrap)
macro_textattr(BulletStyle,wxTextAttrBulletStyle,wrapenum)
macro_textattr(BulletNumber,int,wrap)
macro_textattr(BulletText,wxString,wrap)
macro_attr(BulletFont,wxString)
macro_textattr(BulletName,wxString,wrap)
macro_textattr(URL,wxString,wrap)


}
}
#endif

/* Document-attr: text_color
 * the text color of the TextAttr, WX::Color
 */
/* Document-attr: background_color
 * the background color of the TextAttr, WX::Color
 */
/* Document-attr: alignment
 * the alignment of the TextAttr, Symbol
 */
/* Document-attr: tabs
 * the Tabs in the section of the TextAttr, [Integer*]
 */
/* Document-attr: left_indent
 * the left indent of the TextAttr, Integer
 */
/* Document-attr: right_indent
 * the right indent of the TextAttr, Integer
 */


/* Document-attr: font_size
 * the font size of the TextAttr, Integer
 */
/* Document-attr: font_family
 * the font family of the TextAttr, Symbol
 */
/* Document-attr: font_style
 * the font style of the TextAttr, Symbol
 */
/* Document-attr: font_weight
 * the font weight of the TextAttr, Symbol
 */
/* Document-attr: font_face_name
 * the font face name of the TextAttr, Symbol
 */
/* Document-attr: font_encoding
 * the font encoding of the TextAttr, Encoding
 */

DLL_LOCAL void Init_WXTextAttr(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXTextAttr,"text_color",1,1);
	rb_define_attr(rb_cWXTextAttr,"background_color",1,1);

	rb_define_attr(rb_cWXTextAttr,"alignment",1,1);

	rb_define_attr(rb_cWXTextAttr,"tabs",1,1);

	rb_define_attr(rb_cWXTextAttr,"left_indent",1,1);
	rb_define_attr(rb_cWXTextAttr,"right_indent",1,1);


	rb_define_attr(rb_cWXTextAttr,"font_size",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_style",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_weight",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_facename",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_underlined",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_strikethrough",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_encoding",1,1);
	rb_define_attr(rb_cWXTextAttr,"font_family",1,1);

	rb_define_attr(rb_cWXTextAttr,"font",1,1);

	rb_define_attr(rb_cWXTextAttr,"character_style_name",1,1);
	rb_define_attr(rb_cWXTextAttr,"paragraph_style_name",1,1);
	rb_define_attr(rb_cWXTextAttr,"list_style_name",1,1);
	rb_define_attr(rb_cWXTextAttr,"paragraph_spacing_after",1,1);
	rb_define_attr(rb_cWXTextAttr,"paragraph_Spacing_before",1,1);
	rb_define_attr(rb_cWXTextAttr,"line_spacing",1,1);

#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextAttr;
	rb_cWXTextAttr = rb_define_class_under(rb_mWX,"TextAttr",rb_cObject);
	rb_define_alloc_func(rb_cWXTextAttr,_alloc);


	rb_define_attr_method(rb_cWXTextAttr,"text_color",_getTextColour,_setTextColour);
	rb_define_attr_method(rb_cWXTextAttr,"background_color",_getBackgroundColour,_setBackgroundColour);

	rb_define_attr_method(rb_cWXTextAttr,"alignment",_getAlignment,_setAlignment);

	rb_define_attr_method(rb_cWXTextAttr,"tabs",_getTabs,_setTabs);

	rb_define_attr_method(rb_cWXTextAttr,"left_indent",_getLeftIndent,_setLeftIndent);
	rb_define_attr_method(rb_cWXTextAttr,"right_indent",_getRightIndent,_setRightIndent);


	rb_define_attr_method(rb_cWXTextAttr,"font_size",_getFontSize,_setFontSize);
	rb_define_attr_method(rb_cWXTextAttr,"font_style",_getFontStyle,_setFontStyle);
	rb_define_attr_method(rb_cWXTextAttr,"font_weight",_getFontWeight,_setFontWeight);
	rb_define_attr_method(rb_cWXTextAttr,"font_facename",_getFontFaceName,_setFontFaceName);
	rb_define_attr_method(rb_cWXTextAttr,"font_underlined",_getFontUnderlined,_setFontUnderlined);
	rb_define_attr_method(rb_cWXTextAttr,"font_strikethrough",_getFontStrikethrough,_setFontStrikethrough);
	rb_define_attr_method(rb_cWXTextAttr,"font_encoding",_getFontEncoding,_setFontEncoding);
	rb_define_attr_method(rb_cWXTextAttr,"font_family",_getFontFamily,_setFontFamily);

	rb_define_attr_method(rb_cWXTextAttr,"font",_getFont,_setFont);

	rb_define_attr_method(rb_cWXTextAttr,"character_style_name",_getCharacterStyleName,_setCharacterStyleName);
	rb_define_attr_method(rb_cWXTextAttr,"paragraph_style_name",_getParagraphStyleName,_setParagraphStyleName);
	rb_define_attr_method(rb_cWXTextAttr,"list_style_name",_getListStyleName,_setListStyleName);
	rb_define_attr_method(rb_cWXTextAttr,"paragraph_spacing_after",_getParagraphSpacingAfter,_setParagraphSpacingAfter);
	rb_define_attr_method(rb_cWXTextAttr,"paragraph_Spacing_before",_getParagraphSpacingBefore,_setParagraphSpacingBefore);
	rb_define_attr_method(rb_cWXTextAttr,"line_spacing",_getLineSpacing,_setLineSpacing);


	registerType<wxTextAttr>(rb_cWXTextAttr);

	registerEnum<wxTextAttrAlignment>("wxTextAttrAlignment")
		->add(wxTEXT_ALIGNMENT_DEFAULT,"default")
		->add(wxTEXT_ALIGNMENT_LEFT,"left")
		->add(wxTEXT_ALIGNMENT_CENTER,"center")
		->add(wxTEXT_ALIGNMENT_RIGHT,"right")
		->add(wxTEXT_ALIGNMENT_JUSTIFIED,"justified");

#endif

}
