/*
 * wxRearrangeList.hpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#ifndef WXREARRANGELIST_HPP_
#define WXREARRANGELIST_HPP_

#include "wxCheckListBox.hpp"

extern VALUE rb_cWXRearrangeList;

void Init_WXRearrangeList(VALUE rb_mWX);
#if wxUSE_REARRANGECTRL
#include <wx/rearrangectrl.h>
#endif

#endif /* WXREARRANGELIST_HPP_ */
