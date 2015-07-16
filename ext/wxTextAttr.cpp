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

namespace RubyWX {
namespace TextAttr {

void _from_hash(VALUE hash, wxTextAttr *attr);

}
}

//find better way?
template <>
VALUE wrap< wxTextAttr >(wxTextAttr *attr )
{
	return wrapTypedPtr(attr, rb_cWXTextAttr);
}

template <>
wxTextAttr unwrap< wxTextAttr >(const VALUE &vattr)
{
	if(rb_obj_is_kind_of(vattr, rb_cHash)) {
		wxTextAttr result;
		RubyWX::TextAttr::_from_hash(vattr, &result);
		return result;
	}
	return *unwrapTypedPtr<wxTextAttr>(vattr,rb_cWXTextAttr);
}

#define _self unwrap<wxTextAttr*>(self)


//use this macro to automatically check the Has Methods
#define macro_textattr3(attr, attr2, attr3, type, flag, w) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	return _self->Has##attr2() ? w<type>(_self->Get##attr3()) : Qnil;\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	rb_check_frozen(self);\
	if(NIL_P(other))\
		_self->RemoveFlag(wxTEXT_ATTR_##flag);\
	else\
		_self->Set##attr(un##w<type>(other));\
	return other;\
}

#define macro_textattr2(attr,attr2,type,flag,w) macro_textattr3(attr,attr2,attr,type,flag,w)
#define macro_textattr2a(attr,attr2,type,flag,w) macro_textattr3(attr,attr,attr2,type,flag,w)
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
macro_textattr2a(FontPointSize, FontSize, int,FONT_POINT_SIZE, wrap)
macro_textattr2a(FontPixelSize, FontSize, int,FONT_PIXEL_SIZE, wrap)
macro_textattr2(FontStyle,FontItalic,wxFontStyle,FONT_ITALIC,wrapenum)
macro_textattr(FontWeight,wxFontWeight,FONT_WEIGHT,wrapenum)
macro_textattr(FontFaceName,wxString,FONT_FACE,wrap)
macro_textattr(FontUnderlined,bool,FONT_UNDERLINE,wrap)
macro_textattr(FontStrikethrough,bool,FONT_STRIKETHROUGH,wrap)
//macro_textattr(FontEncoding,wxFontEncoding,FONT_ENCODING,wrapenum)
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
macro_textattr2(BulletFont,BulletText, wxString, BULLET_TEXT, wrap)
macro_textattr(BulletName,wxString,BULLET_NAME,wrap)

macro_textattr(URL,wxString,URL,wrap)

singlereturn(IsCharacterStyle)
singlereturn(IsParagraphStyle)


void _from_hash(VALUE hash, wxTextAttr *attr)
{
	int left_indent;
	wxFont font;
	wxTextAttrBulletStyle bullet_style;
	wxTextAttrLineSpacing line_spacing;

	set_obj_option(hash, "text_color", &wxTextAttr::SetTextColour, attr);
	set_obj_option(hash, "background_color", &wxTextAttr::SetBackgroundColour, attr);
	set_obj_option(hash, "alignment", &wxTextAttr::SetAlignment, attr, unwrapenum<wxTextAttrAlignment>);

	set_obj_option(hash, "tabs", &wxTextAttr::SetTabs, attr);

	if(set_hash_option(hash, "left_indent", left_indent))
		attr->SetLeftIndent(left_indent);

	set_obj_option(hash, "right_indent", &wxTextAttr::SetRightIndent, attr);

	set_obj_option(hash, "font_size", &wxTextAttr::SetFontSize, attr);

	set_obj_option(hash, "font_point_size", &wxTextAttr::SetFontPointSize, attr);
	set_obj_option(hash, "font_pixel_size", &wxTextAttr::SetFontPixelSize, attr);

	set_obj_option(hash, "font_weight", &wxTextAttr::SetFontWeight, attr, unwrapenum<wxFontWeight>);
	set_obj_option(hash, "font_face_name", &wxTextAttr::SetFontFaceName, attr);
	set_obj_option(hash, "font_underlined", &wxTextAttr::SetFontUnderlined, attr);
	set_obj_option(hash, "font_strikethrough", &wxTextAttr::SetFontStrikethrough, attr);
	set_obj_option(hash, "font_family", &wxTextAttr::SetFontFamily, attr, unwrapenum<wxFontFamily>);

	if(set_hash_option(hash, "font", font))
		attr->SetFont(font);

	set_obj_option(hash, "character_style_name", &wxTextAttr::SetCharacterStyleName, attr);
	set_obj_option(hash, "paragraph_style_name", &wxTextAttr::SetParagraphStyleName, attr);
	set_obj_option(hash, "list_style_name", &wxTextAttr::SetListStyleName, attr);

	set_obj_option(hash, "paragraph_spacing_after", &wxTextAttr::SetParagraphSpacingAfter, attr);
	set_obj_option(hash, "paragraph_Spacing_before", &wxTextAttr::SetParagraphSpacingBefore, attr);

	if(set_hash_option(hash, "line_spacing", line_spacing, unwrapenum<wxTextAttrLineSpacing>))
		attr->SetLineSpacing(line_spacing);
	
	if(set_hash_option(hash, "bullet_style", bullet_style, unwrapenum<wxTextAttrBulletStyle>))
		attr->SetBulletStyle(bullet_style);

	set_obj_option(hash, "bullet_number", &wxTextAttr::SetBulletNumber, attr);

	set_obj_option(hash, "bullet_text", &wxTextAttr::SetBulletText, attr);

	set_obj_option(hash, "bullet_font", &wxTextAttr::SetBulletFont, attr);
	set_obj_option(hash, "bullet_name", &wxTextAttr::SetBulletName, attr);

	set_obj_option(hash, "url", &wxTextAttr::SetURL, attr);

}

