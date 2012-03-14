/*
 * wxFileHistory.hpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#ifndef WXFILEHISTORY_HPP_
#define WXFILEHISTORY_HPP_

#include "main.hpp"

extern VALUE rb_cWXFileHistory;

void Init_WXFileHistory(VALUE rb_mWX);
#if wxUSE_FILE_HISTORY
#include <wx/filehistory.h>

template <>
inline VALUE wrap< wxFileHistory >(wxFileHistory* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXFileHistory);
}


template <>
inline wxFileHistory* wrap< wxFileHistory* >(const VALUE &vwindow)
{
	return unwrapPtr<wxFileHistory>(vwindow, rb_cWXFileHistory);
}
#endif

#endif /* WXFILEHISTORY_HPP_ */
