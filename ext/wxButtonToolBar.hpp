/*
 * wxToolBar.hpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#ifndef WXBUTTONTOOLBAR_HPP_
#define WXBUTTONTOOLBAR_HPP_

#include "main.hpp"

#if defined(__WXMAC__) && wxUSE_TOOLBAR && wxUSE_BMPBUTTON
#include <wx/generic/buttonbar.h>
#endif

extern VALUE rb_cWXButtonToolBar;

void Init_WXButtonToolBar(VALUE rb_mWX);

#endif /* WXBUTTONTOOLBAR_HPP_ */