/*
 * call-seq:
 *   TextAttr.new(hash)
 *
 * Creates a new TextAttr object.
*/
DLL_LOCAL VALUE _initialize(VALUE self,VALUE hash)
{
	_from_hash(hash, _self);
	return self;
}

/* Document-method: initialize_copy
 * call-seq:
 *   initialize_copy(orig)
 *
 * Duplicate an object
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_self->Copy(unwrap<wxTextAttr>(other));
	return result;
}


/*
 * call-seq:
 *   hash -> Fixnum
 *
 * Generates a Fixnum hash value for this object.
 *
 *
 */
DLL_LOCAL VALUE _getHash(VALUE self)
{
	st_index_t h = rb_hash_start(0);

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getTextColour(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBackgroundColour(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getAlignment(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getTabs(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getLeftIndent(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getRightIndent(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontSize(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontPointSize(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontPixelSize(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontStyle(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontWeight(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontFaceName(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontUnderlined(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontStrikethrough(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFontFamily(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getFont(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getCharacterStyleName(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getParagraphStyleName(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getListStyleName(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getParagraphSpacingAfter(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getParagraphSpacingBefore(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getLineSpacing(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBulletStyle(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBulletNumber(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBulletText(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBulletFont(self))));
	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getBulletName(self))));

	h = rb_hash_uint(h, NUM2LONG(rb_hash(_getURL(self))));

	h = rb_hash_end(h);
	return LONG2FIX(h);
}



struct equal_obj {
	wxTextAttr* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(*obj->self == unwrap<wxTextAttr>(obj->other));
}

VALUE _equal_rescue(VALUE val)
{
	return Qfalse;
}

/*
 * call-seq:
 *   == attr -> bool
 *
 * compares two TextAttr.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}



/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
	VALUE ary = rb_ary_new();

	rb_ary_push(ary, _getTextColour(self));
	rb_ary_push(ary, _getBackgroundColour(self));
	rb_ary_push(ary, _getAlignment(self));

	rb_ary_push(ary, _getTabs(self));
	rb_ary_push(ary, _getLeftIndent(self));
	rb_ary_push(ary, _getRightIndent(self));

	//rb_ary_push(ary, _getFontSize(self));

	rb_ary_push(ary, _getFontPointSize(self));
	rb_ary_push(ary, _getFontPixelSize(self));
	rb_ary_push(ary, _getFontStyle(self));
	rb_ary_push(ary, _getFontWeight(self));
	rb_ary_push(ary, _getFontFaceName(self));

	rb_ary_push(ary, _getFontUnderlined(self));
	rb_ary_push(ary, _getFontStrikethrough(self));

	rb_ary_push(ary, _getFontFamily(self));

	//rb_ary_push(ary, _getFont(self));

	rb_ary_push(ary, _getCharacterStyleName(self));
	rb_ary_push(ary, _getParagraphStyleName(self));
	rb_ary_push(ary, _getListStyleName(self));

	rb_ary_push(ary, _getParagraphSpacingAfter(self));
	rb_ary_push(ary, _getParagraphSpacingBefore(self));
	rb_ary_push(ary, _getLineSpacing(self));

	rb_ary_push(ary, _getBulletStyle(self));
	rb_ary_push(ary, _getBulletNumber(self));
	rb_ary_push(ary, _getBulletText(self));
	rb_ary_push(ary, _getBulletFont(self));
	rb_ary_push(ary, _getBulletName(self));

	rb_ary_push(ary, _getURL(self));

	return ary;
}

#define __marshal_load_helper(m) 	if(!NIL_P(val = rb_ary_shift(data))) {\
		_set##m(self, val);\
	}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self,VALUE data)
{
	data = rb_Array(data);
	VALUE val;

	__marshal_load_helper(TextColour)
	__marshal_load_helper(BackgroundColour)
	__marshal_load_helper(Alignment)

	__marshal_load_helper(Tabs)
	__marshal_load_helper(LeftIndent)
	__marshal_load_helper(RightIndent)

	//__marshal_load_helper(FontSize)

	__marshal_load_helper(FontPointSize)
	__marshal_load_helper(FontPixelSize)
	__marshal_load_helper(FontStyle)
	__marshal_load_helper(FontWeight)
	__marshal_load_helper(FontFaceName)

	__marshal_load_helper(FontUnderlined)
	__marshal_load_helper(FontStrikethrough)

	__marshal_load_helper(FontFamily)

	//__marshal_load_helper(Font)

	__marshal_load_helper(CharacterStyleName)
	__marshal_load_helper(ParagraphStyleName)
	__marshal_load_helper(ListStyleName)

	__marshal_load_helper(ParagraphSpacingAfter)
	__marshal_load_helper(ParagraphSpacingBefore)
	__marshal_load_helper(LineSpacing)

	__marshal_load_helper(BulletStyle)
	__marshal_load_helper(BulletNumber)
	__marshal_load_helper(BulletText)
	__marshal_load_helper(BulletFont)
	__marshal_load_helper(BulletName)

	__marshal_load_helper(URL)

	return self;
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

	rb_define_method(rb_cWXTextAttr,"initialize",RUBY_METHOD_FUNC(_initialize),1);
	rb_define_private_method(rb_cWXTextAttr,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXTextAttr,"text_color",_getTextColour,_setTextColour);
	rb_define_attr_method(rb_cWXTextAttr,"background_color",_getBackgroundColour,_setBackgroundColour);

	rb_define_attr_method(rb_cWXTextAttr,"alignment",_getAlignment,_setAlignment);

	rb_define_attr_method(rb_cWXTextAttr,"tabs",_getTabs,_setTabs);

	rb_define_attr_method(rb_cWXTextAttr,"left_indent",_getLeftIndent,_setLeftIndent);
	rb_define_attr_method(rb_cWXTextAttr,"right_indent",_getRightIndent,_setRightIndent);


	rb_define_attr_method(rb_cWXTextAttr,"font_size",_getFontSize,_setFontSize);
	rb_define_attr_method(rb_cWXTextAttr,"font_point_size",_getFontPointSize,_setFontPointSize);
	rb_define_attr_method(rb_cWXTextAttr,"font_pixel_size",_getFontPixelSize,_setFontPixelSize);

	rb_define_attr_method(rb_cWXTextAttr,"font_style",_getFontStyle,_setFontStyle);
	rb_define_attr_method(rb_cWXTextAttr,"font_weight",_getFontWeight,_setFontWeight);
	rb_define_attr_method(rb_cWXTextAttr,"font_facename",_getFontFaceName,_setFontFaceName);
	rb_define_attr_method(rb_cWXTextAttr,"font_underlined",_getFontUnderlined,_setFontUnderlined);
	rb_define_attr_method(rb_cWXTextAttr,"font_strikethrough",_getFontStrikethrough,_setFontStrikethrough);
//	rb_define_attr_method(rb_cWXTextAttr,"font_encoding",_getFontEncoding,_setFontEncoding);
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

	rb_define_method(rb_cWXTextAttr,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cWXTextAttr,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cWXTextAttr,"eql?","==");

	rb_define_method(rb_cWXTextAttr,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXTextAttr,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

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
