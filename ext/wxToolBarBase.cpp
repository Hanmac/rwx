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

		toolbar->Bind(wxEVT_MENU,RubyFunctor(proc),id);
	}
}

wxControl* create_control(VALUE self,VALUE ctrl,VALUE hash)
{
	wxControl *c = NULL;
	if(rb_obj_is_kind_of(ctrl,rb_cClass) && rb_class_inherited(ctrl,rb_cWXControl)) {
		VALUE argv2[] = {self, hash };
		c = unwrap<wxControl*>(rb_class_new_instance(2,argv2,ctrl));
	}else if(nil_check(ctrl)) {
		window_parent_check(ctrl,_self,c);
	}
	return c;
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


/*
 * call-seq:
 *   add_normal(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   add_normal(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * adds a new normal tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _addNormal(int argc,VALUE *argv,VALUE self)
{
	return wrap(_add_base(argc,argv,self,wxITEM_NORMAL));
}

/*
 * call-seq:
 *   add_check(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   add_check(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * adds a new check tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _addCheck(int argc,VALUE *argv,VALUE self)
{
	return wrap(_add_base(argc,argv,self,wxITEM_CHECK));
}

/*
 * call-seq:
 *   add_radio(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   add_radio(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * adds a new radio tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _addRadio(int argc,VALUE *argv,VALUE self)
{
	return wrap(_add_base(argc,argv,self,wxITEM_RADIO));
}


/*
 * call-seq:
 *   add_control(control, [text]) -> WX::ToolBarBase::Tool
 *   add_control(klass, [text], ...) [{|control| ... }] -> WX::ToolBarBase::Tool
 *
 * adds a new control tool item to the ToolBar widget.
 * in the second form create the control from the given class.
 * ===Arguments
 * * control WX::Control widget that will be added to the tool bar. the Tool bar need to be the parent of the widget.
 * * text is the Label of the tool item. String
 * * klass, Class from which the control will be created, it need to inherit from WX::Control
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [TypeError]
 * * control is nil or can't be converted into an WX::Control instance
 * [ArgumentError]
 * * control does not have this ToolBar as parent
 *
*/
DLL_LOCAL VALUE _addControl(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,arg;

	rb_scan_args(argc, argv, "11:",&id,&text,&arg);

	wxControl *c = create_control(self,id,arg);

	return wrap( _self->AddControl(c,unwrap<wxString>(text)));
}


DLL_LOCAL wxToolBarToolBase* _insert_base(int argc,VALUE *argv,VALUE self, wxItemKind kind )
{
	VALUE idx,id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "34",&idx,&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);


	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetToolsCount()+1))
	{
		wxWindowID wxid = unwrapID(id);

		wxToolBarToolBase *tool = _self->InsertTool(cidx, wxid, unwrap<wxString>(text),
				wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
				wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),kind,
				unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));
		bind_callback(_self,tool->GetId());
		return tool;
	}
	return NULL;
}



/*
 * call-seq:
 *   insert_normal(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   insert_normal(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * insert a new normal tool item to the ToolBar widget at the given position.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
 * [ArgumentError]
 * * does raise when either bitmap or disabled_bitmap can't converted into an WX::Bitmap
 *
*/
DLL_LOCAL VALUE _insertNormal(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_NORMAL));
}

/*
 * call-seq:
 *   insert_check(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   insert_check(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * insert a new check tool item to the ToolBar widget at the given position.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
 * [ArgumentError]
 * * does raise when either bitmap or disabled_bitmap can't converted into an WX::Bitmap
 *
*/
DLL_LOCAL VALUE _insertCheck(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_CHECK));
}


/*
 * call-seq:
 *   insert_radio(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   insert_radio(pos, id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * insert a new radio tool item to the ToolBar widget at the given position.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
 * [ArgumentError]
 * * does raise when either bitmap or disabled_bitmap can't converted into an WX::Bitmap
 *
*/
DLL_LOCAL VALUE _insertRadio(int argc,VALUE *argv,VALUE self)
{
	return wrap(_insert_base(argc,argv,self,wxITEM_RADIO));
}


/*
 * call-seq:
 *   insert_control(pos, control, [text]) -> WX::ToolBarBase::Tool
 *   insert_control(pos, klass, [text], ...) [{|control| ... }] -> WX::ToolBarBase::Tool
 *
 * inserts a new control tool item to the ToolBar widget into the given position.
 * in the second form create the control from the given class.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * * control WX::Control widget that will be added to the tool bar. the Tool bar need to be the parent of the widget.
 * * text is the Label of the tool item. String
 * * klass, Class from which the control will be created, it need to inherit from WX::Control
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
 * [TypeError]
 * * control is nil or can't be converted into an WX::Control instance
 * [ArgumentError]
 * * control does not have this ToolBar as parent
*/
DLL_LOCAL VALUE _insertControl(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,id,text,arg;
	rb_scan_args(argc, argv, "21:",&idx,&id,&text,&arg);
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetToolsCount()+1))
	{

		wxControl *c = create_control(self,id,arg);

		return wrap( _self->InsertControl(NUM2UINT(idx),c,unwrap<wxString>(text)));
	}
	return Qnil;
}

