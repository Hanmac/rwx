/*
 * wxFont.hpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#ifndef WXFONT_HPP_
#define WXFONT_HPP_

#include "main.hpp"

extern VALUE rb_cWXFont;
void Init_WXFont(VALUE rb_mWX);


template <>
inline VALUE wrap< wxFont >(wxFont *menu )
{
	if(menu)
		return Data_Wrap_Struct(rb_cWXFont, NULL, NULL, menu);
	return Qnil;
}

template <>
inline wxFont* wrap< wxFont* >(const VALUE &vmenu)
{
	return unwrapPtr<wxFont>(vmenu, rb_cWXFont);
}

template <>
inline wxFont wrap< wxFont >(const VALUE &vmenu)
{
	return *wrap<wxFont*>(vmenu);
}

template <>
inline VALUE wrap< wxFontWeight >(const wxFontWeight &vmenu)
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
inline wxFontWeight wrap< wxFontWeight >(const VALUE &vmenu)
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
inline wxFontStyle wrap< wxFontStyle >(const VALUE &vmenu)
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
inline wxFontFamily wrap< wxFontFamily >(const VALUE &vmenu)
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


#endif /* WXFONT_HPP_ */
