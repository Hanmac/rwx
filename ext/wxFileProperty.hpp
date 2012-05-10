/*
 * wxFileProperty.hpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#ifndef WXFILEPROPERTY_HPP_
#define WXFILEPROPERTY_HPP_

#include "wxProperty.hpp"

extern VALUE rb_cWXFileProperty;
DLL_LOCAL void Init_WXFileProperty(VALUE rb_mWX);
#if wxUSE_PROPGRID
#include <wx/propgrid/props.h>
#endif

#endif /* WXFILEPROPERTY_HPP_ */
