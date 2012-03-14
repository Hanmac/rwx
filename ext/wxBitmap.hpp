/*
 * wxBitmap.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXBITMAP_HPP_
#define WXBITMAP_HPP_

#include "wxImage.hpp"

#include <wx/artprov.h>

extern VALUE rb_cWXBitmap;
void Init_WXBitmap(VALUE rb_mWX);

template <>
inline VALUE wrap< wxBitmap >(wxBitmap *bitmap )
{
	if(bitmap)
		return Data_Wrap_Struct(rb_cWXBitmap, NULL, free, bitmap);
	return Qnil;
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

wxBitmap wrapBitmap(const VALUE &vbitmap,wxWindowID id,bool disabled,const wxArtClient &type);

template <>
inline VALUE wrap< wxIcon >(wxIcon *icon )
{
	if(icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(icon);
}
template <>
inline VALUE wrap< wxIcon >(const wxIcon &icon )
{
	if(&icon == &wxNullIcon)
		return Qnil;
	return wrap< wxBitmap >(new wxBitmap(icon));
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
