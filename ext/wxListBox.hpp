/*
 * wxListBox.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXLISTBOX_HPP_
#define WXLISTBOX_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXListBox;

void Init_WXListBox(VALUE rb_mWX);
#if wxUSE_LISTBOX
#include <wx/listbox.h>
#endif

#endif /* WXLISTBOX_HPP_ */
