/*
 * wxBitmap.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXBITMAP_HPP_
#define WXBITMAP_HPP_

#include "wxImage.hpp"
#include <wx/imaglist.h>


extern VALUE rb_cWXBitmap;
void Init_WXBitmap(VALUE rb_mWX);

template <>
VALUE wrap< wxBitmap >(const wxBitmap& bitmap );

template <>
VALUE wrap< wxBitmap >(wxBitmap *bitmap );

template <>
wxBitmap* unwrap< wxBitmap* >(const VALUE &vbitmap);

template <>
wxBitmap unwrap< wxBitmap >(const VALUE &vbitmap);

template <>
wxBitmap& unwrap< wxBitmap& >(const VALUE &vbitmap);

enum wrapBitmapType
{
	WRAP_BITMAP_NULL,
	WRAP_BITMAP_ID,
	WRAP_BITMAP_RAISE
};

wxBitmap wrapBitmap(const VALUE &vbitmap,wxWindowID id,wrapBitmapType type,const wxArtClient &client);

template <>
VALUE wrap< wxIcon >(wxIcon *icon );

template <>
VALUE wrap< wxIcon >(const wxIcon &icon );

template <>
wxIcon* unwrap< wxIcon* >(const VALUE &vbitmap);

template <>
wxIcon unwrap< wxIcon >(const VALUE &vbitmap);

template <>
VALUE wrap< wxImageList >(wxImageList *imagelist );

template <>
wxImageList* unwrap< wxImageList* >(const VALUE &imagelist);

#endif /* WXBITMAP_HPP_ */
