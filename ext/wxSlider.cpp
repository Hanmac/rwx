/*
 * wxSlider.cpp
 *
 *  Created on: 22.03.2012
 *      Author: hanmac
 */


#include "wxSlider.hpp"


VALUE rb_cWXSlider;

#if wxUSE_SLIDER
#define _self unwrap<wxSlider*>(self)

namespace RubyWX {
namespace Slider {

macro_attr(Value,int)
macro_attr(Max,int)
macro_attr(Min,int)

macro_attr(LineSize,int)
macro_attr(PageSize,int)

macro_attr(ThumbLength,int)

APP_PROTECT(wxSlider)


/*
 * call-seq:
 *   Slider.new(parent, name, [options])
 *   Slider.new(parent, [options])
 *
 * creates a new Slider widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * min Integer
 *   * max Integer
 *   * value Integer
 *   * line_size Integer
 *   * page_size Integer
 *   * thumb_length Integer
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int value(0), min(0), max(0);
		int style(wxSL_HORIZONTAL);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"min",min);
			set_hash_option(hash,"max",max);
			set_hash_option(hash,"value",value);
			set_hash_option(hash,"style",style);


			set_hash_flag_option(hash,"vertical",wxSL_VERTICAL,style);
			set_hash_flag_option(hash,"inverse",wxSL_INVERSE,style);

			set_hash_flag_option(hash,"labels",wxSL_LABELS,style);

			set_hash_flag_option(hash,"min_max_labels",wxSL_MIN_MAX_LABELS,style);
			set_hash_flag_option(hash,"value_label",wxSL_VALUE_LABEL,style);

		}
		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,value,min,max,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		if(rb_obj_is_kind_of(name,rb_cString))
		{
			set_obj_option(hash, "min", &wxSlider::SetMin,_self);
			set_obj_option(hash, "max", &wxSlider::SetMax,_self);
			set_obj_option(hash, "value", &wxSlider::SetValue,_self);
		}
		set_obj_option(hash, "line_size", &wxSlider::SetLineSize,_self);
		set_obj_option(hash, "page_size", &wxSlider::SetPageSize,_self);
		set_obj_option(hash, "thumb_length", &wxSlider::SetThumbLength,_self);

	}

	rb_call_super(argc,argv);
	return self;
}

/*
 * call-seq:
 *   vertical? -> true/false
 *
 * returns true if this control is vertical.
 * ===Return value
 * true/false
*/
VALUE _IsVertical(VALUE self)
{
	return wrap(_self->HasFlag(wxSL_VERTICAL));
}

}
}
#endif


/* Document-attr: value
 * the value of the Slider. Integer
 */
/* Document-attr: min
 * the minimum value of the Slider. Integer
 */
/* Document-attr: max
 * the maximum value of the Slider. Integer
 */

/* Document-attr: line_size
 * the line size of the Slider. Integer
 */
/* Document-attr: page_size
 * the page size of the Slider. Integer
 */
/* Document-attr: thumb_length
 * the thumb length of the Slider. Integer
 */


/* Document-const: VERTICAL
 *   Displays the slider vertically.
 */
/* Document-const: INVERSE
 *   Inverses the minimum and maximum endpoints on the slider.
 */
/* Document-const: MIN_MAX_LABELS
 *   Displays minimum, maximum labels
 */
/* Document-const: VALUE_LABEL
 *   Displays value label
 */
/* Document-const: LABELS
 *   Displays minimum, maximum and value labels
 */

DLL_LOCAL void Init_WXSlider(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_SLIDER
	using namespace RubyWX::Slider;
	rb_cWXSlider = rb_define_class_under(rb_mWX,"Slider",rb_cWXControl);
	rb_define_alloc_func(rb_cWXSlider,_alloc);

#if 0
	rb_define_attr(rb_cWXSlider,"value",1,1);
	rb_define_attr(rb_cWXSlider,"min",1,1);
	rb_define_attr(rb_cWXSlider,"max",1,1);

	rb_define_attr(rb_cWXSlider,"line_size",1,1);
	rb_define_attr(rb_cWXSlider,"page_size",1,1);
	rb_define_attr(rb_cWXSlider,"thumb_length",1,1);
#endif

	rb_define_method(rb_cWXSlider,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXSlider,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXSlider,"min",_getMin,_setMin);
	rb_define_attr_method(rb_cWXSlider,"max",_getMax,_setMax);

	rb_define_attr_method(rb_cWXSlider,"line_size",_getLineSize,_setLineSize);
	rb_define_attr_method(rb_cWXSlider,"page_size",_getPageSize,_setPageSize);
	rb_define_attr_method(rb_cWXSlider,"thumb_length",_getThumbLength,_setThumbLength);

	rb_define_method(rb_cWXSlider,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);

	rb_define_const(rb_cWXSlider,"VERTICAL",RB_INT2NUM(wxSL_VERTICAL));
	rb_define_const(rb_cWXSlider,"INVERSE",RB_INT2NUM(wxSL_INVERSE));
	rb_define_const(rb_cWXSlider,"MIN_MAX_LABELS",RB_INT2NUM(wxSL_MIN_MAX_LABELS));
	rb_define_const(rb_cWXSlider,"VALUE_LABEL",RB_INT2NUM(wxSL_VALUE_LABEL));
	rb_define_const(rb_cWXSlider,"LABELS",RB_INT2NUM(wxSL_LABELS));

	registerEventType("slider", wxEVT_SLIDER,rb_cWXCommandEvent);

	registerInfo<wxSlider>(rb_cWXSlider);
#endif

}



