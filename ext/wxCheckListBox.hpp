/*
 * wxCheckListBox.hpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#ifndef WXCHECKLISTBOX_HPP_
#define WXCHECKLISTBOX_HPP_

#include "wxListBox.hpp"

extern VALUE rb_cWXCheckListBox;

void Init_WXCheckListBox(VALUE rb_mWX);
#if wxUSE_CHECKLISTBOX
#include <wx/checklst.h>
#endif

#endif /* WXCHECKLISTBOX_HPP_ */
