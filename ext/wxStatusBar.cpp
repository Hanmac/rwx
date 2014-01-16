/*
 * wxStatusBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"
#include "wxRect.hpp"

VALUE rb_cWXStatusBar,rb_cWXStatusBarPane;

#if wxUSE_STATUSBAR
#define _self unwrap<wxStatusBar*>(self)



namespace RubyWX {
namespace StatusBar {

APP_PROTECT(wxStatusBar)


/*
 * call-seq:
 *   StatusBar.new(parent, name, [options])
 *   StatusBar.new(parent, [options])
 *
 * creates a new StatusBar widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		int style(wxSTB_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,style);
	}
	rb_call_super(argc,argv);
	return self;
}

macro_attr(StatusText,wxString)
macro_attr(FieldsCount,int)

DLL_LOCAL VALUE _GetFieldRect(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
	{
		wxRect rect;
		if(_self->GetFieldRect(NUM2INT(num),rect))
			return wrap(rect);
	}
	return Qnil;
}

DLL_LOCAL VALUE _getStatusText2(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		return wrap(_self->GetStatusText(NUM2INT(num)));
	return Qnil;
}

DLL_LOCAL VALUE _setStatusText2(VALUE self,VALUE str,VALUE num)
{
	rb_check_frozen(self);
	if(NUM2INT(num) < _self->GetFieldsCount())
		_self->SetStatusText(unwrap<wxString>(str),NUM2INT(num));
	//const_cast<wxStatusBarPane&>(_self->GetField(NUM2INT(num))).SetText(unwrap<wxString>(str));
	//_self->Update();
	return Qnil;
}

DLL_LOCAL VALUE _getStatusWidth(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		return INT2NUM(_self->GetStatusWidth(NUM2INT(num)));
	return Qnil;
}

DLL_LOCAL VALUE _setStatusWidth(VALUE self,VALUE num,VALUE val)
{
	rb_check_frozen(self);
	const size_t count = _self->GetFieldsCount();


	if(NUM2INT(num) < (int)count)
	{
		int w[count];
		for(size_t i = 0; i < count; ++i )
		{
			int v = _self->GetStatusWidth(i);
			w[i] = v ? v : -1;
		}
		w[NUM2INT(num)] = NUM2INT(val);
		_self->SetStatusWidths(count,w);
		//const_cast<wxStatusBarPane&>(_self->GetField(NUM2INT(num))).SetWidth(w);
		//_self->Refresh();
	}
	return Qnil;
}

DLL_LOCAL VALUE _pushStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE str,num;
	rb_scan_args(argc, argv, "11",&str,&num);
	rb_check_frozen(self);
	if(NIL_P(num))
		_self->PushStatusText(unwrap<wxString>(str));
	else
		_self->PushStatusText(unwrap<wxString>(str),NUM2INT(num));
	return Qnil;
}

DLL_LOCAL VALUE _popStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE num;
	rb_scan_args(argc, argv, "01",&num);
	rb_check_frozen(self);
	if(NIL_P(num))
		_self->PopStatusText();
	else
		_self->PopStatusText(NUM2INT(num));
	return Qnil;
}

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetFieldsCount());
}

//TODO Fix the bad Reference
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_size);
	std::size_t s = _self->GetFieldsCount();

	for(std::size_t i = 0 ; i < s; ++i)
		rb_yield(wrapPtr(&const_cast<wxStatusBarPane&>(_self->GetField(i)),rb_cWXStatusBarPane));
	return self;
}

namespace Pane
{
#undef _self
#define _self unwrapPtr<wxStatusBarPane>(self,rb_cWXStatusBarPane)

macro_attr(Width,int)
macro_attr(Style,int)
macro_attr(Text,wxString)

DLL_LOCAL VALUE _pushText(VALUE self,VALUE str)
{
	rb_check_frozen(self);
	return wrap(_self->PushText(unwrap<wxString>(str)));
}

singlereturn(PopText)

macro_attr_bool2(Ellipsized,SetIsEllipsized)

}

}
}
#endif
DLL_LOCAL void Init_WXStatusBar(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_define_attr(rb_cWXStatusBar,"status_text",1,1);
	rb_define_attr(rb_cWXStatusBar,"fields_count",1,1);

	rb_define_attr(rb_cWXStatusBarPane,"width",1,1);
	rb_define_attr(rb_cWXStatusBarPane,"style",1,1);
	rb_define_attr(rb_cWXStatusBarPane,"text",1,1);

#endif
#if wxUSE_STATUSBAR
	using namespace RubyWX::StatusBar;
	rb_cWXStatusBar = rb_define_class_under(rb_mWX,"StatusBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStatusBar,_alloc);

	rb_define_method(rb_cWXStatusBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_define_attr_method(rb_cWXStatusBar,"status_text",_getStatusText,_setStatusText);
	rb_define_attr_method(rb_cWXStatusBar,"fields_count",_getFieldsCount,_setFieldsCount);

	rb_define_method(rb_cWXStatusBar,"get_field_rect",RUBY_METHOD_FUNC(_GetFieldRect),1);


	rb_define_method(rb_cWXStatusBar,"get_status_text",RUBY_METHOD_FUNC(_getStatusText2),1);
	rb_define_method(rb_cWXStatusBar,"set_status_text",RUBY_METHOD_FUNC(_setStatusText2),2);

	rb_define_method(rb_cWXStatusBar,"get_status_width",RUBY_METHOD_FUNC(_getStatusWidth),1);
	rb_define_method(rb_cWXStatusBar,"set_status_width",RUBY_METHOD_FUNC(_setStatusWidth),2);

	rb_define_method(rb_cWXStatusBar,"push_status_text",RUBY_METHOD_FUNC(_pushStatusText),-1);
	rb_define_method(rb_cWXStatusBar,"pop_status_text",RUBY_METHOD_FUNC(_popStatusText),-1);

	rb_define_method(rb_cWXStatusBar,"each_statuspane",RUBY_METHOD_FUNC(_each),0);

	rb_cWXStatusBarPane = rb_define_class_under(rb_cWXStatusBar,"Pane",rb_cObject);
	rb_undef_alloc_func(rb_cWXStatusBarPane);

	using namespace Pane;
	rb_define_attr_method(rb_cWXStatusBarPane,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXStatusBarPane,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXStatusBarPane,"text",_getText,_setText);

	rb_define_method(rb_cWXStatusBarPane,"push_text",RUBY_METHOD_FUNC(_pushText),1);
	rb_define_method(rb_cWXStatusBarPane,"pop_text",RUBY_METHOD_FUNC(_PopText),0);


	registerInfo<wxStatusBar>(rb_cWXStatusBar);

#endif

}




