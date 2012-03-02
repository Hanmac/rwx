/*
 * wxBitmap.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXBITMAP_HPP_
#define WXBITMAP_HPP_

#include "wxImage.hpp"

extern VALUE rb_cWXBitmap;
void Init_WXBitmap(VALUE rb_mWX);


template <>
inline VALUE wrap< wxBitmap >(wxBitmap *bitmap )
{
	return Data_Wrap_Struct(rb_cWXBitmap, NULL, free, bitmap);
}

template <>
inline wxBitmap* wrap< wxBitmap* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullBitmap;
	if(rb_obj_is_kind_of(vbitmap,rb_cWXBitmap))
		return unwrapPtr<wxBitmap>(vbitmap, rb_cWXBitmap);
	if(is_wrapable<wxSize>(vbitmap))
		return new wxBitmap(wrap<wxSize>(vbitmap));
#if wxUSE_IMAGE
	return new wxBitmap(wrap<wxImage>(vbitmap));
#else
	return new wxBitmap(wrap<wxString>(vbitmap),wxBITMAP_TYPE_ANY);
#endif
}

template <>
inline wxBitmap wrap< wxBitmap >(const VALUE &vbitmap)
{
	return *wrap<wxBitmap*>(vbitmap);
}

template <>
inline wxIcon* wrap< wxIcon* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullIcon;
	wxIcon *icon = new wxIcon();
	icon->CopyFromBitmap(wrap<wxBitmap>(vbitmap));
	return icon;
}

template <>
inline wxIcon wrap< wxIcon >(const VALUE &vbitmap)
{
	return *wrap<wxIcon*>(vbitmap);
}


#endif /* WXBITMAP_HPP_ */
