/*
 * wxPropertyGrid.hpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#ifndef WXPROPERTYGRID_HPP_
#define WXPROPERTYGRID_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXPropertyGrid;
DLL_LOCAL void Init_WXPropertyGrid(VALUE rb_mWX);

#if wxUSE_PROPGRID
#include <wx/propgrid/propgrid.h>
#endif

#endif /* WXPROPERTYGRID_HPP_ */
