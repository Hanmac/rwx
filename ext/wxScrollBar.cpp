/*
 * wxScrollBar.cpp
 *
 *  Created on: 15.04.2016
 *      Author: hanmac
 */


#include "wxScrollBar.hpp"


VALUE rb_cWXScrollBar;

#if wxUSE_SCROLLBAR
#define _self unwrap<wxScrollBar*>(self)

namespace RubyWX {
namespace ScrollBar {

macro_attr(ThumbPosition,int)

#ifdef HAVE_WXSCROLLBAR_SETTHUMBSIZE
macro_attr(ThumbSize,int)
#else
singlereturn(GetThumbSize)
#define _getThumbSize _GetThumbSize

VALUE _setThumbSize(VALUE self, VALUE val)
{
	rb_check_frozen(self);
	wxScrollBar* sb = _self;
	sb->SetScrollbar(
		sb->GetThumbPosition(),
		NUM2INT(val),
		sb->GetRange(),
		sb->GetPageSize()
	);
	return val;
}

#endif

macro_attr(PageSize,int)
macro_attr(Range,int)

singlereturn(IsVertical)

APP_PROTECT(wxScrollBar)


/*
 * call-seq:
 *   ScrollBar.new(parent, name, [options])
 *   ScrollBar.new(parent, [options])
 *
 * creates a new ScrollBar widget.
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
		int style(wxSB_HORIZONTAL);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);

			set_hash_flag_option(hash,"vertical",wxSB_VERTICAL,style);
		}
		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);
		}
	}

	rb_call_super(argc,argv);
	return self;
}

}
}
#endif


/* Document-attr: thumb_position
 * the value of the ScrollBar. Integer
 */
/* Document-attr: thumb_size
 * the thumb size of the ScrollBar. Integer
 */
/* Document-attr: thumb_length
 * the thumb length of the ScrollBar. Integer
 */
/* Document-attr: range
 * the range value of the ScrollBar. Integer
 */

/* Document-const: VERTICAL
 *   Displays the ScrollBar vertically.
 */

/* Document-attr: vertical?
 * call-seq:
 *   vertical? -> true/false
 *
 * returns true if this control is vertical.
 * ===Return value
 * true/false
*/

DLL_LOCAL void Init_WXScrollBar(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_SCROLLBAR
	using namespace RubyWX::ScrollBar;
	rb_cWXScrollBar = rb_define_class_under(rb_mWX,"ScrollBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXScrollBar,_alloc);

#if 0
	rb_define_attr(rb_cWXScrollBar,"thumb_position",1,1);
	rb_define_attr(rb_cWXScrollBar,"thumb_size",1,1);
	rb_define_attr(rb_cWXScrollBar,"page_size",1,1);

	rb_define_attr(rb_cWXScrollBar,"range",1,1);
#endif

	rb_define_method(rb_cWXScrollBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXScrollBar,"thumb_position",_getThumbPosition,_setThumbPosition);
	rb_define_attr_method(rb_cWXScrollBar,"thumb_size",_getThumbSize,_setThumbSize);
	rb_define_attr_method(rb_cWXScrollBar,"page_size",_getPageSize,_setPageSize);
	rb_define_attr_method(rb_cWXScrollBar,"range",_getRange,_setRange);

	rb_define_method(rb_cWXScrollBar,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);

	rb_define_const(rb_cWXScrollBar,"VERTICAL",INT2NUM(wxSB_VERTICAL));

	registerInfo<wxScrollBar>(rb_cWXScrollBar);
#endif

}



