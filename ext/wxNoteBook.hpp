/*
 * wxNotebook.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXNOTEBOOK_HPP_
#define WXNOTEBOOK_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXNotebook;

void Init_WXNoteBookCtrl(VALUE rb_mWX);

#if wxUSE_NOTEBOOK
#include <wx/notebook.h>
#endif

#endif /* WXNOTEBOOK_HPP_ */
