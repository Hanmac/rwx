/*
 * wxDataViewList.hpp
 *
 *  Created on: 24.03.2012
 *      Author: hanmac
 */

#ifndef WXDATAVIEWLIST_HPP_
#define WXDATAVIEWLIST_HPP_

#include "main.hpp"

extern VALUE rb_cWXDataViewList;

void Init_WXDataViewList(VALUE rb_mWX);
#if wxUSE_DATAVIEWCTRL
#include <wx/dataview.h>
template <>
inline VALUE wrap< wxDataViewListCtrl >(wxDataViewListCtrl* window)
{
	return wrap(window,rb_cWXDataViewList);
}


template <>
inline wxDataViewListCtrl* wrap< wxDataViewListCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxDataViewListCtrl>(vwindow, rb_cWXDataViewList);
}

#endif


#endif /* WXDATAVIEWLIST_HPP_ */
