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

DLL_LOCAL wxToolBarToolBase* _add_base(int argc,VALUE *argv,VALUE self,wxItemKind kind)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "24",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	wxWindowID wxid = unwrapID(id);
	wxToolBarToolBase *tool = _self->AddTool(wxid, unwrap<wxString>(text),
				wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
				wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),kind,
				unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));
	bind_callback(_self,tool->GetId());
	return tool;
}



DLL_LOCAL VALUE _addNormal(int argc,VALUE *argv,VALUE self)
{
	return wrap(_add_base(argc,argv,self,wxITEM_NORMAL));
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
	return wrap(_add_base(argc,argv,self,wxITEM_CHECK));
}


DLL_LOCAL VALUE _addRadio(int argc,VALUE *argv,VALUE self)
{
	return wrap(_add_base(argc,argv,self,wxITEM_RADIO));
}

DLL_LOCAL wxToolBarToolBase* _insert_base(int argc,VALUE *argv,VALUE self, wxItemKind kind )
{
	VALUE idx,id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "34",&idx,&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);

	wxWindowID wxid = unwrapID(id);
	wxToolBarToolBase *tool = _self->InsertTool(NUM2UINT(idx), wxid, unwrap<wxString>(text),
			wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
			wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),kind,
			unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));
	bind_callback(_self,tool->GetId());
	return tool;
}

DLL_LOCAL VALUE _insertNormal(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_NORMAL));
}

DLL_LOCAL VALUE _insertCheck(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_CHECK));
}

DLL_LOCAL VALUE _insertRadio(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_RADIO));
}


DLL_LOCAL VALUE _insertControl(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,id,text,arg;
	wxControl *c = NULL;
	rb_scan_args(argc, argv, "21*",&idx,&id,&text,&arg);
	if(rb_obj_is_kind_of(id,rb_cClass) && rb_class_inherited(id,rb_cWXControl)) {
		rb_scan_args(argc, argv, "21",&idx,&id,&arg);
		VALUE argv2[] = {self, arg };
		c = unwrap<wxControl*>(rb_class_new_instance(2,argv2,id));
	}else
	{
		c = unwrap<wxControl*>(id);
		if(c->GetParent() != _self)
			rb_raise(rb_eArgError, "%s has wrong parent.",unwrap<char*>(id));
	}

	return wrap( _self->InsertControl(NUM2UINT(idx),c,unwrap<wxString>(text)));

}


DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetToolsCount());
}

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_size);
	std::size_t count = _self->GetToolsCount();
	for(std::size_t i = 0; i < count; ++i)
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
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_TOOLBAR
	using namespace RubyWX::ToolBarBase;
	rb_cWXToolBarBase = rb_define_class_under(rb_mWX,"ToolBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXToolBarBase);

//	rb_define_method(rb_cWXToolBarBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXToolBarBase,"add_normal",RUBY_METHOD_FUNC(_addNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"add_control",RUBY_METHOD_FUNC(_addControl),-1);
	rb_define_method(rb_cWXToolBarBase,"add_check",RUBY_METHOD_FUNC(_addCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"add_radio",RUBY_METHOD_FUNC(_addRadio),-1);

	rb_define_method(rb_cWXToolBarBase,"add_separator",RUBY_METHOD_FUNC(_AddSeparator),0);
	rb_define_method(rb_cWXToolBarBase,"add_stretchable_space",RUBY_METHOD_FUNC(_AddStretchableSpace),0);

	rb_define_method(rb_cWXToolBarBase,"insert_normal",RUBY_METHOD_FUNC(_insertNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_control",RUBY_METHOD_FUNC(_insertControl),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_check",RUBY_METHOD_FUNC(_insertCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_radio",RUBY_METHOD_FUNC(_insertRadio),-1);


	rb_define_method(rb_cWXToolBarBase,"realize",RUBY_METHOD_FUNC(_Realize),0);


	rb_define_method(rb_cWXToolBarBase,"each_tool",RUBY_METHOD_FUNC(_each),0);
#endif
}
