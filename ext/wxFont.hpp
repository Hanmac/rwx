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
wxFont nullPtr<wxFont>();


template <>
VALUE wrapenum< wxFontEncoding >(const wxFontEncoding &enc );

template <>
wxFontEncoding unwrapenum< wxFontEncoding >(const VALUE &venc);


#endif /* WXFONT_HPP_ */
