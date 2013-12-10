/*
 * wxPreferences.cpp
 *
 *  Created on: 25.05.2013
 *      Author: hanmac
 */

#include "main.hpp"

#include "wxApp.hpp"

VALUE rb_cWXPreferences;

#ifdef HAVE_WX_PREFERENCES_H

#include <wx/preferences.h>

template <>
VALUE wrap< wxPreferencesEditor >(wxPreferencesEditor *point )
{
	return Data_Wrap_Struct(rb_cWXPreferences, NULL, NULL, point);
}

#define _self unwrap<wxPreferencesEditor*>(self)

class RubyPreferencesPageInterface
{
public:
	RubyPreferencesPageInterface(VALUE block,VALUE klass) :
		mblock(block),mklass(klass) {}

	virtual ~RubyPreferencesPageInterface() {}

	virtual wxWindow *CreateWindow(wxWindow *parent)
	{
		//VALUE argv[] = {wrap(parent)};
		//VALUE result = rb_class_new_instance(1,argv,mklass);

		VALUE result = rb_funcall(mklass,rb_intern("new"),1,wrap(parent));

		if(!NIL_P(mblock))
			rb_funcall(mblock,rb_intern("call"),1,result);

		return unwrap<wxWindow*>(result);
	}

private:
	VALUE mblock;
	VALUE mklass;
};

class RubyPreferencesPage : public RubyPreferencesPageInterface,public wxPreferencesPage
{
public:
	RubyPreferencesPage(VALUE block,VALUE klass,const wxString &title) :
		RubyPreferencesPageInterface(block,klass),wxPreferencesPage(),mtitle(title) {}

	//using RubyPreferencesPageInterface::CreateWindow;
	wxWindow *CreateWindow(wxWindow *parent)
	{
		return RubyPreferencesPageInterface::CreateWindow(parent);
	}

	wxString GetName() const {return mtitle;}
private:
	wxString mtitle;
};


class RubyStockPreferencesPage : public RubyPreferencesPageInterface, public wxStockPreferencesPage
{
public:
	RubyStockPreferencesPage(VALUE block,VALUE klass,wxStockPreferencesPage::Kind kind) :
		RubyPreferencesPageInterface(block,klass), wxStockPreferencesPage(kind) {}

	//using RubyPreferencesPageInterface::CreateWindow;
	wxWindow *CreateWindow(wxWindow *parent)
	{
		return RubyPreferencesPageInterface::CreateWindow(parent);
	}
};

namespace RubyWX {
namespace Preferences {

DLL_LOCAL VALUE _add_page(int argc,VALUE *argv,VALUE self)
{
	VALUE kind, klass,block;
	rb_scan_args(argc, argv, "20&",&kind,&klass,&block);
	wxPreferencesPage *page = NULL;
	//VALUE block = rb_block_proc();
	if(SYMBOL_P(kind))
	{
		wxStockPreferencesPage::Kind wxkind = wxStockPreferencesPage::Kind_General;
		if(SYM2ID(kind) == rb_intern("general"))
			wxkind = wxStockPreferencesPage::Kind_General;
		if(SYM2ID(kind) == rb_intern("advanced"))
			wxkind = wxStockPreferencesPage::Kind_Advanced;
		page = new RubyStockPreferencesPage(block,klass,wxkind);
	}else
	{
		page = new RubyPreferencesPage(block,klass,unwrap<wxString>(kind));
	}

	_self->AddPage(page);
	return self;
}

DLL_LOCAL VALUE _preferences(int argc,VALUE *argv,VALUE self)
{
	VALUE window, title;
	rb_scan_args(argc, argv, "11",&window,&title);

	app_protected();

	wxPreferencesEditor *pref = new wxPreferencesEditor(unwrap<wxString>(title));

	if(rb_block_given_p())
	{
		rb_funcall(rb_block_proc(),rb_intern("call"),1,wrap(pref));
		//rb_yield(wrap(pref));
		pref->Show(unwrap<wxWindow*>(window));
	}

	return self;
}

}

}

#endif

DLL_LOCAL void Init_WXPreferences(VALUE rb_mWX)
{
#ifdef HAVE_WX_PREFERENCES_H
	using namespace RubyWX::Preferences;
	rb_cWXPreferences = rb_define_class_under(rb_mWX,"Preferences",rb_cObject);

	rb_undef_alloc_func(rb_cWXPreferences);

	rb_define_method(rb_cWXPreferences,"add_page",RUBY_METHOD_FUNC(_add_page),-1);

	rb_define_module_function(rb_mWX,"preferences",RUBY_METHOD_FUNC(_preferences),-1);
	registerType<wxPreferencesEditor>(rb_cWXPreferences);
#endif

}

