/*
 * wxPropertyGridManager.hpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#ifndef WXPROPERTYGRIDMANAGER_HPP_
#define WXPROPERTYGRIDMANAGER_HPP_

#include "wxPanel.hpp"

extern VALUE rb_cWXPropertyGridManager;
DLL_LOCAL void Init_WXPropertyGridManager(VALUE rb_mWX);

#if wxUSE_PROPGRID
#include <wx/propgrid/manager.h>
#endif

#endif /* WXPROPERTYGRIDMANAGER_HPP_ */
