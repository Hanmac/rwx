/*
 * wxToolBarBase.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */

#include "wxToolBarBase.hpp"
#include "wxBitmap.hpp"
#include "wxToolBarTool.hpp"
#include "wxControl.hpp"

VALUE rb_cWXToolBarBase;

#if wxUSE_TOOLBAR

#define _self unwrapPtr<wxToolBarBase>(self,rb_cWXToolBarBase)

namespace RubyWX {
namespace ToolBarBase {

macro_attr(ToolBitmapSize,wxSize)
macro_attr(Margins,wxSize)

macro_attr(ToolPacking,int)
macro_attr(ToolSeparation,int)

void bind_callback(wxToolBarBase* toolbar,wxWindowID id)
{
	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
		toolbar->Bind(wxEVT_MENU,RubyFunctor(proc),id);
#else
		toolbar->Connect(id,wxEVT_MENU,wxCommandEventHandler(RubyFunctor::operator()),NULL,new RubyFunctor(proc));
#endif
	}
}


DLL_LOCAL VALUE _addNormal(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "24",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	wxWindowID wxid = unwrapID(id);
	wxToolBarToolBase *tool = _self->AddTool(wxid, unwrap<wxString>(text),
				wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
				wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),wxITEM_NORMAL,
				unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));
	bind_callback(_self,tool->GetId());

	return wrap(tool);
}

DLL_LOCAL VALUE _addControl(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,arg;
	wxControl *c = NULL;
	rb_scan_args(argc, argv, "11*",&id,&text,&arg);
	if(rb_obj_is_kind_of(id,rb_cClass) && rb_class_inherited(id,rb_cWXControl)) {
		rb_scan_args(argc, argv, "11",&id,&arg);
		VALUE argv2[] = {self, arg };
		c = unwrap<wxControl*>(rb_class_new_instance(2,argv2,id));
	}else
	{
		c = unwrap<wxControl*>(id);
		if(c->GetParent() != _self)
			rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(id));
	}

	return wrap( _self->AddControl(c,unwrap<wxString>(text)));
}


DLL_LOCAL VALUE _addCheck(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "24",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	wxWindowID wxid = unwrapID(id);

	wxToolBarToolBase *tool = _self->AddCheckTool(wxid, unwrap<wxString>(text),
				wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
				wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),
				unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));

	bind_callback(_self,tool->GetId());

	return wrap(tool);
}


DLL_LOCAL VALUE _addRadio(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "24",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	wxWindowID wxid = unwrapID(id);
	wxToolBarToolBase *tool = _self->AddRadioTool(wxid, unwrap<wxString>(text),
			wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
			wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),
			unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));

	bind_callback(_self,tool->GetId());

	return wrap(tool);
}

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	size_t count = _self->GetToolsCount();
	for(size_t i = 0; i < count; ++i)
	{
		rb_yield(wrap(const_cast<wxToolBarToolBase*>(_self->GetToolByPos(i))));
	}
	return self;
}


singlereturn(AddSeparator)
singlereturn(AddStretchableSpace)

singlereturn(Realize)

}
}

#endif

DLL_LOCAL void Init_WXToolBarBase(VALUE rb_mWX)
{
#if wxUSE_TOOLBAR
	using namespace RubyWX::ToolBarBase;
	rb_cWXToolBarBase = rb_define_class_under(rb_mWX,"ToolBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXToolBarBase);

//	rb_define_method(rb_cWXToolBarBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXToolBarBase,"add_normal",RUBY_METHOD_FUNC(_addNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"add_control",RUBY_METHOD_FUNC(_addNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"add_check",RUBY_METHOD_FUNC(_addCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"add_radio",RUBY_METHOD_FUNC(_addRadio),-1);

	rb_define_method(rb_cWXToolBarBase,"addSeparator",RUBY_METHOD_FUNC(_AddSeparator),0);
	rb_define_method(rb_cWXToolBarBase,"addStretchableSpace",RUBY_METHOD_FUNC(_AddStretchableSpace),0);

	rb_define_method(rb_cWXToolBarBase,"realize",RUBY_METHOD_FUNC(_Realize),0);


	rb_define_method(rb_cWXToolBarBase,"each_tool",RUBY_METHOD_FUNC(_each),0);
#endif
}
