/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#ifndef WXPEN_HPP_
#define WXPEN_HPP_

#include "main.hpp"

extern VALUE rb_cWXPen;
void Init_WXPen(VALUE rb_mWX);

template <>
inline VALUE wrap< wxPen >(wxPen *bitmap )
{
	if(bitmap->IsOk())
		return wrap(bitmap,rb_cWXPen);
	return Qnil;
}

template <>
inline VALUE wrap< wxPen >(const wxPen &bitmap )
{
	if(bitmap.IsOk())
		return wrap(const_cast<wxPen*>(&bitmap),rb_cWXPen);
	return Qnil;
}

template <>
inline VALUE wrap< wxPenStyle >(const wxPenStyle &style )
{
	ID id;
	switch(style)
	{
	case wxPENSTYLE_INVALID:
		return Qnil;
		break;
	case wxPENSTYLE_SOLID:
		id = rb_intern("solid");
		break;
	case wxPENSTYLE_DOT:
		id = rb_intern("dot");
		break;
	case wxPENSTYLE_LONG_DASH:
		id = rb_intern("long_dash");
		break;
	case wxPENSTYLE_SHORT_DASH:
		id = rb_intern("short_dash");
		break;
	case wxPENSTYLE_DOT_DASH:
		id = rb_intern("dot_dash");
		break;
	case wxPENSTYLE_USER_DASH:
		id = rb_intern("user_dash");
		break;
	case wxPENSTYLE_TRANSPARENT:
		id = rb_intern("transparent");
		break;
	case wxPENSTYLE_STIPPLE_MASK_OPAQUE:
		id = rb_intern("stipple_mask_opaque");
		break;
	case wxPENSTYLE_STIPPLE_MASK:
		id = rb_intern("stipple_mask");
		break;
	case wxPENSTYLE_STIPPLE:
		id = rb_intern("stipple");
		break;
	case wxPENSTYLE_BDIAGONAL_HATCH:
		id = rb_intern("bdiagona_hatch");
		break;
	case wxPENSTYLE_CROSSDIAG_HATCH:
		id = rb_intern("crossdiag_hatch");
		break;
	case wxPENSTYLE_FDIAGONAL_HATCH:
		id = rb_intern("fdiagonal_hatch");
		break;
	case wxPENSTYLE_CROSS_HATCH:
		id = rb_intern("cross_hatch");
		break;
	case wxPENSTYLE_HORIZONTAL_HATCH:
		id = rb_intern("horizontal_hatch");
		break;
	case wxPENSTYLE_VERTICAL_HATCH:
		id = rb_intern("vertical_hatch");
		break;
	default:
		return Qnil;
	};
	return ID2SYM(id);
}


template <>
inline wxPen* wrap< wxPen* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullPen;
	return unwrapPtr<wxPen>(vbitmap, rb_cWXPen);
}

template <>
inline wxPen wrap< wxPen >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return wxNullPen;
	return *wrap<wxPen*>(vbitmap);
}

#endif /* WXPEN_HPP_ */
