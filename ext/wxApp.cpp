/*
 * wxApp.cpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"
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
	return unwrapPtr<RubyApp>(vapp, rb_cWXApp);
}


class AppAfter
#ifndef wxHAS_EVENT_BIND
: public wxEvtHandler
#endif
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




RubyApp::RubyApp(VALUE klass)
{
	mRuby = wrapPtr(this,klass);
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

#ifdef wxHAS_EVENT_BIND
	this->Bind(wxEVT_ASYNC_METHOD_CALL,AppAfter());
#else
	this->Connect(wxEVT_ASYNC_METHOD_CALL,wxEventHandler(AppAfter::operator()),NULL,new AppAfter());
#endif

	ruby_app_inited = true;

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


VALUE _alloc(VALUE self)
{
	return (new RubyApp(self))->mRuby;
}


VALUE _main_loop(VALUE self)
{
	wxChar* argv[] = { NULL};
	int argc = 0;
	return INT2NUM(wxEntry(argc, argv));
}

VALUE _on_init(VALUE self)
{
	return Qtrue; //For wxWidgets to continue
}

VALUE _on_run(VALUE self)
{
	return Qnil;
}

VALUE _on_exit(VALUE self)
{
	return Qnil;
}

VALUE _wxExit(VALUE self)
{
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


void Init_WXApp(VALUE rb_mWX)
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
}

