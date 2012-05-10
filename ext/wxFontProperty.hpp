/*
 * wxFontProperty.hpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#ifndef WXFONTPROPERTY_HPP_
#define WXFONTPROPERTY_HPP_

#include "wxProperty.hpp"

extern VALUE rb_cWXFontProperty;
DLL_LOCAL void Init_WXFontProperty(VALUE rb_mWX);
#if wxUSE_PROPGRID
#include <wx/propgrid/advprops.h>
#endif

#endif /* WXFONTPROPERTY_HPP_ */
