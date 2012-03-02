/*
 * wxToolBarTool.hpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#ifndef WXTOOLBARTOOL_HPP_
#define WXTOOLBARTOOL_HPP_

#include "main.hpp"

extern VALUE rb_cWXToolBarTool;
void Init_WXToolBarTool(VALUE rb_mWX);


template <>
inline VALUE wrap< wxToolBarToolBase >(wxToolBarToolBase *menu )
{
	if(menu)
		return Data_Wrap_Struct(rb_cWXToolBarTool, NULL, NULL, menu);
	return Qnil;
}

template <>
inline wxToolBarToolBase* wrap< wxToolBarToolBase* >(const VALUE &vmenu)
{
	return unwrapPtr<wxToolBarToolBase>(vmenu, rb_cWXToolBarTool);
}


#endif /* WXTOOLBARTOOL_HPP_ */
