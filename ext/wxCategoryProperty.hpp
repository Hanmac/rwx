/*
 * wxCategoryProperty.hpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#ifndef WXCATEGORYPROPERTY_HPP_
#define WXCATEGORYPROPERTY_HPP_

#include "wxProperty.hpp"

extern VALUE rb_cWXCategoryProperty;
DLL_LOCAL void Init_WXCategoryProperty(VALUE rb_mWX);
#if wxUSE_PROPGRID
#include <wx/propgrid/props.h>
#endif

#endif /* WXCATEGORYPROPERTY_HPP_ */
