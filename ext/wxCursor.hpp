/*
 * wxCursor.hpp
 *
 *  Created on: 10.03.2012
 *      Author: hanmac
 */

#ifndef WXCURSOR_HPP_
#define WXCURSOR_HPP_

#include "wxBitmap.hpp"

extern VALUE rb_cWXCursor;
void Init_WXCursor(VALUE rb_mWX);

wxStockCursor toStockCursor(ID id);

template <>
inline VALUE wrap< wxCursor >(wxCursor *bitmap )
{
	if(bitmap)
		return Data_Wrap_Struct(rb_cWXCursor, NULL, free, bitmap);
	return Qnil;
}

template <>
inline wxCursor* wrap< wxCursor* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullCursor;
	if(SYMBOL_P(vbitmap))
	{
		return new wxCursor(toStockCursor(SYM2ID(vbitmap)));
	}else if(rb_obj_is_kind_of(vbitmap,rb_cWXCursor))
		return unwrapPtr<wxCursor>(vbitmap, rb_cWXCursor);
	else
#if wxUSE_IMAGE
	return new wxCursor(wrap<wxImage>(vbitmap));
#else
	return &wxNullCursor;
#endif
}


#endif /* WXCURSOR_HPP_ */
