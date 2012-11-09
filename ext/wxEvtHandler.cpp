/*
 * wxEvtHandler.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"
#include "wxEvent.hpp"

#include <map>

#define _self unwrap<wxEvtHandler*>(self)

VALUE rb_mWXEvtHandler;

VALUE global_evthandler;

std::map<VALUE,wxEvtHandler*> evthandlerholder;

typedef std::map<ID,wxEventType>  evttypeholdertype;
typedef std::map<wxEventType,wxClassInfo*>  evttypeclassholdertype;

evttypeholdertype evttypeholder;
evttypeclassholdertype evttypeclassholder;

template <>
wxEvtHandler* unwrap< wxEvtHandler* >(const VALUE &vhandler)
{
	if(TYPE(vhandler) == T_DATA)
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

#if wxUSE_XRC
bool loadxrc(wxObject *self,VALUE name,wxWindow *parent)
{
	if(rb_obj_is_kind_of(name,rb_cString)){

		wxClassInfo *info = self->GetClassInfo();
		if(!(wxXmlResource::Get()->LoadObjectRecursively(self,parent,unwrap<wxString>(name),wxString(info->GetClassName()))))
			rb_raise(rb_eNameError,"Named %s '%s' is not found.",wxString(info->GetClassName()).GetData().AsChar(),unwrap<char*>(name));
		return true;
	}
	return false;
}
#endif

#if !wxUSE_GUI
class RubyEvent : public wxEvent
{
public:

	wxEvent* Clone()const { return new RubyEvent(*this);}
};
#endif

#define rubyclientdata(T) \
VALUE wrapPtr(T *handler,VALUE klass)\
{\
	if(!handler)\
		return Qnil;\
\
	RubyClientData *rcd = dynamic_cast<RubyClientData*>(handler->GetClientObject());\
	if(!rcd)\
	{\
		if(NIL_P(klass))\
			return Qnil;\
		handler->SetClientObject(rcd = new RubyClientData(wrapPtr((void*)handler,klass)));\
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

VALUE wrapPtr(wxObject *object,VALUE klass)
{
	if(!object)
		return Qnil;

	if(wxEvtHandler *evt = wxDynamicCast(object,wxEvtHandler))
		return wrapPtr(evt,klass);
	if(wxSizer *evt = wxDynamicCast(object,wxSizer))
		return wrapPtr(evt,klass);
	if(wxClientDataContainer *evt = dynamic_cast<wxClientDataContainer*>(object))
		return wrapPtr(evt,klass);
#if wxUSE_PROPGRID
	if(wxPGProperty *evt = wxDynamicCast(object,wxPGProperty))
		return wrapPtr(evt,klass);
#endif

	return wrapPtr((void*)object,klass);
}


wxEventType unwrapEventType(VALUE type)
{
	if(NIL_P(type))
		return wxEVT_ANY;

	if(!SYMBOL_P(type))
		return NUM2INT(type);

	evttypeholdertype::iterator it = evttypeholder.find(SYM2ID(type));
	if(it != evttypeholder.end())
		return it->second;

	wxEventType evt = wxNewEventType();
	evttypeholder[SYM2ID(type)] = evt;
	return evt;
}

VALUE wrapEventType(wxEventType type)
{
	for(evttypeholdertype::iterator it = evttypeholder.begin();it != evttypeholder.end(); ++it)
	{
		if(it->second == type)
			return ID2SYM(it->first);
	}
	return INT2NUM(type);
}

void registerEventType(const char *sym, wxEventType type,wxClassInfo *info)
{
	evttypeholder[rb_intern(sym)]=type;
	evttypeclassholder[type] = info;
}

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

	wxEvent *evt;
	wxEventType etype = unwrapEventType(type);

	evttypeclassholdertype::iterator it = evttypeclassholder.find(etype);
	if(it != evttypeclassholder.end())
		evt = wxDynamicCast(it->second->CreateObject(),wxEvent);
	else
#if wxUSE_GUI
		evt = new wxCommandEvent;
#else
		evt = new RubyEvent;
#endif

	ID IDid = rb_intern("id");
	evt->SetEventType(etype);
	if(!NIL_P(id))
		evt->SetId(unwrapID(id));
	else
	{
		if(rb_respond_to(self,IDid))
			evt->SetId(unwrapID(rb_funcall(self,IDid,0)));
	}
	evt->SetEventObject(_self);

	if(rb_block_given_p())
		rb_yield(wrap(evt));

	return wrap(_self->ProcessEvent(*evt));

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



