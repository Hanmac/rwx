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
inline VALUE wrap< RubyApp >(RubyApp *app )
{
	return app->mRuby;
}

template <>
inline wxApp* wrap< wxApp* >(const VALUE &vapp)
{
	return unwrapPtr<RubyApp>(vapp, rb_cWXApp);
}


#endif /* WXAPP_HPP_ */
