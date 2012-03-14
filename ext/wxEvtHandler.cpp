/*
 * wxEvtHandler.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"
#include "wxEvent.hpp"

#if wxUSE_COLOURPICKERCTRL
	#include <wx/clrpicker.h>
#endif

#define _self wrap<wxEvtHandler*>(self)

VALUE rb_mWXEvtHandler;

std::map<VALUE,wxEvtHandler*> evthandlerholder;

std::map<ID,wxEventType> evttypeholder;

std::map<wxEventType,VALUE> evttypeclassholder;

wxEventType unwrapEventType(VALUE type)
{
	if(!SYMBOL_P(type))
		return NUM2INT(type);

	std::map<ID,wxEventType>::iterator it = evttypeholder.find(SYM2ID(type));
	if(it != evttypeholder.end())
		return it->second;

	return wxEVT_NULL;
//	if(id==rb_intern("button_clicked"))
//		return wxEVT_COMMAND_BUTTON_CLICKED;
}

void registerEventType(const char *sym, wxEventType type,VALUE klass)
{
	evttypeholder.insert(std::make_pair(rb_intern(sym),type));
	evttypeclassholder.insert(std::make_pair(type,klass));
}

VALUE getEvtObj(wxEvtHandler *handler,VALUE klass)
{
	if(!handler)
		return Qnil;

	std::map<VALUE,wxEvtHandler*>::iterator it;
	for(it = evthandlerholder.begin();it != evthandlerholder.end();++it)
	{
		if(it->second == handler)
			return it->first;
	}
	if(NIL_P(klass))
		return Qnil;
	VALUE result = wrap(handler,klass);
	evthandlerholder.insert(std::make_pair(result,handler));
	return result;
}

void RubyFunctor::operator()( wxEvent & event )
{
	event.Skip(RTEST(rb_funcall(mValue,rb_intern("call"),1,wrap(event.Clone()))));
}
void RubyFunctor::operator()( wxCommandEvent & event )
{
	event.Skip(RTEST(rb_funcall(mValue,rb_intern("call"),1,wrap(event.Clone()))));
}
void RubyFunctor::operator()( wxTimerEvent & event )
{
	event.Skip(RTEST(rb_funcall(mValue,rb_intern("call"),1,wrap(event.Clone()))));
}

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
	_self->Connect(unwrapID(id),unwrapID(last),unwrapEventType(type),(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
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
}



