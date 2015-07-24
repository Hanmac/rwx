/*
 * wxMenuBar.cpp
 *
 *  Created on: 24.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"

#include "wxMenu.hpp"
#include "wxEvtHandler.hpp"

VALUE rb_cWXMenuBar;

#if wxUSE_MENUS
#define _self unwrap<wxMenuBar*>(self)

namespace RubyWX {
namespace MenuBar {

APP_PROTECT(wxMenuBar)

singlereturn(GetMenuCount)
singlereturn(IsAttached)

singlereturn(GetFrame)

singlefunc(Detach)


DLL_LOCAL VALUE _SetFrame(VALUE self,VALUE frame)
{
	rb_check_frozen(self);

	if(_self->IsAttached())
		_self->Detach();

	if(!NIL_P(frame)) {
		_self->Attach(unwrap<wxFrame*>(frame));
	}

	return frame;
}


macro_attr_item_simple(MenuLabel, GetMenuCount, wxString)
macro_attr_item(EnableTop, IsEnabledTop, EnableTop, GetMenuCount, bool)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetMenuCount));
	for(std::size_t i = 0;i < _self->GetMenuCount();++i)
		rb_yield(wrap(_self->GetMenu(i)));
	return self;
}

DLL_LOCAL wxMenuItem* _getItemBase(wxMenuBar *menu, VALUE val)
{
	return menu->FindItem(unwrapID(val));
}


wxMenu* add_base(VALUE menu,wxString &name)
{
	wxMenu *m = NULL;
	if(!rb_obj_is_kind_of(menu,rb_cWXMenu))
	{
		m = new wxMenu;
		if(rb_block_given_p())
			rb_yield(wrap(m));

		name = unwrap<wxString>(menu);

		if(SYMBOL_P(menu))
		{
			wxWindowID id(unwrapID(menu));
			if(wxIsStockID(id))
			{
				name = wxGetStockLabel(id);
			}
		}

	} else {
		m = unwrap<wxMenu*>(menu);
		name = m->GetTitle();
	}

	if(name.IsEmpty())
		rb_raise(rb_eArgError,"Menu must have a title.");

	return m;
}

DLL_LOCAL VALUE _appendSelf(VALUE self,VALUE menu)
{
	wxString name(wxEmptyString);

	_self->Append(add_base(menu,name),name);
	return self;
}


/*
 * call-seq:
 *   append(menu) -> true/false
 *   append(title) {|menu| } -> true/false
 * ===Arguments
 * * menu WX::Menu
 * * title String
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _append(VALUE self,VALUE menu)
{
	wxString name(wxEmptyString);

	return wrap(_self->Append(add_base(menu,name),name));
}


/*
 * call-seq:
 *   insert(idx, menu) -> true/false
 *   insert(idx, title) {|menu| } -> true/false
 * ===Arguments
 * * idx Integer
 * * menu WX::Menu
 * * title String
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _insert(VALUE self,VALUE idx,VALUE menu)
{
	wxString name(wxEmptyString);
	int cidx = NUM2INT(idx);

	if(check_index(cidx,_self->GetMenuCount()+1))
		return wrap(_self->Insert(cidx,add_base(menu,name),name));

	return Qnil;
}

/*
 * call-seq:
 *   replace(idx, menu) -> WX::Menu
 *   replace(idx, title) {|menu| } -> WX::Menu
 * ===Arguments
 * * idx Integer
 * * menu WX::Menu
 * * title String
 *
 * ===Return value
 * WX::Menu
 */
DLL_LOCAL VALUE _replace(VALUE self,VALUE idx,VALUE menu)
{
	wxString name(wxEmptyString);
	int cidx = NUM2INT(idx);

	if(check_index(cidx,_self->GetMenuCount()))
		return wrap(_self->Replace(cidx,add_base(menu,name),name));

	return Qnil;
}

/*
 * call-seq:
 *   prepend(menu) -> true/false
 *   prepend(title) {|menu| } -> true/false
 * ===Arguments
 * * menu WX::Menu
 * * title String
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _prepend(VALUE self,VALUE menu)
{
	wxString name(wxEmptyString);

	return wrap(_self->Insert(0,add_base(menu,name),name));
}

/*
 * call-seq:
 *   remove(idx) -> WX::Menu
 * ===Arguments
 * * idx Integer
 * * menu WX::Menu
 * * title String
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _remove(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);

	if(check_index(cidx,_self->GetMenuCount()))
		return wrap(_self->Remove(cidx));

	return Qnil;
}

}
}

#endif

DLL_LOCAL void Init_WXMenuBar(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

#endif
#if wxUSE_MENUS
	using namespace RubyWX::MenuBar;
	rb_cWXMenuBar = rb_define_class_under(rb_mWX,"MenuBar",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXMenuBar,_alloc);

	rb_define_method(rb_cWXMenuBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXMenuBar,"each_menu",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXMenuBar,"<<",RUBY_METHOD_FUNC(_appendSelf),1);

	rb_define_method(rb_cWXMenuBar,"append",RUBY_METHOD_FUNC(_append),1);
	rb_define_method(rb_cWXMenuBar,"insert",RUBY_METHOD_FUNC(_insert),2);
	rb_define_method(rb_cWXMenuBar,"prepend",RUBY_METHOD_FUNC(_prepend),1);

	rb_define_method(rb_cWXMenuBar,"replace",RUBY_METHOD_FUNC(_replace),2);
	rb_define_method(rb_cWXMenuBar,"remove",RUBY_METHOD_FUNC(_remove),1);

	rb_define_method(rb_cWXMenuBar,"frame",RUBY_METHOD_FUNC(_GetFrame),0);

	rb_define_const(rb_cWXMenuBar,"DOCKABLE",INT2NUM(wxMB_DOCKABLE));

	registerInfo<wxMenuBar>(rb_cWXMenuBar);
#endif
}
