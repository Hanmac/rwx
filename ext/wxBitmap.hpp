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
		return Data_Wrap_Struct(rb_cWXBitmap, NULL, NULL, bitmap);
	return Qnil;
}

template <>
inline wxBitmap* wrap< wxBitmap* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullBitmap;
	if(rb_obj_is_kind_of(vbitmap,rb_cWXBitmap))
		return unwrapPtr<wxBitmap>(vbitmap, rb_cWXBitmap);
#if wxUSE_IMAGE
	if(rb_obj_is_kind_of(vbitmap,rb_cWXImage))
		return new wxBitmap(wrap<wxImage>(vbitmap));
#endif
	if(is_wrapable<wxSize>(vbitmap))
		return new wxBitmap(wrap<wxSize>(vbitmap));
	else
		return new wxBitmap(wrap<wxString>(vbitmap),wxBITMAP_TYPE_ANY);
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

#include <wx/imaglist.h>

template <>
inline VALUE wrap< wxImageList >(wxImageList *imagelist )
{
	VALUE result = rb_ary_new();
	int count = imagelist->GetImageCount();
	for(int i = 0;i < count;++i)
		rb_ary_push(result,wrap(imagelist->GetBitmap(i)));
	return result;
}

template <>
inline wxImageList* wrap< wxImageList* >(const VALUE &imagelist)
{
	wxImageList *result = new wxImageList;
	VALUE dup = rb_funcall(imagelist,rb_intern("to_a"),0);
	result->Create();
	size_t count = RARRAY_LEN(dup);
	for(size_t i = 0;i < count;++i)
		result->Add(wrap<wxBitmap>(RARRAY_PTR(dup)[i]));
	return result;
}
#endif /* WXBITMAP_HPP_ */
