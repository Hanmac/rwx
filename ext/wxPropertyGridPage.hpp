/*
 * wxPropertyGridPAge.hpp
 *
 *  Created on: 05.12.2013
 *      Author: hanmac
 */

#ifndef WXPROPERTYGRIDPAGE_HPP_
#define WXPROPERTYGRIDPAGE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXPropertyGridPage;
DLL_LOCAL void Init_WXPropertyGridPage(VALUE rb_mWX);

#if wxUSE_PROPGRID
#include <wx/propgrid/propgrid.h>
#endif

#endif /* WXPROPERTYGRIDPAGE_HPP_ */
