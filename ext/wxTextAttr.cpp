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
#define _self unwrap<wxTextAttr*>(self)


//use this macro to automaticly check the Has Methods
#define macro_textattr(attr,type,w) \
DLL_LOCAL VALUE _get##attr(VALUE self)\
{ \
	return _self->Has##attr() ? w<type>(_self->Get##attr()) : Qnil;\
}\
\
DLL_LOCAL VALUE _set##attr(VALUE self,VALUE other)\
{\
	_self->Set##attr(un##w<type>(other));\
	return other;\
}



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
//macro_textattr(FontStyle,wxFontStyle,wrapenum)
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
//macro_textattr(PageBreak,bool,wrap)



}
}
#endif
void Init_WXTextAttr(VALUE rb_mWX)
{
#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextAttr;
	rb_cWXTextAttr = rb_define_class_under(rb_mWX,"TextAttr",rb_cObject);
	rb_define_alloc_func(rb_cWXTextAttr,_alloc);

	registerType<wxTextAttr>(rb_cWXTextAttr);
#endif

}
