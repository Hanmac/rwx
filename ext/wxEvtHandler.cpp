/*
 * wxEvtHandler.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxEvent.hpp"

#if wxUSE_TEXTCTRL
#include "wxTextCtrl.hpp"
#endif

#include <map>

#define _self unwrap<wxEvtHandler*>(self)

VALUE rb_mWXEvtHandler;
ID rwx_IDcall, rwx_IDid;

typedef std::map<VALUE,wxEvtHandler*> evthandlerholdertype;
typedef std::map<ID,wxEventType>  evttypeholdertype;
typedef std::map<wxEventType,wxClassInfo*>  evttypeclassholdertype;

evthandlerholdertype evthandlerholder;
evttypeholdertype evttypeholder;
evttypeclassholdertype evttypeclassholder;

template <>
wxEvtHandler* unwrap< wxEvtHandler* >(const VALUE &vhandler)
{
	if(TYPE(vhandler) == T_DATA) {
		return unwrapTypedPtr<wxEvtHandler>(vhandler,rb_mWXEvtHandler);
	}

	evthandlerholdertype::iterator it = evthandlerholder.find(vhandler);
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

		wxString classname = wxString(self->GetClassInfo()->GetClassName());
		if(!(wxXmlResource::Get()->LoadObjectRecursively(self,parent,unwrap<wxString>(name),classname)))
			rb_raise(rb_eNameError,
				"Named %s '%" PRIsVALUE "' is not found.",
				classname.GetData().AsChar(), RB_OBJ_STRING(name)
			);
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
VALUE wrapTypedPtr(T *handler,VALUE klass)\
{\
	if(!handler)\
		return Qnil;\
\
	RubyClientData *rcd = dynamic_cast<RubyClientData*>(handler->GetClientObject());\
	if(!rcd)\
	{\
		if(NIL_P(klass))\
			return Qnil;\
		handler->SetClientObject(rcd = new RubyClientData(wrapTypedPtr((void*)handler,klass)));\
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
#if wxUSE_GRID
rubyclientdata(wxGridTableBase)
rubyclientdata(wxGridCellAttr)
#endif


VALUE wrapTypedPtr(wxObject *object,VALUE klass)
{
	if(!object)
		return Qnil;

	if(wxEvtHandler *evt = wxDynamicCast(object,wxEvtHandler))
		return wrapTypedPtr(evt,klass);
	if(wxSizer *evt = wxDynamicCast(object,wxSizer))
		return wrapTypedPtr(evt,klass);
	if(wxClientDataContainer *evt = dynamic_cast<wxClientDataContainer*>(object))
		return wrapTypedPtr(evt,klass);
#if wxUSE_PROPGRID
	if(wxPGProperty *evt = wxDynamicCast(object,wxPGProperty))
		return wrapTypedPtr(evt,klass);
#endif

	return wrapTypedPtr((void*)object,klass);
}


wxEventType unwrapEventType(VALUE type)
{
	if(NIL_P(type))
		return wxEVT_ANY;

	if(!RB_SYMBOL_P(type))
		return RB_NUM2INT(type);

	evttypeholdertype::iterator it = evttypeholder.find(RB_SYM2ID(type));
	if(it != evttypeholder.end())
		return it->second;

	wxEventType evt = wxNewEventType();
	evttypeholder[RB_SYM2ID(type)] = evt;
	return evt;
}

VALUE wrapEventType(wxEventType type)
{
	for(evttypeholdertype::iterator it = evttypeholder.begin();it != evttypeholder.end(); ++it)
	{
		if(it->second == type)
			return RB_ID2SYM(it->first);
	}
	return RB_INT2NUM(type);
}

void registerEventType(const char *sym, wxEventType type,wxClassInfo *info)
{
	evttypeholder[rb_intern(sym)]=type;
	evttypeclassholder[type] = info;
}

#ifdef wxHAS_CALL_AFTER
class RubyCallAfter : public wxObject
{
public:
	RubyCallAfter(VALUE val) : mVal(val){};

	void call(VALUE val)
	{
		rb_funcall(mVal, rwx_IDcall, 1, val);
	}

private:
	VALUE mVal;
};
#endif


RubyClientData::RubyClientData(VALUE obj) : wxClientData(), mRuby(obj)
{
	rwx_refobject(obj);
}

RubyClientData::~RubyClientData()
{
	if(rwx_unrefobject(mRuby) && unwrapDataType(CLASS_OF(mRuby)))
	{
		RTYPEDDATA_DATA(mRuby) = NULL;
	}
}

RubyFunctor::RubyFunctor(VALUE obj) : ptr(new RubyClientData(obj)){

}


void RubyFunctor::operator()( wxEvent & event )
{
	rb_funcall(ptr->mRuby,rwx_IDcall,1,wrap(&event));
}
#if wxUSE_GUI
void RubyFunctor::operator()( wxCommandEvent & event )
{
	rb_funcall(ptr->mRuby,rwx_IDcall,1,wrap(&event));
}
#endif
#if wxUSE_TIMER
void RubyFunctor::operator()( wxTimerEvent & event )
{
	rb_funcall(ptr->mRuby,rwx_IDcall,1,wrap(&event));
}
#endif
namespace RubyWX {
namespace EvtHandler {

/*
 * call-seq:
 *   bind(type[, id[, last]]) {|evt| } -> self
 *
 * bind the block to the given event type. id and id-range are optional.
 * ===Arguments
 * * type Symbol/Integer
 * * id Symbol/Integer
 * * last Symbol/Integer
 * ===Return value
 * self
 * === Exceptions
 * [ArgumentError]
 * * when the evthander prevent the binding of the given type.
*/
DLL_LOCAL VALUE _bind(int argc,VALUE *argv,VALUE self)
{
	VALUE type,id,last,proc;
	rb_scan_args(argc, argv, "12&",&type,&id,&last,&proc);

	wxEventType ctype = unwrapEventType(type);

#if wxUSE_TEXTCTRL
	if(ctype == wxEVT_TEXT_ENTER && rb_obj_is_kind_of(self, rb_cWXTextCtrl))
	{
		wxTextCtrl* textctrl = unwrap<wxTextCtrl*>(self);
		if(!textctrl->HasFlag(wxTE_PROCESS_ENTER)) {
			rb_raise(rb_eArgError,
				"'%" PRIsVALUE "' for '%" PRIsVALUE "' needs 'process_enter' option.",
				RB_OBJ_STRING(type), RB_OBJ_STRING(self)
			);
		}
	}
#endif

	if(NIL_P(proc))
		proc = rb_block_proc();

	_self->Bind(wxEventTypeTag<wxEvent>(ctype),RubyFunctor(proc),unwrapID(id),unwrapID(last));

	return self;
}


