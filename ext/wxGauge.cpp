/*
 * wxGauge.cpp
 *
 *  Created on: 27.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"

VALUE rb_cWXGauge;

#if wxUSE_GAUGE
#define _self wrap<wxGauge*>(self)

namespace RubyWX {
namespace Gauge {

macro_attr(Range,int)
macro_attr(Value,int)


VALUE _alloc(VALUE self)
{
	return wrap(new wxGauge(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,id;
	rb_scan_args(argc, argv, "11",&parent,&id);
	int i = NIL_P(id) ? wxID_ANY : NUM2INT(id);
	_self->Create(wrap<wxWindow*>(parent),i,0);
	rb_call_super(argc,argv);
	return self;
}

singlefunc(Pulse)

}
}
#endif
void Init_WXGauge(VALUE rb_mWX)
{
#if wxUSE_GAUGE
	using namespace RubyWX::Gauge;
	rb_cWXGauge = rb_define_class_under(rb_mWX,"Gauge",rb_cWXControl);
	rb_define_alloc_func(rb_cWXGauge,_alloc);

	rb_define_method(rb_cWXGauge,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXGauge,"range",_getRange,_setRange);
	rb_define_attr_method(rb_cWXGauge,"value",_getValue,_setValue);

	rb_define_method(rb_cWXGauge,"pulse",RUBY_METHOD_FUNC(_Pulse),0);
#endif

}