/*
 * call-seq:
 *   insert_separator(pos) -> WX::ToolBarBase::Tool
 *
 * insert a new separator tool item to the ToolBar widget at the given position.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
*/
DLL_LOCAL VALUE _insert_separator(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetToolsCount()+1))
	{
		return wrap(_self->InsertSeparator(cidx));
	}
	return Qnil;
}

/*
 * call-seq:
 *   insert_stretchable_space(pos) -> WX::ToolBarBase::Tool
 *
 * insert a new stretchable space tool item to the ToolBar widget at the given position.
 * ===Arguments
 * * pos position of the new tool item. Integer
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of ToolBarItems
*/
DLL_LOCAL VALUE _insert_stretchable_space(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetToolsCount()+1))
	{
		return wrap(_self->InsertStretchableSpace(cidx));
	}
	return Qnil;
}



DLL_LOCAL wxToolBarToolBase* _prepend_base(int argc,VALUE *argv,VALUE self, wxItemKind kind )
{
	VALUE id,text,bitmap,bmpDisabled,shorthelp,longhelp;
	rb_scan_args(argc, argv, "24",&id,&text,&bitmap,&bmpDisabled,&shorthelp,&longhelp);

	wxWindowID wxid = unwrapID(id);
	wxToolBarToolBase *tool = _self->InsertTool(0, wxid, unwrap<wxString>(text),
			wrapBitmap(bitmap,wxid,WRAP_BITMAP_RAISE,wxART_TOOLBAR),
			wrapBitmap(bmpDisabled,wxid,WRAP_BITMAP_NULL,wxART_TOOLBAR),kind,
			unwrap<wxString>(shorthelp), unwrap<wxString>(longhelp));
	bind_callback(_self,tool->GetId());
	return tool;
}


/*
 * call-seq:
 *   prepend_normal(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   prepend_normal(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * prepends a new normal tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _prependNormal(int argc,VALUE *argv,VALUE self)
{
	return wrap(_prepend_base(argc,argv,self,wxITEM_NORMAL));
}

/*
 * call-seq:
 *   prepend_check(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   prepend_check(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * prepends a new check tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _prependCheck(int argc,VALUE *argv,VALUE self)
{
	return wrap(_prepend_base(argc,argv,self,wxITEM_CHECK));
}

/*
 * call-seq:
 *   prepend_radio(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) -> WX::ToolBarBase::Tool
 *   prepend_radio(id, text, [bitmap], [disabled_bitmap], [short_help], [long_help]) {|event| ... } -> WX::ToolBarBase::Tool
 *
 * adds a new radio tool item to the ToolBar widget.
 * when block is given, bind the block to the event of the tool item.
 * ===Arguments
 * * id of the tool item: Symbol/Integer/nil
 * * text is the Label of the tool item. String
 * * bitmap is the icon shown for the tool item. WX::Bitmap, when nil try to get bitmap from id
 * * disabled_bitmap is the icon shown when the tool item is disabled. WX::Bitmap, when nil make one from gray-scaled bitmap.
 * * short_help shown in WX::StatusBar. String
 * * long_help shown in ToolTip. String
 * ===Return value
 * WX::ToolBarBase::Tool
 *
*/
DLL_LOCAL VALUE _prependRadio(int argc,VALUE *argv,VALUE self)
{
	return wrap(_prepend_base(argc,argv,self,wxITEM_RADIO));
}

/*
 * call-seq:
 *   prepends_control(control, [text]) -> WX::ToolBarBase::Tool
 *   prepends_control(klass, [text], ...) [{|control| ... }] -> WX::ToolBarBase::Tool
 *
 * prepends a new control tool item to the ToolBar widget.
 * in the second form create the control from the given class.
 * ===Arguments
 * * control WX::Control widget that will be added to the tool bar. the Tool bar need to be the parent of the widget.
 * * text is the Label of the tool item. String
 * * klass, Class from which the control will be created, it need to inherit from WX::Control
 * ===Return value
 * WX::ToolBarBase::Tool
 * === Exceptions
 * [TypeError]
 * * control is nil or can't be converted into an WX::Control instance
 * [ArgumentError]
 * * control does not have this ToolBar as parent
 *
*/
DLL_LOCAL VALUE _prependControl(int argc,VALUE *argv,VALUE self)
{
	VALUE id,text,arg;
	rb_scan_args(argc, argv, "11:",&id,&text,&arg);

	wxControl *c = create_control(self,id,arg);

	return wrap( _self->InsertControl(0,c,unwrap<wxString>(text)));
}

