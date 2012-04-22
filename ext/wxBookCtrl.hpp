/*
 * wxBookCtrl.hpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#ifndef WXBOOKCTRL_HPP_
#define WXBOOKCTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXBookCtrlBase,rb_cWXBookCtrlEvent;

void Init_WXBookCtrl(VALUE rb_mWX);

#if wxUSE_BOOKCTRL
#include "wxChoiceBook.hpp"
#include "wxNoteBook.hpp"
#include "wxToolBook.hpp"
#include "wxAuiBook.hpp"
template <>
inline VALUE wrap< wxBookCtrlBase >(wxBookCtrlBase* window)
{
	return wrap(window,rb_cWXBookCtrlBase);
}

template <>
inline wxBookCtrlBase* wrap< wxBookCtrlBase* >(const VALUE &vwindow)
{
	return unwrapPtr<wxBookCtrlBase>(vwindow, rb_cWXBookCtrlBase);
}
#endif

#endif /* WXBOOKCTRL_HPP_ */
