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
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_size));
	std::size_t count = _self->GetMenuItemCount();
	for(std::size_t i = 0;i < count;++i)
		rb_yield(wrap(_self->FindItemByPosition(i)));
	return self;
}

void bind_callback(wxMenu* menu,wxWindowID id)
{
	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
		menu->Bind(wxEVT_MENU,RubyFunctor(proc),id);
	}
}

DLL_LOCAL bool check_title(wxWindowID wid, VALUE id, VALUE text)
{
	if(!wxIsStockID(wid) && (NIL_P(text) || rb_str_length(text) == INT2FIX(0)))
	{
		rb_raise(rb_eArgError,"id %"PRIsVALUE"s (%d) needs an text", id, wid);
		return false;
	}
	return true;
}

DLL_LOCAL bool check_menu_mitle(wxMenu *m,wxString &wtext)
{
	if(wtext.empty())
	{
		if(m->GetTitle().empty())
		{
			rb_raise(rb_eArgError,"menu must have a title");
			return false;
		}

		wtext = m->GetTitle();
	}
	return true;
}

DLL_LOCAL VALUE _append_base(int argc,VALUE *argv,VALUE self,wxItemKind kind)
{
	VALUE id,text,help;

	rb_scan_args(argc, argv, "12",&id,&text,&help);
	wxWindowID wid(unwrapID(id));

	if(check_title(wid,id,text))
	{
		wxMenuItem *item = _self->Append(wid,unwrap<wxString>(text),unwrap<wxString>(help),kind);
		bind_callback(_self,item->GetId());
		return wrap(item);
	}
	return Qnil;
}


/*
 * call-seq:
 *   add_normal(id, text, [help]) -> WX::Menu::Item
 *   add_normal(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * adds a new normal menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the system defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _appendNormalItem(int argc,VALUE *argv,VALUE self)
{
	return _append_base(argc,argv,self,wxITEM_NORMAL);
}

/*
 * call-seq:
 *   add_check(id, text, [help]) -> WX::Menu::Item
 *   add_check(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * adds a new check menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _appendCheckItem(int argc,VALUE *argv,VALUE self)
{
	return _append_base(argc,argv,self,wxITEM_CHECK);
}

/*
 * call-seq:
 *   add_radio(id, text, [help]) -> WX::Menu::Item
 *   add_radio(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * adds a new radio menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _appendRadioItem(int argc,VALUE *argv,VALUE self)
{
	return _append_base(argc,argv,self,wxITEM_RADIO);
}

/*
 * call-seq:
 *   add_menu(text, [help]) {|menu| ... } -> WX::Menu::Item
 *   add_menu(menu, text, [help]) -> WX::Menu::Item
 *
 * adds a new sub menu item to the Menu widget.
 * when block is given, it created a new menu and yields it into the block.
 * ===Arguments
 * * text is the Label of the menu item. String
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when either menu hasn't a title and text is empty
 * [TypeError]
 * * when menu is nil
 *
*/
DLL_LOCAL VALUE _append_menu(int argc,VALUE *argv,VALUE self)
{
	VALUE text,help,menu;

	wxMenu *m = NULL;
	if(rb_block_given_p()){
		rb_scan_args(argc, argv, "11",&text,&help);
		m = new wxMenu;
		rb_yield(wrap(m));
	}else{
		rb_scan_args(argc, argv, "12",&menu,&text,&help);

		if(!nil_check(menu,"menu"))
			return Qnil;

		m = unwrap<wxMenu*>(menu);
	}

	wxString wtext(unwrap<wxString>(text));

	if(check_menu_mitle(m,wtext))
		return wrap(_self->AppendSubMenu(m,wtext,unwrap<wxString>(help)));

	return Qnil;

}

DLL_LOCAL VALUE _appendShift(VALUE self,VALUE val)
{
	if(rb_obj_is_kind_of(val,rb_cWXMenuItem)) {
		_self->Append(unwrap<wxMenuItem*>(val));
	} else {
		wxWindowID id = unwrapID(val);
		if(!wxIsStockID(id))
			rb_raise(rb_eArgError,"id \"%"PRIsVALUE"\" cant be fast added", val);
		_self->Append(id);
	}
	return self;
}



DLL_LOCAL VALUE _insert_base(int argc,VALUE *argv,VALUE self,wxItemKind kind)
{
	VALUE idx,id,text,help;
	rb_scan_args(argc, argv, "22",&idx,&id,&text,&help);

	wxWindowID wid(unwrapID(id));

	if(check_title(wid,id,text))
	{
		int cidx = NUM2INT(idx);
		if(check_index(cidx,_self->GetMenuItemCount()+1))
		{
			wxMenuItem *item = _self->Insert(cidx,wid,unwrap<wxString>(text),unwrap<wxString>(help),kind);
			bind_callback(_self,item->GetId());
			return wrap(item);
		}
	}
	return Qnil;
}


