/*
 * wxStaticBox.hpp
 *
 *  Created on: 24.04.2012
 *      Author: hanmac
 */

#ifndef WXSTATICBOX_HPP_
#define WXSTATICBOX_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXStaticBox;

void Init_WXStaticBox(VALUE rb_mWX);

#if wxUSE_STATBOX
#include <wx/statbox.h>
#endif


#endif /* WXSTATICBOX_HPP_ */
