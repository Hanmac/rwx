/*
 * wxSimplebook.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXSIMPLEBOOK_HPP_
#define WXSIMPLEBOOK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXSimplebook;

void Init_WXSimpleBookCtrl(VALUE rb_mWX);

#if wxUSE_BOOKCTRL
#include <wx/simplebook.h>
#endif

#endif /* WXSIMPLEBOOK_HPP_ */
