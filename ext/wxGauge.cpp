/*
 * wxGauge.cpp
 *
 *  Created on: 27.02.2012
 *      Author: hanmac
 */

#include "wxGauge.hpp"

VALUE rb_cWXGauge;

#if wxUSE_GAUGE
#define _self unwrap<wxGauge*>(self)

namespace RubyWX {
namespace Gauge {

macro_attr(Range,int)
macro_attr(Value,int)

APP_PROTECT(wxGauge)

/*
 * call-seq:
 *   Gauge.new(parent, name, [options])
 *   Gauge.new(parent, [options])
 *
 * creates a new Gauge widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set:
 *   * range Integer
 *   * value Integer
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxGA_HORIZONTAL);

		int range(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"range",style);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,range,wxDefaultPosition,wxDefaultSize,style);

	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;

		set_option(value,Value,int)
	}
	return self;
}

singlefunc(Pulse)

}
}
#endif


/* Document-attr: range
 * the range of the Gauge. Integer
 */
/* Document-attr: value
 * the value of the Gauge. Integer
 */

DLL_LOCAL void Init_WXGauge(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXGauge,"range",1,1);
	rb_define_attr(rb_cWXGauge,"value",1,1);
#endif

#if wxUSE_GAUGE
	using namespace RubyWX::Gauge;
	rb_cWXGauge = rb_define_class_under(rb_mWX,"Gauge",rb_cWXControl);
	rb_define_alloc_func(rb_cWXGauge,_alloc);

	rb_define_method(rb_cWXGauge,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXGauge,"range",_getRange,_setRange);
	rb_define_attr_method(rb_cWXGauge,"value",_getValue,_setValue);

	rb_define_method(rb_cWXGauge,"pulse",RUBY_METHOD_FUNC(_Pulse),0);

	registerInfo<wxGauge>(rb_cWXGauge);
#endif

}

