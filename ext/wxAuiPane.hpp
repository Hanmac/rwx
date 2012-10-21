/*
 * wxAuiPane.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXAUIPANE_HPP_
#define WXAUIPANE_HPP_

#include "wxBitmap.hpp"
#include "wxAui.hpp"

extern VALUE rb_cWXAuiPane;

void Init_WXAuiPane(VALUE rb_mWX);


#if wxUSE_AUI

template <>
VALUE wrap< wxAuiPaneInfo >(wxAuiPaneInfo *vinfo);

template <>
wxAuiPaneInfo unwrap< wxAuiPaneInfo >(const VALUE &vinfo);

#endif

#endif /* WXAUIPANE_HPP_ */
