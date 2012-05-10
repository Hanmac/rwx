/*
 * wxFont.hpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#ifndef WXFONT_HPP_
#define WXFONT_HPP_

#include "main.hpp"

extern VALUE rb_cWXFont;
DLL_LOCAL void Init_WXFont(VALUE rb_mWX);


template <>
VALUE wrap< wxFontWeight >(const wxFontWeight &vmenu);

template <>
wxFontWeight wrap< wxFontWeight >(const VALUE &vmenu);

template <>
wxFontStyle wrap< wxFontStyle >(const VALUE &vmenu);

template <>
wxFontFamily wrap< wxFontFamily >(const VALUE &vmenu);

template <>
wxFont nullPtr<wxFont>();

#endif /* WXFONT_HPP_ */
