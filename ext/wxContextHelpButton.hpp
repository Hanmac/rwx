/*
 * wxContextHelpButton.hpp
 *
 *  Created on: 22.12.2013
 *      Author: hanmac
 */

#ifndef WXCONTEXTHELPBUTTON_HPP_
#define WXCONTEXTHELPBUTTON_HPP_

#include "wxBitmapButton.hpp"

extern VALUE rb_cWXContextHelpButton;

void Init_WXContextHelpButton(VALUE rb_mWX);

#if wxUSE_HELP
#include <wx/cshelp.h>
#endif

#endif /* WXCONTEXTHELPBUTTON_HPP_ */
