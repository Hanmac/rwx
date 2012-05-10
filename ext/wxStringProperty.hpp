/*
 * wxStringProperty.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXSTRINGPROPERTY_HPP_
#define WXSTRINGPROPERTY_HPP_

#include "wxProperty.hpp"

extern VALUE rb_cWXStringProperty;
DLL_LOCAL void Init_WXStringProperty(VALUE rb_mWX);
#if wxUSE_PROPGRID
#include <wx/propgrid/props.h>
#endif
#endif /* WXSTRINGPROPERTY_HPP_ */
