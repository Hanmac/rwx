/*
 * wxToolBook.hpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#ifndef WXTOOLBOOK_HPP_
#define WXTOOLBOOK_HPP_


#include "wxEvtHandler.hpp"

extern VALUE rb_cWXToolbook;

void Init_WXToolBookCtrl(VALUE rb_mWX);

#if wxUSE_TOOLBOOK
#include <wx/toolbook.h>
#endif


#endif /* WXTOOLBOOK_HPP_ */
