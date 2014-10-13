/*
 * wxPreferences.cpp
 *
 *  Created on: 25.05.2013
 *      Author: hanmac
 */

#include "main.hpp"

#include "wxApp.hpp"

#ifdef wxHAS_PREF_EDITOR_ICONS
#include "wxBitmap.hpp"
#endif

VALUE rb_cWXPreferences;

#ifdef HAVE_WX_PREFERENCES_H

#include <wx/preferences.h>

template <>
VALUE wrap< wxPreferencesEditor >(wxPreferencesEditor *point )
{
	return wrapTypedPtr(point, rb_cWXPreferences);
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
	RubyPreferencesPage(VALUE block,VALUE klass,const wxString &title, VALUE bitmap) :
		RubyPreferencesPageInterface(block,klass),wxPreferencesPage(),mtitle(title)
#ifdef wxHAS_PREF_EDITOR_ICONS
	,mBitmap(unwrap<wxBitmap>(bitmap))
#endif
	{}

	//using RubyPreferencesPageInterface::CreateWindow;
	wxWindow *CreateWindow(wxWindow *parent)
	{
		return RubyPreferencesPageInterface::CreateWindow(parent);
	}

#ifdef wxHAS_PREF_EDITOR_ICONS
	virtual wxBitmap GetLargeIcon() const { return mBitmap; };
#endif

	wxString GetName() const {return mtitle;}
private:
	wxString mtitle;

#ifdef wxHAS_PREF_EDITOR_ICONS
	wxBitmap mBitmap;
#endif
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
	VALUE kind, klass, block, bitmap;
	rb_scan_args(argc, argv, "21&",&kind,&klass,&bitmap,&block);
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
		page = new RubyPreferencesPage(block,klass,unwrap<wxString>(kind), bitmap);
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

