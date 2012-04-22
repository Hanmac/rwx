/*
 * wxScrollEvent.hpp
 *
 *  Created on: 08.04.2012
 *      Author: hanmac
 */

#ifndef WXSCROLLEVENT_HPP_
#define WXSCROLLEVENT_HPP_

#include "main.hpp"

extern VALUE rb_cWXScrollEvent;
void Init_WXScrollEvent(VALUE rb_mWX);

template <>
inline wxScrollEvent* wrap< wxScrollEvent* >(const VALUE &vcolor)
{
	return unwrapPtr<wxScrollEvent>(vcolor, rb_cWXScrollEvent);
}

#endif /* WXSCROLLEVENT_HPP_ */
