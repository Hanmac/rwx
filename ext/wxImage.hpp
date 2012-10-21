/*
 * wxImage.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXIMAGE_HPP_
#define WXIMAGE_HPP_

#include "wxSize.hpp"

extern VALUE rb_cWXImage;
DLL_LOCAL void Init_WXImage(VALUE rb_mWX);

#if wxUSE_IMAGE
template <>
wxImage* unwrap< wxImage* >(const VALUE &vimage);


template <>
wxImage unwrap< wxImage >(const VALUE &vimage);
#endif
#endif /* WXIMAGE_HPP_ */
