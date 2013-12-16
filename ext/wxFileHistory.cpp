/*
 * wxFileHistory.cpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#include "wxFileHistory.hpp"
#include "wxApp.hpp"
#include "wxConfig.hpp"
#include "wxMenu.hpp"

VALUE rb_cWXFileHistory;
#if wxUSE_FILE_HISTORY
#define _self unwrap<wxFileHistory*>(self)

namespace RubyWX
{
namespace FileHistory
{

macro_attr(BaseId,wxWindowID)

APP_PROTECT(wxFileHistory)

DLL_LOCAL VALUE _each(VALUE self)
{
	std::size_t count = _self->GetCount();
	for(std::size_t i = 0;i < count;++i)
		rb_yield(wrap(_self->GetHistoryFile(i)));
	return self;
}

#if wxUSE_CONFIG
DLL_LOCAL VALUE _load(int argc,VALUE *argv,VALUE self)
{
	VALUE config;
	rb_scan_args(argc, argv, "01",&config);
	wxConfigBase *cconf;
	if(NIL_P(config))
		cconf = wxConfig::Get();
	else
		cconf = unwrap<wxConfig*>(config);
	_self->Load(*cconf);
	return self;
}

DLL_LOCAL VALUE _save(int argc,VALUE *argv,VALUE self)
{
	VALUE config;
	rb_scan_args(argc, argv, "01",&config);
	wxConfigBase *cconf;
	if(NIL_P(config))
		cconf = wxConfig::Get();
	else
		cconf = unwrap<wxConfig*>(config);
	_self->Save(*cconf);
	return self;
}

#endif // wxUSE_CONFIG

DLL_LOCAL VALUE _shiftleft(VALUE self,VALUE val)
{
#if wxUSE_CONFIG
	if(rb_obj_is_kind_of(val,rb_cWXConfig))
		_self->Load(*unwrap<wxConfig*>(val));
	else
#endif // wxUSE_CONFIG
		_self->AddFileToHistory(unwrap<wxString>(val));
	return self;
}

DLL_LOCAL VALUE _shiftright(VALUE self,VALUE val)
{
#if wxUSE_CONFIG
	if(rb_obj_is_kind_of(val,rb_cWXConfig))
		_self->Save(*unwrap<wxConfig*>(val));
	else
#endif // wxUSE_CONFIG
		_self->AddFilesToMenu(unwrap<wxMenu*>(val));
	return self;
}


DLL_LOCAL VALUE _add(VALUE self,VALUE val)
{

	_self->AddFileToHistory(unwrap<wxString>(val));
	return self;
}

DLL_LOCAL VALUE _add_files(int argc,VALUE *argv,VALUE self)
{
	VALUE menu;
	rb_scan_args(argc, argv, "01",&menu);
	if(NIL_P(menu))
		_self->AddFilesToMenu();
	else
		_self->AddFilesToMenu(unwrap<wxMenu*>(menu));

	return self;
}


}
}

#endif


DLL_LOCAL void Init_WXFileHistory(VALUE rb_mWX)
{
#if wxUSE_FILE_HISTORY
	using namespace RubyWX::FileHistory;
	rb_cWXFileHistory = rb_define_class_under(rb_mWX,"FileHistory",rb_cObject);
	rb_define_alloc_func(rb_cWXFileHistory,_alloc);
	rb_include_module(rb_cWXFileHistory,rb_mEnumerable);

	rb_define_method(rb_cWXFileHistory,"each",RUBY_METHOD_FUNC(_each),0);
#if wxUSE_CONFIG
	rb_define_method(rb_cWXFileHistory,"load",RUBY_METHOD_FUNC(_load),-1);
	rb_define_method(rb_cWXFileHistory,"save",RUBY_METHOD_FUNC(_save),-1);
#endif // wxUSE_CONFIG

	rb_define_method(rb_cWXFileHistory,"add",RUBY_METHOD_FUNC(_add),1);
	rb_define_method(rb_cWXFileHistory,"add_files",RUBY_METHOD_FUNC(_add_files),-1);

	rb_define_method(rb_cWXFileHistory,"<<",RUBY_METHOD_FUNC(_shiftleft),1);
	rb_define_method(rb_cWXFileHistory,">>",RUBY_METHOD_FUNC(_shiftright),1);

	registerInfo<wxFileHistory>(rb_cWXFileHistory);
#endif
}
