/*
 * wxMenu.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */



#include "wxMenu.hpp"
#include "wxMenuItem.hpp"
#include "wxEvtHandler.hpp"
#include "wxApp.hpp"

VALUE rb_cWXMenu;

#if wxUSE_MENUS

#define _self unwrap<wxMenu*>(self)

namespace RubyWX {
namespace Menu {

macro_attr(Title,wxString)
macro_attr(Parent,wxMenu*)
macro_attr(EventHandler,wxEvtHandler*)

singlereturn(AppendSeparator)
singlereturn(PrependSeparator)


APP_PROTECT(wxMenu)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE title;
	rb_scan_args(argc, argv, "01",&title);

	if(NIL_P(title))
		_self->SetTitle("");
	else if(SYMBOL_P(title))
		_self->SetTitle(wxGetStockLabel(unwrapID(title)));
	else
		_setTitle(self,title);
	if(rb_block_given_p()){
		rb_yield(self);
	}
	return self;
}

singlereturn(GetMenuBar);

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetMenuItemCount());
}


DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_size);
	std::size_t count = _self->GetMenuItemCount();
	for(std::size_t i = 0;i < count;++i)
		rb_yield(wrap(_self->FindItemByPosition(i)));
	return self;
}

void bind_callback(wxMenu* menu,wxWindowID id)
{
	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
		menu->Bind(wxEVT_MENU,RubyFunctor(proc),id);
#else
		menu->Connect(id,wxEVT_MENU,wxCommandEventHandler(RubyFunctor::operator()),NULL,new RubyFunctor(proc));
#endif
	}
}


DLL_LOCAL VALUE _appendNormalItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help,temp;

	rb_scan_args(argc, argv, "1*",&id,&temp);

	if(rb_obj_is_kind_of(id,rb_cString) && rb_block_given_p()){
		rb_scan_args(argc, argv, "11",&text,&help);
		wxMenu *m = new wxMenu;
		rb_yield(wrap(m));
		return wrap(_self->AppendSubMenu(m,unwrap<wxString>(text),unwrap<wxString>(help)));
	}else{
		rb_scan_args(argc, argv, "12",&id,&text,&help);
		wxWindowID wid = unwrapID(id);
		if(!wxIsStockID(wid) && NIL_P(text))
			rb_raise(rb_eArgError,"id %d needs an text",wid);
		wxMenuItem *item = _self->Append(wid,unwrap<wxString>(text),unwrap<wxString>(help));
		bind_callback(_self,item->GetId());
		return wrap(item);
	}
}


DLL_LOCAL VALUE _appendCheckItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help;
	rb_scan_args(argc, argv, "12",&id,&text,&help);
	wxMenuItem *item = _self->AppendCheckItem(unwrapID(id),unwrap<wxString>(text),unwrap<wxString>(help));
	bind_callback(_self,item->GetId());
	return wrap(item);
}

DLL_LOCAL VALUE _appendRadioItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help;
	rb_scan_args(argc, argv, "12",&id,&text,&help);
	wxMenuItem *item = _self->AppendRadioItem(unwrapID(id),unwrap<wxString>(text),unwrap<wxString>(help));
	bind_callback(_self,item->GetId());
	return wrap(item);
}

DLL_LOCAL VALUE _appendShift(VALUE self,VALUE val)
{
	wxWindowID id = unwrapID(val);
	if(!wxIsStockID(id))
		rb_raise(rb_eArgError,"id \"%s\" cant be fast added",unwrap<char*>(val));
	_self->Append(id);
	return self;
}


}
}
#endif

DLL_LOCAL void Init_WXMenu(VALUE rb_mWX)
{
#if wxUSE_MENUS
	using namespace RubyWX::Menu;
	//rb_cWXEvtHandler = rb_define_class_under(rb_mWX,"EvtHandler",rb_cObject);
	rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cObject);
	rb_define_alloc_func(rb_cWXMenu,_alloc);

	rb_define_method(rb_cWXMenu,"initialize",RUBY_METHOD_FUNC(_initialize),1);

	rb_include_module(rb_cWXMenu,rb_mWXEvtHandler);
	rb_include_module(rb_cWXMenu,rb_mEnumerable);

	rb_define_attr_method(rb_cWXMenu,"title",_getTitle,_setTitle);
	rb_define_attr_method(rb_cWXMenu,"parent",_getParent,_setParent);

	rb_define_method(rb_cWXMenu,"each",RUBY_METHOD_FUNC(_each),0);
	rb_define_method(rb_cWXMenu,"append_normal",RUBY_METHOD_FUNC(_appendNormalItem),-1);
	rb_define_method(rb_cWXMenu,"append_check",RUBY_METHOD_FUNC(_appendCheckItem),-1);
	rb_define_method(rb_cWXMenu,"append_radio",RUBY_METHOD_FUNC(_appendRadioItem),-1);

	rb_define_method(rb_cWXMenu,"append_separator",RUBY_METHOD_FUNC(_AppendSeparator),0);
	rb_define_method(rb_cWXMenu,"prepend_separator",RUBY_METHOD_FUNC(_PrependSeparator),0);

	rb_define_method(rb_cWXMenu,"<<",RUBY_METHOD_FUNC(_appendShift),1);

	rb_define_method(rb_cWXMenu,"menubar",RUBY_METHOD_FUNC(_GetMenuBar),0);

	registerEventType<wxCommandEvent>("menu",wxEVT_MENU);

	registerInfo<wxMenu>(rb_cWXMenu);
#endif
}
