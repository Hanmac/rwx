/*
 * wxEvtHandler.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"
#include "wxEvent.hpp"

#include <map>

#define _self wrap<wxEvtHandler*>(self)

VALUE rb_mWXEvtHandler;

VALUE global_evthandler;

std::map<VALUE,wxEvtHandler*> evthandlerholder;

std::map<ID,wxEventType> evttypeholder;

template <>
wxEvtHandler* wrap< wxEvtHandler* >(const VALUE &vhandler)
{
	if(rb_type_p(vhandler,T_DATA))
		return unwrapPtr<wxEvtHandler>(vhandler,rb_mWXEvtHandler);
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




#define rubyclientdata(T) \
VALUE wrap(T *handler,VALUE klass)\
{\
	if(!handler)\
		return Qnil;\
\
	RubyClientData *rcd = dynamic_cast<RubyClientData*>(handler->GetClientObject());\
	if(!rcd)\
	{\
		if(NIL_P(klass))\
			return Qnil;\
		handler->SetClientObject(rcd = new RubyClientData(wrap((void*)handler,klass)));\
	}\
	if(rcd)\
		return rcd->mRuby;\
	return Qnil;\
}


rubyclientdata(wxEvtHandler)
rubyclientdata(wxClientDataContainer)
rubyclientdata(wxSizer)

#if wxUSE_PROPGRID
rubyclientdata(wxPGProperty)
#endif

wxEventType unwrapEventType(VALUE type)
{
	if(!SYMBOL_P(type))
		return NUM2INT(type);

	std::map<ID,wxEventType>::iterator it = evttypeholder.find(SYM2ID(type));
	if(it != evttypeholder.end())
		return it->second;

	return wxEVT_NULL;
}

void registerEventType(const char *sym, wxEventType type)
{
	evttypeholder.insert(std::make_pair(rb_intern(sym),type));
}
#ifndef wxHAS_EVENT_BIND
void registerEventType(const char *sym, wxEventType type,VALUE klass)
{
	registerEventType(sym,type);
}
#endif

RubyClientData::RubyClientData(VALUE obj) : wxClientData(), mRuby(obj),created(false)
{
	rb_hash_aset(global_evthandler,INT2NUM(obj),obj);
}

RubyClientData::~RubyClientData()
{
	rb_hash_delete(global_evthandler,INT2NUM(mRuby));
}

RubyFunctorPtr::RubyFunctorPtr(VALUE obj) : mValue(obj){
	rb_hash_aset(global_evthandler,INT2NUM(obj),obj);
}

RubyFunctorPtr::~RubyFunctorPtr(){
	rb_hash_delete(global_evthandler,INT2NUM(mValue));
}

RubyFunctor::RubyFunctor(VALUE obj) : ptr(new RubyFunctorPtr(obj)){

}


void RubyFunctor::operator()( wxEvent & event )
{
	rb_funcall(ptr->get(),rb_intern("call"),1,wrap(&event));
}
#if wxUSE_GUI
void RubyFunctor::operator()( wxCommandEvent & event )
{
	rb_funcall(ptr->get(),rb_intern("call"),1,wrap(&event));
}
#endif
#if wxUSE_TIMER
void RubyFunctor::operator()( wxTimerEvent & event )
{
	rb_funcall(ptr->get(),rb_intern("call"),1,wrap(&event));
}
#endif
namespace RubyWX {
namespace EvtHandler {

VALUE _bind(int argc,VALUE *argv,VALUE self)
{
	VALUE type,id,last,proc;
	rb_scan_args(argc, argv, "12&",&type,&id,&last,&proc);

	if(NIL_P(proc))
		proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
	_self->Bind(wxEventTypeTag<wxEvent>(unwrapEventType(type)),RubyFunctor(proc),unwrapID(id),unwrapID(last));
#else
	_self->Connect(unwrapID(id),unwrapID(last),unwrapEventType(type),wxEventHandler(RubyFunctor::operator()),NULL,new RubyFunctor(proc));
#endif
	return self;
}

VALUE _fire(int argc,VALUE *argv,VALUE self)
{
	VALUE type,id;
	rb_scan_args(argc, argv, "11",&type,&id);

	wxMenuEvent e(unwrapEventType(type),unwrapID(id));
	_self->ProcessEvent(e);
	return self;

}
}
}


void Init_WXEvtHandler(VALUE rb_mWX)
{
	using namespace RubyWX::EvtHandler;
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_define_method(rb_mWXEvtHandler,"bind",RUBY_METHOD_FUNC(_bind),-1);
	rb_define_method(rb_mWXEvtHandler,"call",RUBY_METHOD_FUNC(_fire),-1);

	global_evthandler = rb_hash_new();
	rb_global_variable(&global_evthandler);

	//because only Evthandler are created different
	registerInfo<wxEvtHandler>(Qnil);
}



