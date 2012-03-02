/*
 * wxEvtHandler.hpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */

#ifndef WXEVTHANDLER_HPP_
#define WXEVTHANDLER_HPP_

#include <map>

#include "wxMenu.hpp"
#include "wxApp.hpp"
#include "wxWindow.hpp"

extern std::map<VALUE,wxEvtHandler*> evthandlerholder;

class RubyFunctor
#ifndef wxHAS_EVENT_BIND
 : public wxEvtHandler
#endif
{
public:
	RubyFunctor(VALUE obj) : mValue(obj){
	}
	void operator()( wxEvent & event );
	void operator()( wxCommandEvent & event );

private:
	VALUE mValue;
};

extern VALUE rb_mWXEvtHandler;
void Init_WXEvtHandler(VALUE rb_mWX);


template <>
inline VALUE wrap< wxEvtHandler >(wxEvtHandler *handler )
{
	if(handler==NULL)
			return Qnil;

	wxMenu *menu = dynamic_cast<wxMenu*>(handler);
	if(menu)
		return wrap(menu);
	wxApp *app = dynamic_cast<wxApp*>(handler);
	if(app)
		return wrap(app);
	wxWindow *window = dynamic_cast<wxWindow*>(handler);
	if(window)
		return wrap(window);

	std::map<VALUE,wxEvtHandler*>::iterator it;
	for(it = evthandlerholder.begin();it != evthandlerholder.end();++it)
	{
		if(it->second == handler)
			return it->first;
	}

	return Qnil;
}

template <>
inline wxEvtHandler* wrap< wxEvtHandler* >(const VALUE &vhandler)
{
	if (rb_obj_is_kind_of(vhandler, rb_cWXWindow))
		return wrap< wxWindow* >(vhandler);
	if (rb_obj_is_kind_of(vhandler, rb_cWXApp))
		return wrap< wxApp* >(vhandler);
	if (rb_obj_is_kind_of(vhandler, rb_cWXMenu))
		return wrap< wxMenu* >(vhandler);

	std::map<VALUE,wxEvtHandler*>::iterator it = evthandlerholder.find(vhandler);
	if(it != evthandlerholder.end())
		return it->second;
	else
	{
		wxEvtHandler *result = new wxEvtHandler;
		evthandlerholder.insert(std::make_pair(vhandler,result));
		return result;
	}
}



#endif /* WXEVTHANDLER_HPP_ */
