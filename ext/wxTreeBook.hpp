/*
 * wxTreebook.hpp
 *
 *  Created on: 26.01.2014
 *      Author: hanmac
 */

#ifndef WXTREEBOOK_HPP_
#define WXTREEBOOK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXTreebook;

void Init_WXTreeBookCtrl(VALUE rb_mWX);

#if wxUSE_TREEBOOK
#include <wx/treebook.h>
#endif

#endif /* WXTREEBOOK_HPP_ */
