/*
 * wxSTC.hpp
 *
 *  Created on: 01.04.2012
 *      Author: hanmac
 */

#ifndef WXSTC_HPP_
#define WXSTC_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXSTC;

void Init_WXSTC(VALUE rb_mWX);
#if wxUSE_STC
#include <wx/stc/stc.h>
#endif


#endif /* WXSTC_HPP_ */
