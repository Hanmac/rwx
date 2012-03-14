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
#include "wxEvent.hpp"

#include "wxTimer.hpp"
extern std::map<VALUE,wxEvtHandler*> evthandlerholder;

void registerEventType(const char *sym, wxEventType type,VALUE klass);

class RubyFunctor
#ifndef wxHAS_EVENT_BIND
: public wxEvtHandler
#endif
{
public:
	RubyFunctor(VALUE obj) : mValue(obj){
	}
	void operator()( wxEvent & event );
//*
	void operator()( wxCommandEvent & event );
//*/
	void operator()( wxTimerEvent & event );
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

	return getEvtObj(handler,Qnil);
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
	if (rb_obj_is_kind_of(vhandler, rb_cWXTimer))
		return wrap< wxTimer* >(vhandler);

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