/*
 * call-seq:
 *   prepend_separator -> WX::ToolBarBase::Tool
 *
 * prepends a new separator tool item to the ToolBar widget.
 * ===Return value
 * WX::ToolBarBase::Tool
*/
DLL_LOCAL VALUE _prepend_separator(VALUE self)
{
	return wrap(_self->InsertSeparator(0));
}

/*
 * call-seq:
 *   prepend_stretchable_space -> WX::ToolBarBase::Tool
 *
 * prepends a new stretchable space tool item to the ToolBar widget.
 * ===Return value
 * WX::ToolBarBase::Tool
*/
DLL_LOCAL VALUE _prepend_stretchable_space(VALUE self)
{
	return wrap(_self->InsertStretchableSpace(0));
}


DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetToolsCount());
}

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_size));
	std::size_t count = _self->GetToolsCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		rb_yield(wrap(const_cast<wxToolBarToolBase*>(_self->GetToolByPos(i))));
	}
	return self;
}

DLL_LOCAL VALUE _set_rows(VALUE self,VALUE val)
{
	_self->SetRows(NUM2INT(val));
	return val;
}


singlereturn(AddSeparator)
singlereturn(AddStretchableSpace)

singlereturn(Realize)

singlereturn(IsVertical)

singlereturn(GetMaxRows)

/* Document-method: add_separator
 * call-seq:
 *   add_separator -> WX::ToolBarBase::Tool
 *
 * adds a new separator tool item to the ToolBar widget.
 * ===Return value
 * WX::ToolBarBase::Tool
*/

/* Document-method: add_stretchable_space
 * call-seq:
 *   add_stretchable_space -> WX::ToolBarBase::Tool
 *
 * adds a new stretchable space tool item to the ToolBar widget.
 * ===Return value
 * WX::ToolBarBase::Tool
*/

/* Document-method: vertical?
 * call-seq:
 *   vertical? -> true/false
 *
 * returns true if this control is vertical.
 * ===Return value
 * true/false
*/

}
}

#endif

DLL_LOCAL void Init_WXToolBarBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_TOOLBAR
	using namespace RubyWX::ToolBarBase;
	rb_cWXToolBarBase = rb_define_class_under(rb_mWX,"ToolBarBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXToolBarBase);

//	rb_define_method(rb_cWXToolBarBase,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXToolBarBase,"add_normal",RUBY_METHOD_FUNC(_addNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"add_check",RUBY_METHOD_FUNC(_addCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"add_radio",RUBY_METHOD_FUNC(_addRadio),-1);
	rb_define_method(rb_cWXToolBarBase,"add_control",RUBY_METHOD_FUNC(_addControl),-1);

	rb_define_method(rb_cWXToolBarBase,"add_separator",RUBY_METHOD_FUNC(_AddSeparator),0);
	rb_define_method(rb_cWXToolBarBase,"add_stretchable_space",RUBY_METHOD_FUNC(_AddStretchableSpace),0);

	rb_define_method(rb_cWXToolBarBase,"insert_normal",RUBY_METHOD_FUNC(_insertNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_check",RUBY_METHOD_FUNC(_insertCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_radio",RUBY_METHOD_FUNC(_insertRadio),-1);
	rb_define_method(rb_cWXToolBarBase,"insert_control",RUBY_METHOD_FUNC(_insertControl),-1);

	rb_define_method(rb_cWXToolBarBase,"insert_separator",RUBY_METHOD_FUNC(_insert_separator),1);
	rb_define_method(rb_cWXToolBarBase,"insert_stretchable_space",RUBY_METHOD_FUNC(_insert_stretchable_space),1);

	rb_define_method(rb_cWXToolBarBase,"prepend_normal",RUBY_METHOD_FUNC(_prependNormal),-1);
	rb_define_method(rb_cWXToolBarBase,"prepend_check",RUBY_METHOD_FUNC(_prependCheck),-1);
	rb_define_method(rb_cWXToolBarBase,"prepend_radio",RUBY_METHOD_FUNC(_prependRadio),-1);
	rb_define_method(rb_cWXToolBarBase,"prepend_control",RUBY_METHOD_FUNC(_prependControl),-1);

	rb_define_method(rb_cWXToolBarBase,"prepend_separator",RUBY_METHOD_FUNC(_prepend_separator),0);
	rb_define_method(rb_cWXToolBarBase,"prepend_stretchable_space",RUBY_METHOD_FUNC(_prepend_stretchable_space),0);


	rb_define_method(rb_cWXToolBarBase,"realize",RUBY_METHOD_FUNC(_Realize),0);

	rb_define_method(rb_cWXToolBarBase,"vertical?",RUBY_METHOD_FUNC(_IsVertical),0);


	rb_define_method(rb_cWXToolBarBase,"each_tool",RUBY_METHOD_FUNC(_each),0);
#endif
}
