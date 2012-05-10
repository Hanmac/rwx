/*
 * wxArrayStringProperty.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXARRAYSTRINGPROPERTY_HPP_
#define WXARRAYSTRINGPROPERTY_HPP_

#include "wxProperty.hpp"

extern VALUE rb_cWXArrayStringProperty;
DLL_LOCAL void Init_WXArrayStringProperty(VALUE rb_mWX);
#if wxUSE_PROPGRID
#include <wx/propgrid/props.h>
#endif

#endif /* WXARRAYSTRINGPROPERTY_HPP_ */
