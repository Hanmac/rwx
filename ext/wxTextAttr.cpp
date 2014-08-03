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
#define macro_textattr2(attr,attr2,type,flag,w) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	return _self->Has##attr2() ? w<type>(_self->Get##attr()) : Qnil;\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	if(NIL_P(other))\
		_self->RemoveFlag(wxTEXT_ATTR_##flag);\
	else\
		_self->Set##attr(un##w<type>(other));\
	return other;\
}
#define macro_textattr(attr,type,flag,w) macro_textattr2(attr,attr,type,flag,w)


namespace RubyWX {
namespace TextAttr {

APP_PROTECT(wxTextAttr)

macro_textattr(TextColour,wxColour,TEXT_COLOUR,wrap)
macro_textattr(BackgroundColour,wxColour,BACKGROUND_COLOUR,wrap)
macro_textattr(Alignment,wxTextAttrAlignment,ALIGNMENT,wrapenum)

macro_textattr(Tabs,wxArrayInt,TABS,wrap)
macro_textattr(LeftIndent,int,LEFT_INDENT,wrap)
macro_textattr(RightIndent,int,RIGHT_INDENT,wrap)

macro_textattr(FontSize,int,FONT_SIZE,wrap)
//macro_textattr(FontPointSize,int,wrap)
//macro_textattr(FontPixelSize,int,wrap)
macro_textattr2(FontStyle,Font,wxFontStyle,FONT_ITALIC,wrapenum)
macro_textattr(FontWeight,wxFontWeight,FONT_WEIGHT,wrapenum)
macro_textattr(FontFaceName,wxString,FONT_FACE,wrap)
macro_textattr(FontUnderlined,bool,FONT_UNDERLINE,wrap)
macro_textattr(FontStrikethrough,bool,FONT_STRIKETHROUGH,wrap)
macro_textattr(FontEncoding,wxFontEncoding,FONT_ENCODING,wrapenum)
macro_textattr(FontFamily,wxFontFamily,FONT_FAMILY,wrapenum)

macro_textattr(Font,wxFont,FONT,wrap)

macro_textattr(CharacterStyleName,wxString,CHARACTER_STYLE_NAME,wrap)
macro_textattr(ParagraphStyleName,wxString,PARAGRAPH_STYLE_NAME,wrap)
macro_textattr(ListStyleName,wxString,LIST_STYLE_NAME,wrap)
macro_textattr(ParagraphSpacingAfter,int,PARA_SPACING_AFTER,wrap)
macro_textattr(ParagraphSpacingBefore,int,PARA_SPACING_BEFORE,wrap)
macro_textattr(LineSpacing,wxTextAttrLineSpacing,LINE_SPACING,wrapenum)

macro_textattr(BulletStyle,wxTextAttrBulletStyle,BULLET_STYLE,wrapenum)
macro_textattr(BulletNumber,int,BULLET_NUMBER,wrap)
macro_textattr(BulletText,wxString,BULLET_TEXT,wrap)
macro_attr(BulletFont,wxString)
macro_textattr(BulletName,wxString,BULLET_NAME,wrap)

macro_textattr(URL,wxString,URL,wrap)


/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->Copy(unwrap<wxTextAttr>(other));
	return result;
}


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
/* Document-attr: font_facename
 * the font face name of the TextAttr, String
 */
/* Document-attr: font_underlined
 * true if the font is underlined. bool
 */
/* Document-attr: font_strikethrough
 * true if the font is strikethrough. bool
 */
/* Document-attr: font_encoding
 * the font encoding of the TextAttr, Encoding
 */

/* Document-attr: font
 * the font of the TextAttr, WX::Font
 */

/* Document-attr: character_style_name
 * the name of the character style. String
 */
/* Document-attr: paragraph_style_name
 * the name of the paragraph style. String
 */
/* Document-attr: list_style_name
 * the name of the list style. String
 */
/* Document-attr: paragraph_spacing_after
 * the space in tenths of a millimeter after the paragraph. Integer
 */
/* Document-attr: paragraph_Spacing_before
 * the space in tenths of a millimeter before the paragraph. Integer
 */
/* Document-attr: line_spacing
 * the bullet style of the TextAttr, Symbol
 */

/* Document-attr: bullet_style
 * the bullet style of the TextAttr, Symbol
 */
/* Document-attr: bullet_number
 * the bullet number of the TextAttr, Integer
 */
/* Document-attr: bullet_text
 * the bullet text of the TextAttr, String
 */
/* Document-attr: bullet_font
 * the bullet font of the TextAttr, String
 */
/* Document-attr: bullet_name
 * the bullet name of the TextAttr, String
 */

/* Document-attr: url
 * the URL of the TextAttr, String
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

	rb_define_attr(rb_cWXTextAttr,"bullet_style",1,1);
	rb_define_attr(rb_cWXTextAttr,"bullet_number",1,1);
	rb_define_attr(rb_cWXTextAttr,"bullet_text",1,1);
	rb_define_attr(rb_cWXTextAttr,"bullet_font",1,1);
	rb_define_attr(rb_cWXTextAttr,"bullet_name",1,1);

	rb_define_attr(rb_cWXTextAttr,"url",1,1);

#endif

#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextAttr;
	rb_cWXTextAttr = rb_define_class_under(rb_mWX,"TextAttr",rb_cObject);
	rb_define_alloc_func(rb_cWXTextAttr,_alloc);

	rb_define_private_method(rb_cWXTextAttr,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

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

	rb_define_attr_method(rb_cWXTextAttr,"bullet_style",_getBulletStyle,_setBulletStyle);
	rb_define_attr_method(rb_cWXTextAttr,"bullet_number",_getBulletNumber,_setBulletNumber);
	rb_define_attr_method(rb_cWXTextAttr,"bullet_text",_getBulletText,_setBulletText);
	rb_define_attr_method(rb_cWXTextAttr,"bullet_font",_getBulletFont,_setBulletFont);
	rb_define_attr_method(rb_cWXTextAttr,"bullet_name",_getBulletName,_setBulletName);

	rb_define_attr_method(rb_cWXTextAttr,"url",_getURL,_setURL);

	registerType<wxTextAttr>(rb_cWXTextAttr);

	registerEnum<wxTextAttrAlignment>("wxTextAttrAlignment")
		->add(wxTEXT_ALIGNMENT_DEFAULT,"default")
		->add(wxTEXT_ALIGNMENT_LEFT,"left")
		->add(wxTEXT_ALIGNMENT_CENTER,"center")
		->add(wxTEXT_ALIGNMENT_RIGHT,"right")
		->add(wxTEXT_ALIGNMENT_JUSTIFIED,"justified");

	registerEnum<wxTextAttrBulletStyle>("wxTextAttrBulletStyle")
		->add(wxTEXT_ATTR_BULLET_STYLE_NONE,"none")
		->add(wxTEXT_ATTR_BULLET_STYLE_ARABIC,"arabic")
		->add(wxTEXT_ATTR_BULLET_STYLE_LETTERS_UPPER,"letters_upper")
		->add(wxTEXT_ATTR_BULLET_STYLE_LETTERS_LOWER,"letters_lower")
		->add(wxTEXT_ATTR_BULLET_STYLE_ROMAN_UPPER,"roman_upper")
		->add(wxTEXT_ATTR_BULLET_STYLE_ROMAN_LOWER,"roman_lower")
		->add(wxTEXT_ATTR_BULLET_STYLE_SYMBOL,"symbol")
		->add(wxTEXT_ATTR_BULLET_STYLE_BITMAP,"bitmap")
		->add(wxTEXT_ATTR_BULLET_STYLE_PARENTHESES,"parentheses")
		->add(wxTEXT_ATTR_BULLET_STYLE_PERIOD,"period")
		->add(wxTEXT_ATTR_BULLET_STYLE_STANDARD,"standard")
		->add(wxTEXT_ATTR_BULLET_STYLE_RIGHT_PARENTHESIS,"right_parentheses")
		->add(wxTEXT_ATTR_BULLET_STYLE_OUTLINE,"outline")

		->add(wxTEXT_ATTR_BULLET_STYLE_ALIGN_LEFT,"align_left")
		->add(wxTEXT_ATTR_BULLET_STYLE_ALIGN_RIGHT,"align_right")
		->add(wxTEXT_ATTR_BULLET_STYLE_ALIGN_CENTRE,"align_center")

		->add(wxTEXT_ATTR_BULLET_STYLE_CONTINUATION,"continuation");

	registerEnum<wxTextAttrLineSpacing>("wxTextAttrLineSpacing",wxTEXT_ATTR_LINE_SPACING_NORMAL)
		->add(wxTEXT_ATTR_LINE_SPACING_NORMAL,"normal")
		->add(wxTEXT_ATTR_LINE_SPACING_HALF,"half")
		->add(wxTEXT_ATTR_LINE_SPACING_TWICE,"twice");



#endif

}