/*
 * call-seq:
 *   insert_normal(pos, id, text, [help]) -> WX::Menu::Item
 *   insert_normal(pos, id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * inserts a new normal menu item to the Menu widget to the given position.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * pos where the item should be added.
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 * [IndexError]
 * * pos is greater than the count of MenuItems
 *
*/
DLL_LOCAL VALUE _insertNormalItem(int argc,VALUE *argv,VALUE self)
{
	return _insert_base(argc,argv,self,wxITEM_NORMAL);
}

/*
 * call-seq:
 *   insert_check(pos, id, text, [help]) -> WX::Menu::Item
 *   insert_check(pos, id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * inserts a new check menu item to the Menu widget to the given position.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * pos where the item should be added.
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 * [IndexError]
 * * pos is greater than the count of MenuItems
 *
*/
DLL_LOCAL VALUE _insertCheckItem(int argc,VALUE *argv,VALUE self)
{
	return _insert_base(argc,argv,self,wxITEM_CHECK);
}

/*
 * call-seq:
 *   insert_radio(pos, id, text, [help]) -> WX::Menu::Item
 *   insert_radio(pos, id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * inserts a new radio menu item to the Menu widget to the given position.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * pos where the item should be added.
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 * [IndexError]
 * * pos is greater than the count of MenuItems
 *
*/
DLL_LOCAL VALUE _insertRadioItem(int argc,VALUE *argv,VALUE self)
{
	return _insert_base(argc,argv,self,wxITEM_RADIO);
}

/*
 * call-seq:
 *   insert_menu(pos, text, [help]) {|menu| ... } -> WX::Menu::Item
 *   insert_menu(pos, menu, text, [help]) -> WX::Menu::Item
 *
 * insert a new sub menu item to the Menu widget into the given position.
 * when block is given, it created a new menu and yields it into the block.
 * ===Arguments
 * * pos where the item should be added.
 * * text is the Label of the menu item. String
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when either menu hasn't a title and text is empty
 * [TypeError]
 * * when menu is nil
 * [IndexError]
 * * pos is greater than the count of MenuItems
 *
*/
DLL_LOCAL VALUE _insert_menu(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,text,help,menu;

	wxMenu *m = NULL;
	if(rb_block_given_p()){
		rb_scan_args(argc, argv, "21",&idx,&text,&help);
		m = new wxMenu;
		rb_yield(wrap(m));
	}else{
		rb_scan_args(argc, argv, "22",&idx,&menu,&text,&help);

		if(!nil_check(menu,"menu"))
			return Qnil;

		m = unwrap<wxMenu*>(menu);
	}

	wxString wtext(unwrap<wxString>(text));

	if(check_menu_mitle(m,wtext))
	{
		int cidx = NUM2INT(idx);
		if(check_index(cidx,_self->GetMenuItemCount()+1))
		{
			return wrap(_self->Insert(cidx,wxID_ANY,wtext,m,unwrap<wxString>(help)));
		}
	}
	return Qnil;
}


DLL_LOCAL VALUE _prepend_base(int argc,VALUE *argv,VALUE self,wxItemKind kind)
{
	VALUE id,text,help;
	rb_scan_args(argc, argv, "12",&id,&text,&help);

	wxWindowID wid(unwrapID(id));

	if(check_title(wid,id,text))
	{
		wxMenuItem *item = _self->Prepend(wid,unwrap<wxString>(text),unwrap<wxString>(help),kind);
		bind_callback(_self,item->GetId());
		return wrap(item);
	}
	return Qnil;
}

/*
 * call-seq:
 *   prepend_normal(id, text, [help]) -> WX::Menu::Item
 *   prepend_normal(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * prepends a new normal menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _prependNormalItem(int argc,VALUE *argv,VALUE self)
{
	return _prepend_base(argc,argv,self,wxITEM_NORMAL);
}

/*
 * call-seq:
 *   prepend_check(id, text, [help]) -> WX::Menu::Item
 *   prepend_check(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * prepends a new check menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _prependCheckItem(int argc,VALUE *argv,VALUE self)
{
	return _prepend_base(argc,argv,self,wxITEM_CHECK);
}

/*
 * call-seq:
 *   prepend_radio(id, text, [help]) -> WX::Menu::Item
 *   prepend_radio(id, text, [help]) {|event| ... } -> WX::Menu::Item
 *
 * prepends a new radio menu item to the Menu widget.
 * when block is given, bind the block to the event of the menu item.
 * ===Arguments
 * * id of the menu item: Symbol/Integer/nil
 * * text is the Label of the menu item. String (can be nil if id is one of the pre defined)
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when id is not a StockID and text is nil or empty
 *
*/
DLL_LOCAL VALUE _prependRadioItem(int argc,VALUE *argv,VALUE self)
{
	return _prepend_base(argc,argv,self,wxITEM_RADIO);
}

