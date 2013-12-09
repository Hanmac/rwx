/*
 * wxListbook.hpp
 *
 *  Created on: 09.12.2013
 *      Author: hanmac
 */

#ifndef WXLISTBOOK_HPP_
#define WXLISTBOOK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXListbook;

void Init_WXListBookCtrl(VALUE rb_mWX);

#if wxUSE_LISTBOOK
#include <wx/listbook.h>
#endif

#endif /* WXLISTBOOK_HPP_ */
