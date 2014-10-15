/*
 * wxApp.hpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#ifndef WXAPP_HPP_
#define WXAPP_HPP_

#include "main.hpp"

extern bool ruby_app_inited;
extern VALUE rb_cWXApp;
void Init_WXApp(VALUE rb_mWX);

class RubyApp : public wxApp {
public:
	RubyApp(VALUE klass);

    virtual bool OnInit();
    virtual int OnRun();
    virtual int OnExit();

    VALUE mRuby;
protected:
#if wxUSE_INTL
    wxLocale *mLocale;  // locale we'll be using
#endif
};

template <>
wxApp* unwrap< wxApp* >(const VALUE &vapp);

void app_protected();

#define APP_PROTECT(type) DLL_LOCAL VALUE _alloc(VALUE self)\
{\
	app_protected();\
	return wrapTypedPtr(new type,self);\
}
#define APP_PROTECT_NULL DLL_LOCAL VALUE _alloc(VALUE self)\
{\
	app_protected();\
	return wrapTypedPtr((void*)NULL,self, true);\
}

#endif /* WXAPP_HPP_ */