/*
 * call-seq:
 *   prepend_menu(text, [help]) {|menu| ... } -> WX::Menu::Item
 *   prepend_menu(menu, text, [help]) -> WX::Menu::Item
 *
 * prepends a new sub menu item to the Menu widget.
 * when block is given, it created a new menu and yields it into the block.
 * ===Arguments
 * * text is the Label of the menu item. String
 * * help shown in WX::StatusBar. String
 * ===Return value
 * WX::Menu::Item
 * === Exceptions
 * [ArgumentError]
 * * when either menu hasn't a title and text is empty
 * [TypeError]
 * * when menu is nil
 *
*/
DLL_LOCAL VALUE _prepend_menu(int argc,VALUE *argv,VALUE self)
{
	VALUE text,help,menu;

	wxMenu *m = NULL;
	if(rb_block_given_p()){
		rb_scan_args(argc, argv, "11",&text,&help);
		m = new wxMenu;
		rb_yield(wrap(m));
	}else{
		rb_scan_args(argc, argv, "12",&menu,&text,&help);

		if(!nil_check(menu,"menu"))
			return Qnil;

		m = unwrap<wxMenu*>(menu);
	}

	wxString wtext(unwrap<wxString>(text));

	if(check_menu_mitle(m,wtext))
		return wrap(_self->Prepend(wxID_ANY,wtext,m,unwrap<wxString>(help)));

	return Qnil;
}


#if wxUSE_XRC
DLL_LOCAL VALUE _load_xrc(VALUE self,VALUE name)
{
	return wrap(wxXmlResource::Get()->LoadMenu(unwrap<wxString>(name)));
}
#endif

}
}
#endif

DLL_LOCAL void Init_WXMenu(VALUE rb_mWX)
{
#if 0
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_define_attr(rb_cWXMenu,"title",1,1);
	rb_define_attr(rb_cWXMenu,"parent",1,1);

#endif

#if wxUSE_MENUS
	using namespace RubyWX::Menu;
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
	rb_define_method(rb_cWXMenu,"append_menu",RUBY_METHOD_FUNC(_append_menu),-1);

	rb_define_method(rb_cWXMenu,"append_separator",RUBY_METHOD_FUNC(_AppendSeparator),0);

	rb_define_method(rb_cWXMenu,"insert_normal",RUBY_METHOD_FUNC(_insertNormalItem),-1);
	rb_define_method(rb_cWXMenu,"insert_check",RUBY_METHOD_FUNC(_insertCheckItem),-1);
	rb_define_method(rb_cWXMenu,"insert_radio",RUBY_METHOD_FUNC(_insertRadioItem),-1);
	rb_define_method(rb_cWXMenu,"insert_menu",RUBY_METHOD_FUNC(_insert_menu),-1);

	rb_define_method(rb_cWXMenu,"prepend_normal",RUBY_METHOD_FUNC(_prependNormalItem),-1);
	rb_define_method(rb_cWXMenu,"prepend_check",RUBY_METHOD_FUNC(_prependCheckItem),-1);
	rb_define_method(rb_cWXMenu,"prepend_radio",RUBY_METHOD_FUNC(_prependRadioItem),-1);
	rb_define_method(rb_cWXMenu,"prepend_menu",RUBY_METHOD_FUNC(_prepend_menu),-1);

	rb_define_method(rb_cWXMenu,"prepend_separator",RUBY_METHOD_FUNC(_PrependSeparator),0);


	rb_define_method(rb_cWXMenu,"<<",RUBY_METHOD_FUNC(_appendShift),1);

	rb_define_method(rb_cWXMenu,"menubar",RUBY_METHOD_FUNC(_GetMenuBar),0);

#if wxUSE_XRC
	rb_define_singleton_method(rb_cWXMenu,"load_xrc",RUBY_METHOD_FUNC(_load_xrc),1);
#endif

	registerEventType<wxCommandEvent>("menu",wxEVT_MENU);

	registerInfo<wxMenu>(rb_cWXMenu);
#endif
}
