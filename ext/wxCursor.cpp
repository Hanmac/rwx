/*
 * wxCursor.cpp
 *
 *  Created on: 10.03.2012
 *      Author: hanmac
 */

#include "wxCursor.hpp"

VALUE rb_cWXCursor;

wxStockCursor toStockCursor(ID id)
{
	if(id == rb_intern("cross"))
		return wxCURSOR_CROSS;
	else if(id == rb_intern("hand"))
	    return wxCURSOR_HAND;
	else if(id == rb_intern("spraycan"))
	    return wxCURSOR_SPRAYCAN;
	else if(id == rb_intern("wait"))
	    return wxCURSOR_WAIT;
	else if(id == rb_intern("watch"))
	    return wxCURSOR_WATCH;
	else if(id == rb_intern("blank"))
	    return wxCURSOR_BLANK;
	return wxCURSOR_ARROW;
}


template <>
VALUE wrap< wxCursor >(wxCursor *bitmap )
{
	if(bitmap)
		return Data_Wrap_Struct(rb_cWXCursor, NULL, free, bitmap);
	return Qnil;
}

template <>
wxCursor* wrap< wxCursor* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullCursor;
	if(SYMBOL_P(vbitmap))
	{
		return new wxCursor(toStockCursor(SYM2ID(vbitmap)));
	}else if(rb_obj_is_kind_of(vbitmap,rb_cWXCursor))
		return unwrapPtr<wxCursor>(vbitmap, rb_cWXCursor);
	else
#if wxUSE_IMAGE
	return new wxCursor(wrap<wxImage>(vbitmap));
#else
	return &wxNullCursor;
#endif
}

template <>
wxCursor wrap< wxCursor >(const VALUE &vbitmap)
{
	return *wrap< wxCursor* >(vbitmap);
}


VALUE _busy(int argc,VALUE *argv,VALUE self)
{
	VALUE cursor;
	rb_scan_args(argc, argv, "01",&cursor);
	if(NIL_P(cursor))
		cursor = ID2SYM(rb_intern("wait"));

	wxBeginBusyCursor(wrap<wxCursor*>(cursor));
	rb_yield(Qnil);
	wxEndBusyCursor();
	return self;
}

VALUE _isBusy(VALUE self)
{
	return wrap(wxIsBusy());
}

void Init_WXCursor(VALUE rb_mWX)
{
	rb_cWXCursor = rb_define_class_under(rb_mWX,"Cursor",rb_cObject);
	rb_undef_alloc_func(rb_cWXCursor);

	rb_define_module_function(rb_mWX,"busy",RUBY_METHOD_FUNC(_busy),-1);
	rb_define_module_function(rb_mWX,"busy?",RUBY_METHOD_FUNC(_isBusy),0);
}
