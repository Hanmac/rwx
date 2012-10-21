/*
 * wxStartUpTip.cpp
 *
 *  Created on: 08.03.2012
 *      Author: hanmac
 */

#include "wxStartUpTip.hpp"
#if wxUSE_STARTUP_TIPS
class DLL_LOCAL RubyTipProvider : public wxTipProvider
{
public:
	RubyTipProvider(VALUE obj,VALUE i) : wxTipProvider(NUM2UINT(i)), mRuby(obj) {}

	wxString GetTip() {
		if(rb_respond_to(mRuby,rb_intern("to_a"))){

			if(m_currentTip >= NUM2UINT(rb_funcall(mRuby,rb_intern("size"),0)))
				m_currentTip = 0;
			return unwrap<wxString>(rb_funcall(mRuby,rb_intern("[]"),1,UINT2NUM(m_currentTip++)));
		}else
			return unwrap<wxString>(mRuby);
	}

private:
	VALUE mRuby;
};

VALUE _showTip(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,obj,i,show;
	rb_scan_args(argc, argv, "31",&parent,&obj,&i,&show);
	RubyTipProvider tip(obj,i);
	if(NIL_P(show))
		show = Qtrue;
	return wrap(wxShowTip(unwrap<wxWindow*>(parent),&tip,RTEST(show)));
}
#endif
DLL_LOCAL void Init_ShowTip(VALUE rb_mWX)
{
#if wxUSE_STARTUP_TIPS
	rb_define_module_function(rb_mWX,"show_tip",RUBY_METHOD_FUNC(_showTip),-1);
#endif
}
