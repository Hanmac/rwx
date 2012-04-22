/*
 * wxToolBarBase.hpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#ifndef WXTOOLBARBASE_HPP_
#define WXTOOLBARBASE_HPP_

#include "wxToolBar.hpp"
#include "wxAuiToolBar.hpp"
extern VALUE rb_cWXToolBarBase;

void Init_WXToolBarBase(VALUE rb_mWX);


template <>
inline VALUE wrap< wxToolBarBase >(wxToolBarBase* window)
{
	return wrap(window,rb_cWXToolBarBase);
}


template <>
inline wxToolBarBase* wrap< wxToolBarBase* >(const VALUE &vwindow)
{
	return unwrapPtr<wxToolBarBase>(vwindow, rb_cWXToolBarBase);
}



#endif /* WXTOOLBARBASE_HPP_ */
