/*
 * wxImageList.hpp
 *
 *  Created on: 11.05.2015
 *      Author: hanmac
 */

#ifndef WXIMAGELIST_HPP_
#define WXIMAGELIST_HPP_

#include "wxBitmap.hpp"
#include <wx/imaglist.h>


extern VALUE rb_cWXImageList;
void Init_WXImageList(VALUE rb_mWX);

template <>
VALUE wrap< wxImageList >(wxImageList *imagelist );

template <>
wxImageList* unwrap< wxImageList* >(const VALUE &imagelist);

#endif /* WXIMAGELIST_HPP_ */
