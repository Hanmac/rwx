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

template <>
VALUE wrap< wxCursor >(wxCursor *bitmap );

template <>
wxCursor* wrap< wxCursor* >(const VALUE &vbitmap);

template <>
wxCursor wrap< wxCursor >(const VALUE &vbitmap);

#endif /* WXCURSOR_HPP_ */
