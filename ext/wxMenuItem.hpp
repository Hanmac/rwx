/*
 * wxMenuItem.hpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#ifndef WXMENUITEM_HPP_
#define WXMENUITEM_HPP_


#include "main.hpp"

extern VALUE rb_cWXMenuItem;
void Init_WXMenuItem(VALUE rb_mWX);


template <>
inline VALUE wrap< wxMenuItem >(wxMenuItem *menu )
{
	if(menu)
		return wrap(menu,rb_cWXMenuItem);
	return Qnil;
}

template <>
inline wxMenuItem* wrap< wxMenuItem* >(const VALUE &vmenu)
{
	return unwrapPtr<wxMenuItem>(vmenu, rb_cWXMenuItem);
}


#endif /* WXMENUITEM_HPP_ */
