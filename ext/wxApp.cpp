/*
 * wxApp.cpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"
#include "wxBrush.hpp"
#include "wxPen.hpp"
#include "wxPropertyGrid.hpp"

#ifdef __WXMAC__
    
//#ifdef __WXCARBON__
#include <Carbon/Carbon.h>
//#else
//#include <Cocoa/Cocoa.h>
//#endif
    extern "C" { void CPSEnableForegroundOperation(ProcessSerialNumber* psn); }
#endif

#define _self unwrap<wxApp*>(self)
VALUE rb_cWXApp;

bool ruby_app_inited;

template <>
wxApp* unwrap< wxApp* >(const VALUE &vapp)
{
	return unwrapTypedPtr<RubyApp>(vapp, rb_cWXApp);
}


class AppAfter
{
public:

	void operator()(wxEvent &evt)
	{
		wxDynamicCast(&evt,wxAsyncMethodCallEvent)->Execute();
	}

	void operator()(wxAsyncMethodCallEvent &evt)
	{
		evt.Execute();
	}

};

void app_protected()
{
	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%" PRIsVALUE " is not running.",RB_CLASSNAME(rb_cWXApp));
}


void rwx_sigint(int)
{
	wxExit();
}

RubyApp::RubyApp(VALUE klass)
{
	mRuby = wrapTypedPtr(this,klass);
}

bool RubyApp::OnInit()
{
	wxApp::OnInit();

#ifdef __WXMAC__
     ProcessSerialNumber psn;
 
     GetCurrentProcess( &psn );
     CPSEnableForegroundOperation( &psn );
     SetFrontProcess( &psn );
#endif


	RubyWX::Color::define_const();
	RubyWX::Font::define_const();
	RubyWX::Brush::define_const();
	RubyWX::Pen::define_const();

#if wxUSE_INTL
	wxLocale::CreateLanguagesDB();
	mLocale = new wxLocale(wxLANGUAGE_DEFAULT);
	mLocale->AddCatalog("wxstd");
#ifdef __LINUX__
	mLocale->AddCatalog("fileutils");
#endif
#endif
#if wxUSE_PROPGRID
	wxPropertyGrid::RegisterAdditionalEditors();
#endif

	this->Bind(wxEVT_ASYNC_METHOD_CALL,AppAfter());

	ruby_app_inited = true;
	// binds SIGINT to wxExit to close the application
#if defined(__UNIX__) && !defined(__WINDOWS__)
	SetSignalHandler(wxSIGINT, rwx_sigint);
#endif

	bool result = RTEST(rb_funcall(mRuby, rb_intern("on_init"), 0));
	if(rb_block_given_p())
		rb_obj_instance_eval(0, 0, mRuby);
	return result;
}


int RubyApp::OnRun()
{
	int result = wxApp::OnRun();
	rb_funcall(mRuby, rb_intern("on_run"), 0);
	return result;
}


int RubyApp::OnExit()
{
	int result = wxApp::OnExit();
	ruby_app_inited = false;
	rb_funcall(mRuby, rb_intern("on_exit"), 0);
	return result;
}


namespace RubyWX {
namespace App {

macro_attr(AppName,wxString)
macro_attr(AppDisplayName,wxString)
macro_attr(ClassName,wxString)
macro_attr(VendorName,wxString)
macro_attr(VendorDisplayName,wxString)


DLL_LOCAL VALUE _alloc(VALUE self)
{
	return (new RubyApp(self))->mRuby;
}


DLL_LOCAL VALUE _main_loop(VALUE self)
{
	wxChar* argv[] = { NULL};
	int argc = 0;
	return INT2NUM(wxEntry(argc, argv));
}

DLL_LOCAL VALUE _on_init(VALUE self)
{
	return Qtrue; //For wxWidgets to continue
}

DLL_LOCAL VALUE _on_run(VALUE self)
{
	return Qnil;
}

DLL_LOCAL VALUE _on_exit(VALUE self)
{
	return Qnil;
}

DLL_LOCAL VALUE _wxExit(VALUE self)
{
	app_protected();
	wxExit();
	return self;
}


VALUE  singleton_wxApp(VALUE self)
{
	_main_loop(_alloc(rb_cWXApp));
	return Qnil;
}

}
}


DLL_LOCAL void Init_WXApp(VALUE rb_mWX)
{
	using namespace RubyWX::App;
	rb_cWXApp = rb_define_class_under(rb_mWX,"App",rb_cObject);
	rb_define_alloc_func(rb_cWXApp,_alloc);

	rb_define_attr_method(rb_cWXApp,"app_name",_getAppName,_setAppName);
	rb_define_attr_method(rb_cWXApp,"app_display_name",_getAppDisplayName,_setAppDisplayName);

	rb_define_attr_method(rb_cWXApp,"vendor_name",_getVendorName,_setVendorName);
	rb_define_attr_method(rb_cWXApp,"vendor_display_name",_getVendorDisplayName,_setVendorDisplayName);


	rb_define_method(rb_cWXApp,"main_loop",RUBY_METHOD_FUNC(_main_loop),0);

	rb_define_method(rb_cWXApp,"on_init",RUBY_METHOD_FUNC(_on_init),0);
	rb_define_method(rb_cWXApp,"on_run",RUBY_METHOD_FUNC(_on_run),0);
	rb_define_method(rb_cWXApp,"on_exit",RUBY_METHOD_FUNC(_on_exit),0);


	rb_define_singleton_method(rb_mWX,"App",RUBY_METHOD_FUNC(singleton_wxApp),0);

	rb_define_module_function(rb_mWX,"exit",RUBY_METHOD_FUNC(_wxExit),0);

	registerDataType(rb_cWXApp);

}

