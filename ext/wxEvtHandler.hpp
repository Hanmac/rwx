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

#include "wxTaskBar.hpp"


#include "wxTimer.hpp"
#include "wxAui.hpp"

#include <wx/sharedptr.h>

extern std::map<VALUE,wxEvtHandler*> evthandlerholder;

void registerEventType(const char *sym, wxEventType type,VALUE klass);

class RubyFunctorPtr
{
public:
	RubyFunctorPtr(VALUE obj);

	~RubyFunctorPtr();

	VALUE get() {return mValue;};
private:
	VALUE mValue;

};

class RubyFunctor
#ifndef wxHAS_EVENT_BIND
: public wxEvtHandler
#endif

{
public:
	RubyFunctor(VALUE obj);

	void operator()( wxEvent & event );
#if wxUSE_GUI
	void operator()( wxCommandEvent & event );
#endif
#if wxUSE_TIMER
	void operator()( wxTimerEvent & event );
#endif
private:
	wxSharedPtr<RubyFunctorPtr> ptr;
};

class RubyClientData : public wxClientData
{
public:
	RubyClientData(VALUE obj);
	~RubyClientData();
	VALUE mRuby;
	bool created;
};


extern VALUE rb_mWXEvtHandler;
void Init_WXEvtHandler(VALUE rb_mWX);


template <>
inline VALUE wrap< wxEvtHandler >(wxEvtHandler *handler )
{
	if(handler==NULL)
			return Qnil;

	if(wxMenu *menu = dynamic_cast<wxMenu*>(handler))
		return wrap(menu);
	if(wxApp *app = dynamic_cast<wxApp*>(handler))
		return wrap(app);
	if(wxWindow *window = dynamic_cast<wxWindow*>(handler))
		return wrap(window);
	if(wxTimer *timer = dynamic_cast<wxTimer*>(handler))
		return wrap(timer);
#if wxUSE_AUI
	if(wxAuiManager *mgr = dynamic_cast<wxAuiManager*>(handler))
		return wrap(mgr);
#endif
	return wrap(handler,Qnil);
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
#if wxUSE_AUI
	if (rb_obj_is_kind_of(vhandler, rb_cWXAuiManager))
		return wrap< wxAuiManager* >(vhandler);
#endif
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

#define _created static_cast<RubyClientData*>(_self->GetClientObject())->created


#endif /* WXEVTHANDLER_HPP_ */
