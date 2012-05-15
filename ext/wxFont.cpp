/*
 * wxFont.cpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#include "wxFont.hpp"
#include "wxApp.hpp"
#include "wxSize.hpp"

#define _self wrap<wxFont*>(self)

VALUE rb_cWXFont;

template <>
VALUE wrap< wxFontWeight >(const wxFontWeight &vmenu)
{
	switch(vmenu){
	case wxFONTWEIGHT_NORMAL:
		return ID2SYM(rb_intern("normal"));
	case wxFONTWEIGHT_LIGHT:
		return ID2SYM(rb_intern("light"));
	case wxFONTWEIGHT_BOLD:
		return ID2SYM(rb_intern("bold"));
	default:
		return ID2SYM(rb_intern("normal"));
	}
}


template <>
wxFontWeight wrap< wxFontWeight >(const VALUE &vmenu)
{
	if(NIL_P(vmenu))
		return wxFONTWEIGHT_NORMAL;
	ID id = SYM2ID(vmenu);
	if(id == rb_intern("normal"))
		return wxFONTWEIGHT_NORMAL;
	if(id == rb_intern("light"))
		return wxFONTWEIGHT_LIGHT;
	if(id == rb_intern("bold"))
		return wxFONTWEIGHT_BOLD;
	return wxFONTWEIGHT_NORMAL;
}

template <>
VALUE wrap< wxFontStyle >(const wxFontStyle &style)
{
	ID id;
	switch(style) {
	case wxFONTSTYLE_NORMAL:
		id = rb_intern("normal");
		break;
	case wxFONTSTYLE_ITALIC:
		id = rb_intern("italic");
		break;
	case wxFONTSTYLE_SLANT:
		id = rb_intern("slant");
		break;
	default:
		id = rb_intern("normal");
		break;
	}
	return ID2SYM(id);
}


template <>
wxFontStyle wrap< wxFontStyle >(const VALUE &vmenu)
{
	if(NIL_P(vmenu))
		return wxFONTSTYLE_NORMAL;
	ID id = SYM2ID(vmenu);
	if(id == rb_intern("normal"))
		return wxFONTSTYLE_NORMAL;
	if(id == rb_intern("italic"))
		return wxFONTSTYLE_ITALIC;
	if(id == rb_intern("slant"))
		return wxFONTSTYLE_SLANT;
	return wxFONTSTYLE_NORMAL;
}

template <>
VALUE wrap< wxFontFamily >(const wxFontFamily &style)
{
	return Qnil;
}

template <>
wxFontFamily wrap< wxFontFamily >(const VALUE &vmenu)
{
	if(NIL_P(vmenu))
		return wxFONTFAMILY_DEFAULT;
	ID id = SYM2ID(vmenu);
	if(id == rb_intern("default"))
		return wxFONTFAMILY_DEFAULT;
	if(id == rb_intern("decorative"))
		return wxFONTFAMILY_DECORATIVE;
	if(id == rb_intern("roman"))
		return wxFONTFAMILY_ROMAN;
	if(id == rb_intern("script"))
		return wxFONTFAMILY_SCRIPT;
	if(id == rb_intern("swiss"))
		return wxFONTFAMILY_SWISS;
	if(id == rb_intern("modern"))
		return wxFONTFAMILY_MODERN;
	if(id == rb_intern("teletype"))
		return wxFONTFAMILY_TELETYPE;
	return wxFONTFAMILY_DEFAULT;
}

template <>
wxFont nullPtr<wxFont>(){ return wxNullFont;}


namespace RubyWX {
namespace Font {

APP_PROTECT(wxFont)

macro_attr(PointSize,int)
macro_attr(PixelSize,wxSize)
macro_attr(Family,wxFontFamily)
macro_attr(Style,wxFontStyle)
macro_attr(Weight,wxFontWeight)

macro_attr(FaceName,wxString)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE size,family,style,weight;
	rb_scan_args(argc, argv, "13",&size,&family,&style,&weight);
	_self->Create(NUM2INT(size),
		wrap<wxFontFamily>(family),
		wrap<wxFontStyle>(style),
		wrap<wxFontWeight>(weight));

//	rb_call_super(argc,argv);
	return self;
}


}
}

DLL_LOCAL void Init_WXFont(VALUE rb_mWX)
{
	using namespace RubyWX::Font;
	rb_cWXFont = rb_define_class_under(rb_mWX,"Font",rb_cObject);
	rb_define_alloc_func(rb_cWXFont,_alloc);

	rb_define_method(rb_cWXFont,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_define_attr_method(rb_cWXFont,"pointSize",_getPointSize,_setPointSize);

	rb_define_attr_method(rb_cWXFont,"family",_getFamily,_setFamily);
	rb_define_attr_method(rb_cWXFont,"Style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXFont,"weight",_getWeight,_setWeight);
	rb_define_attr_method(rb_cWXFont,"faceName",_getFaceName,_setFaceName);

	registerInfo<wxFont>(rb_cWXFont);
}
