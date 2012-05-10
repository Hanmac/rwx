/*
 * wxWindow.hpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#ifndef WXWINDOW_HPP_
#define WXWINDOW_HPP_


#include "wxEvtHandler.hpp"

#include "wxMenuBar.hpp"
#include "wxPanel.hpp"
#include "wxControl.hpp"
#include "wxToplevel.hpp"

extern VALUE rb_cWXWindow;
DLL_LOCAL void Init_WXWindow(VALUE rb_mWX);

VALUE wrapID(wxWindowID val);
wxWindowID unwrapID(VALUE val);
void registerID(const char *name,wxWindowID id);

#if wxUSE_TOOLTIPS
#include <wx/tooltip.h>

template <>
VALUE wrap< wxToolTip >(wxToolTip* window);
template <>
wxToolTip* wrap< wxToolTip* >(const VALUE &vwindow);

#endif
#endif /* WXWINDOW_HPP_ */
