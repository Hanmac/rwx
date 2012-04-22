/*
 * wxAuiManager.hpp
 *
 *  Created on: 26.03.2012
 *      Author: hanmac
 */

#ifndef WXAUIMANAGER_HPP_
#define WXAUIMANAGER_HPP_

#include "wxAui.hpp"


extern VALUE rb_cWXAuiManager;

void Init_WXAuiManager(VALUE rb_mWX);
#if wxUSE_AUI
template <>
inline VALUE wrap< wxAuiManager >(wxAuiManager* window)
{
	return wrap(window,rb_cWXAuiManager);
}

template <>
inline wxAuiManager* wrap< wxAuiManager* >(const VALUE &vwindow)
{
	return unwrapPtr<wxAuiManager>(vwindow, rb_cWXAuiManager);
}

#endif

#endif /* WXAUIMANAGER_HPP_ */
