/*
 * wxChoiceBook.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXCHOICEBOOK_HPP_
#define WXCHOICEBOOK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXChoicebook;

void Init_WXChoiceBookCtrl(VALUE rb_mWX);

#if wxUSE_CHOICEBOOK
#include <wx/choicebk.h>

#endif


#endif /* WXCHOICEBOOK_HPP_ */
