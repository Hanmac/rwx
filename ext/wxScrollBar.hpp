/*
 * wxScrollBar.hpp
 *
 *  Created on: 15.04.2016
 *      Author: hanmac
 */

#ifndef WXSCROLLBAR_HPP_
#define WXSCROLLBAR_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXScrollBar;

void Init_WXScrollBar(VALUE rb_mWX);
#if wxUSE_SCROLLBAR
#include <wx/scrolbar.h>
#endif


#endif /* WXSCROLLBAR_HPP_ */