/*
 * call-seq:
 *   call(type[, id]) {|evt| } -> bool
 *
 * create an event for the given event type.
 * ===Arguments
 * * type Symbol/Integer
 * * id Symbol/Integer
 * ===Return value
 * bool
*/
DLL_LOCAL VALUE _call(int argc,VALUE *argv,VALUE self)
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

	evt->SetEventType(etype);
	if(!NIL_P(id))
		evt->SetId(unwrapID(id));
	else
	{
		if(rb_respond_to(self,rwx_IDid))
			evt->SetId(unwrapID(rb_funcall(self,rwx_IDid,0)));
	}
	evt->SetEventObject(_self);

	if(rb_block_given_p())
		rb_yield(wrap(evt));

	return wrap(_self->ProcessEvent(*evt));

}

DLL_LOCAL VALUE _callafter(int argc,VALUE *argv,VALUE self)
{
#ifdef wxHAS_CALL_AFTER
	_self->QueueEvent(new wxAsyncMethodCallEvent1<RubyCallAfter, VALUE>(
			new RubyCallAfter(rb_block_proc()), &RubyCallAfter::call, self
		));
#else

#endif
	return Qnil;

}

}
}


DLL_LOCAL void Init_WXEvtHandler(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
#endif

	using namespace RubyWX::EvtHandler;
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_define_method(rb_mWXEvtHandler,"bind",RUBY_METHOD_FUNC(_bind),-1);
	rb_define_method(rb_mWXEvtHandler,"call",RUBY_METHOD_FUNC(_call),-1);

	rb_define_method(rb_mWXEvtHandler,"call_after",RUBY_METHOD_FUNC(_callafter),-1);

	//because only Evthandler are created different
	registerInfo<wxEvtHandler>(rb_mWXEvtHandler);

	rwx_IDcall = rb_intern("call");
	rwx_IDid = rb_intern("id");
}



