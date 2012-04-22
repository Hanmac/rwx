/*
 * wxBrush.hpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#ifndef WXBRUSH_HPP_
#define WXBRUSH_HPP_


#include "main.hpp"

extern VALUE rb_cWXBrush;
void Init_WXBrush(VALUE rb_mWX);

template <>
inline VALUE wrap< wxBrush >(wxBrush *bitmap )
{
	if(bitmap->IsOk())
		return wrap(bitmap,rb_cWXBrush);
	return Qnil;
}

template <>
inline VALUE wrap< wxBrush >(const wxBrush &bitmap )
{
	if(bitmap.IsOk())
		return wrap(const_cast<wxBrush*>(&bitmap),rb_cWXBrush);
	return Qnil;
}

template <>
inline VALUE wrap< wxBrushStyle >(const wxBrushStyle &style )
{
	ID id;
	switch(style)
	{
	case wxBRUSHSTYLE_INVALID:
		return Qnil;
		break;
	case wxBRUSHSTYLE_SOLID:
		id = rb_intern("solid");
		break;
	case wxBRUSHSTYLE_TRANSPARENT:
		id = rb_intern("transparent");
		break;
	case wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE:
		id = rb_intern("stipple_mask_opaque");
		break;
	case wxBRUSHSTYLE_STIPPLE_MASK:
		id = rb_intern("stipple_mask");
		break;
	case wxBRUSHSTYLE_STIPPLE:
		id = rb_intern("stipple");
		break;
	case wxBRUSHSTYLE_BDIAGONAL_HATCH:
		id = rb_intern("bdiagona_hatch");
		break;
	case wxBRUSHSTYLE_CROSSDIAG_HATCH:
		id = rb_intern("crossdiag_hatch");
		break;
	case wxBRUSHSTYLE_FDIAGONAL_HATCH:
		id = rb_intern("fdiagonal_hatch");
		break;
	case wxBRUSHSTYLE_CROSS_HATCH:
		id = rb_intern("cross_hatch");
		break;
	case wxBRUSHSTYLE_HORIZONTAL_HATCH:
		id = rb_intern("horizontal_hatch");
		break;
	case wxBRUSHSTYLE_VERTICAL_HATCH:
		id = rb_intern("vertical_hatch");
		break;
	default:
		return Qnil;
	};
	return ID2SYM(id);
}


template <>
inline wxBrush* wrap< wxBrush* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullBrush;
	return unwrapPtr<wxBrush>(vbitmap, rb_cWXBrush);
}

template <>
inline wxBrush wrap< wxBrush >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return wxNullBrush;
	return *wrap<wxBrush*>(vbitmap);
}



#endif /* WXBRUSH_HPP_ */
