/*
 * wxToolBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"
#include "wxBitmap.hpp"
#include "wxToolBarTool.hpp"
#include "wxEvtHandler.hpp"

VALUE rb_cWXToolBar;

#define _self wrap<wxToolBar*>(self)

namespace RubyWX {
namespace ToolBar {

macro_attr(ToolBitmapSize,wxSize)
macro_attr(Margins,wxSize)

macro_attr(ToolPacking,int)
macro_attr(ToolSeparation,int)

VALUE _alloc(VALUE self)
{
	return wrap(new wxToolBar(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,id;
	rb_scan_args(argc, argv, "11",&parent,&id);
	int i = NIL_P(id) ? wxID_ANY : NUM2INT(id);
	_self->Create(wrap<wxWindow*>(parent),i);
	rb_call_super(argc,argv);
	return self;
}

VALUE _addNormal(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp,arg;
	wxToolBarToolBase *tool = NULL;
	wxControl *c = NULL;
	rb_scan_args(argc, argv, "11*",&id,&text,&arg);
	if(rb_obj_is_kind_of(id,rb_cWXControl)){
		c = wrap<wxControl*>(id);
		if(c->GetParent() != _self)
			rb_raise(rb_eArgError, "%s has wrong parent.",wrap<char*>(id));
	}else if(rb_obj_is_kind_of(id,rb_cClass) && rb_class_inherited(id,rb_cWXControl)) {
		VALUE vc = rb_class_new_instance(1,&self,id);
		if(rb_block_given_p())
			rb_yield(vc);
		c = wrap<wxControl*>(vc);
	}else {
		rb_scan_args(argc, argv, "33",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
		if(NIL_P(bitmap))
			rb_raise(rb_eArgError,"need an valid bitmap");
		tool = _self->AddTool(NUM2INT(id), wrap<wxString>(text),
				wrap<wxBitmap>(bitmap), wrap<wxBitmap>(bmpDisabled),wxITEM_NORMAL,
				wrap<wxString>(shorthelp), wrap<wxString>(longhelp));
	}
	if(c)
		tool = _self->AddControl(c,wrap<wxString>(text));

	if(rb_block_given_p() && !c){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
		_self->Bind(wxEVT_COMMAND_MENU_SELECTED,RubyFunctor(proc),tool->GetId());
#else
		_self->Connect(tool->GetId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
#endif
	}
	return wrap(tool);
}


VALUE _addCheck(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "33",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	if(NIL_P(bitmap))
		rb_raise(rb_eArgError,"need an valid bitmap");

	wxToolBarToolBase *tool = _self->AddCheckTool(NUM2INT(id), wrap<wxString>(text),
				wrap<wxBitmap>(bitmap), wrap<wxBitmap>(bmpDisabled),
				wrap<wxString>(shorthelp), wrap<wxString>(longhelp));

	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
		_self->Bind(wxEVT_COMMAND_MENU_SELECTED,RubyFunctor(proc),tool->GetId());
#else
		_self->Connect(tool->GetId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
#endif
	}

	return wrap(tool);
}


VALUE _addRadio(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "33",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);
	if(NIL_P(bitmap))
		rb_raise(rb_eArgError,"need an valid bitmap");
	wxToolBarToolBase *tool = _self->AddRadioTool(NUM2INT(id), wrap<wxString>(text),
			wrap<wxBitmap>(bitmap), wrap<wxBitmap>(bmpDisabled),
			wrap<wxString>(shorthelp), wrap<wxString>(longhelp));

	if(rb_block_given_p()){
		VALUE proc = rb_block_proc();
#ifdef wxHAS_EVENT_BIND
_self->Bind(wxEVT_COMMAND_MENU_SELECTED,RubyFunctor(proc),tool->GetId());
#else
_self->Connect(tool->GetId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RubyFunctor::operator(),NULL,new RubyFunctor(proc));
#endif
	}

	return wrap(tool);
}

VALUE _each(VALUE self)
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

void Init_WXToolBar(VALUE rb_mWX)
{
	using namespace RubyWX::ToolBar;
	rb_cWXToolBar = rb_define_class_under(rb_mWX,"ToolBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXToolBar,_alloc);

	rb_define_method(rb_cWXToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXToolBar,"addNormal",RUBY_METHOD_FUNC(_addNormal),-1);
	rb_define_method(rb_cWXToolBar,"addCheck",RUBY_METHOD_FUNC(_addCheck),-1);
	rb_define_method(rb_cWXToolBar,"addRadio",RUBY_METHOD_FUNC(_addRadio),-1);

	rb_define_method(rb_cWXToolBar,"addSeparator",RUBY_METHOD_FUNC(_AddSeparator),0);
	rb_define_method(rb_cWXToolBar,"addStretchableSpace",RUBY_METHOD_FUNC(_AddStretchableSpace),0);

	rb_define_method(rb_cWXToolBar,"realize",RUBY_METHOD_FUNC(_Realize),0);


	rb_define_method(rb_cWXToolBar,"each_tool",RUBY_METHOD_FUNC(_each),0);

}


