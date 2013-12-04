/*
 * wxEvtHandler.hpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */

#ifndef WXEVTHANDLER_HPP_
#define WXEVTHANDLER_HPP_

#include "main.hpp"

#include "wxMenu.hpp"
#include "wxApp.hpp"
#include "wxWindow.hpp"
#include "wxEvent.hpp"
#include "wxCommandEvent.hpp"
#include "wxTaskBar.hpp"


#include "wxTimer.hpp"
#include "wxAui.hpp"

#include <wx/sharedptr.h>

DLL_LOCAL void registerEventType(const char *sym, wxEventType type,wxClassInfo *info);

#ifdef wxHAS_EVENT_BIND
template<typename T>
DLL_LOCAL void registerEventType(const char *sym, wxEventTypeTag<T> type,VALUE klass)
{
	registerEventType(sym,type,wxCLASSINFO(T));
	if(!NIL_P(klass))
		registerInfo<T>(klass);
}
template<typename T>
DLL_LOCAL void registerEventType(const char *sym, wxEventTypeTag<T> type)
{
	registerEventType(sym,type,Qnil);
}
#else
template<typename T>
DLL_LOCAL void registerEventType(const char *sym, wxEventType type,VALUE klass)
{
	registerEventType(sym,type,wxCLASSINFO(T));
	if(!NIL_P(klass))
		registerInfo<T>(klass);
}
template<typename T>
DLL_LOCAL void registerEventType(const char *sym, wxEventType type)
{
	registerEventType<T>(sym,type,Qnil);
}

#endif

DLL_LOCAL wxEventType unwrapEventType(VALUE type);
DLL_LOCAL VALUE wrapEventType(wxEventType type);

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
wxEvtHandler* unwrap< wxEvtHandler* >(const VALUE &vhandler);

#define _created static_cast<RubyClientData*>(_self->GetClientObject())->created


#if wxUSE_XRC
bool loadxrc(wxObject *self,VALUE name,wxWindow *parent);
#endif


#endif /* WXEVTHANDLER_HPP_ */
