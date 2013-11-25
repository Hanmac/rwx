/*
 * wxPalette.hpp
 *
 *  Created on: 20.11.2013
 *      Author: hanmac
 */

#ifndef WXPALETTE_HPP_
#define WXPALETTE_HPP_

#include "main.hpp"

extern VALUE rb_cWXPalette;
DLL_LOCAL void Init_WXPalette(VALUE rb_mWX);

#if wxUSE_PALETTE
template <>
wxPalette* unwrap< wxPalette* >(const VALUE &vimage);


template <>
wxPalette unwrap< wxPalette >(const VALUE &vimage);
#endif
#endif /* WXPALETTE_HPP_ */
