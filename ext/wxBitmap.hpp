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
VALUE wrap< wxBitmap >(wxBitmap *bitmap );

template <>
wxBitmap* wrap< wxBitmap* >(const VALUE &vbitmap);

template <>
wxBitmap wrap< wxBitmap >(const VALUE &vbitmap);

wxBitmap wrapBitmap(const VALUE &vbitmap,wxWindowID id,bool disabled,const wxArtClient &type);

template <>
VALUE wrap< wxIcon >(wxIcon *icon );

template <>
VALUE wrap< wxIcon >(const wxIcon &icon );

template <>
wxIcon* wrap< wxIcon* >(const VALUE &vbitmap);

template <>
wxIcon wrap< wxIcon >(const VALUE &vbitmap);

template <>
VALUE wrap< wxImageList >(wxImageList *imagelist );

template <>
wxImageList* wrap< wxImageList* >(const VALUE &imagelist);

#endif /* WXBITMAP_HPP_ */
