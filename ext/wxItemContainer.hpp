/*
 * wxItemContainer.hpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */

#ifndef WXITEMCONTAINER_HPP_
#define WXITEMCONTAINER_HPP_

#include "main.hpp"

extern VALUE rb_mWXItemContainer;

void Init_WXItemContainer(VALUE rb_mWX);

#if wxUSE_CONTROLS

template <>
inline wxItemContainer* wrap< wxItemContainer* >(const VALUE &vwindow)
{
	return unwrapPtr<wxItemContainer>(vwindow, rb_mWXItemContainer);
}
#endif


#endif /* WXITEMCONTAINER_HPP_ */
