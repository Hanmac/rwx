/*
 * wxEvent.hpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#ifndef WXEVENT_HPP_
#define WXEVENT_HPP_

#include <map>

#include "main.hpp"

extern VALUE rb_cWXEvent;
void Init_WXEvent(VALUE rb_mWX);

extern std::map<wxEventType,VALUE> evttypeclassholder;

template <>
inline VALUE wrap< wxEvent >(wxEvent *event )
{
	VALUE klass = rb_cWXEvent;
	std::map<wxEventType,VALUE>::iterator it = evttypeclassholder.find(event->GetEventType());
	if(it != evttypeclassholder.end())
		klass = it->second;
	return wrap(event,klass);
}

template <>
inline wxEvent* wrap< wxEvent* >(const VALUE &vcolor)
{
	return unwrapPtr<wxEvent>(vcolor, rb_cWXEvent);
}


#endif /* WXEVENT_HPP_ */
