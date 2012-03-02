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

#define _self wrap<wxMenu*>(self)

VALUE rb_cWXMenu;


namespace RubyWX {
namespace Menu {

macro_attr(Title,wxString)
macro_attr(Parent,wxMenu*)
macro_attr(EventHandler,wxEvtHandler*)

singlereturn(AppendSeparator)
singlereturn(PrependSeparator)



VALUE _alloc(VALUE self)
{
	if(ruby_app_inited)
		return wrap(new wxMenu,self);
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;

}

VALUE _initialize(VALUE self,VALUE title)
{
	_setTitle(self,title);
	if(rb_block_given_p()){
		rb_yield(self);
	}
	return self;
}

singlereturn(GetMenuBar);

VALUE _each(VALUE self)
{
	int count = _self->GetMenuItemCount();
	for(int i = 0;i < count;++i)
		rb_yield(wrap(_self->FindItemByPosition(i)));
	return self;
}


VALUE _appendNormalItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help,temp;

	rb_scan_args(argc, argv, "1*",&id,&temp);

	if(rb_obj_is_kind_of(id,rb_cString) && rb_block_given_p()){
		rb_scan_args(argc, argv, "11",&text,&help);
		wxMenu *m = new wxMenu;
		rb_yield(wrap(m));
		return wrap(_self->AppendSubMenu(m,wrap<wxString>(text),wrap<wxString>(help)));
	}else{
		rb_scan_args(argc, argv, "12",&id,&text,&help);
		if(!wxIsStockID(NUM2INT(id)) && NIL_P(text))
			rb_raise(rb_eArgError,"id %d needs an text",NUM2INT(id));
		wxMenuItem *item = _self->Append(NUM2INT(id),wrap<wxString>(text),wrap<wxString>(help));
		if(rb_block_given_p()){
			VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
	_self->Bind(wxEVT_COMMAND_MENU_SELECTED,RubyFunctor(proc),item->GetId());
#else
	_self->Connect(item->GetId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
#endif
		}
		return wrap(item);
	}
}


VALUE _appendCheckItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help;
	rb_scan_args(argc, argv, "12",&id,&text,&help);
	return wrap(_self->AppendCheckItem(NUM2INT(id),wrap<wxString>(text),wrap<wxString>(help)));
}

VALUE _appendRadioItem(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,help;
	rb_scan_args(argc, argv, "12",&id,&text,&help);
	return wrap(_self->AppendRadioItem(NUM2INT(id),wrap<wxString>(text),wrap<wxString>(help)));
}

VALUE _appendShift(VALUE self,VALUE val)
{
	if(!wxIsStockID(NUM2INT(val)))
		rb_raise(rb_eArgError,"id %d cant be fast added",NUM2INT(val));
	_self->Append(NUM2INT(val));
	return self;
}


}
}


void Init_WXMenu(VALUE rb_mWX)
{
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
	rb_define_method(rb_cWXMenu,"appendNormal",RUBY_METHOD_FUNC(_appendNormalItem),-1);
	rb_define_method(rb_cWXMenu,"appendCheck",RUBY_METHOD_FUNC(_appendCheckItem),-1);
	rb_define_method(rb_cWXMenu,"appendRadio",RUBY_METHOD_FUNC(_appendRadioItem),-1);

	rb_define_method(rb_cWXMenu,"<<",RUBY_METHOD_FUNC(_appendShift),1);

	rb_define_method(rb_cWXMenu,"menubar",RUBY_METHOD_FUNC(_GetMenuBar),0);
}
