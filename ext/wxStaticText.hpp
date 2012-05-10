/*
 * wxStaticText.hpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */

#ifndef WXSTATICTEXT_HPP_
#define WXSTATICTEXT_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXStaticText;

void Init_WXStaticText(VALUE rb_mWX);

#if wxUSE_STATTEXT
#include <wx/stattext.h>
#endif

#endif /* WXSTATICTEXT_HPP_ */
