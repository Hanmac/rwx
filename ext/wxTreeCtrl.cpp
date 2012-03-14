/*
 * wxTreeCtrl.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

#if wxUSE_TREECTRL
VALUE rb_cWXTreeCtrl,rb_cWXTreeCtrlItem;
#define _self wrap<wxTreeCtrl*>(self)

namespace RubyWX {
namespace TreeCtrl {

macro_attr(Indent,unsigned int)
macro_attr(Spacing,unsigned int)

macro_attr(ImageList,wxImageList*)
macro_attr(StateImageList,wxImageList*)


VALUE _alloc(VALUE self)
{
	return getEvtObj(new wxTreeCtrl(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	rb_call_super(argc,argv);
	return self;
}

VALUE _root(int argc,VALUE *argv,VALUE self)
{
	VALUE str, id, selid;
	rb_scan_args(argc, argv, "03",&str, &id, &selid);
	int cid = -1,cselid = -1;
	if(NIL_P(str) || _self->GetRootItem().IsOk())
		return wrap(_self,_self->GetRootItem());
	if(!NIL_P(id))
		cid = NUM2INT(id);
	if(!NIL_P(selid))
		cid = NUM2INT(selid);
	VALUE result = wrap(_self,_self->AddRoot(wrap<wxString>(str),cid,cselid));
	if(rb_block_given_p())
		rb_yield(result);
	return result;
}

namespace Item {
#undef _self
#define _self unwrapPtr<RubyTreeCtrlItem>(self, rb_cWXTreeCtrlItem)

macro_attr(Text,wxString)

singlefunc(Delete)

VALUE _AppendItem(int argc,VALUE *argv,VALUE self)
{
	VALUE str, id, selid;
	rb_scan_args(argc, argv, "12",&str, &id, &selid);
	int cid = -1,cselid = -1;
	if(!NIL_P(id))
		cid = NUM2INT(id);
	if(!NIL_P(selid))
		cid = NUM2INT(selid);
	VALUE result = _self->AppendItem(wrap<wxString>(str),cid,cselid);

	if(rb_block_given_p())
		rb_yield(result);
	return result;

}
}

}
}

RubyTreeCtrlItem::RubyTreeCtrlItem(wxTreeCtrl* tree,wxTreeItemId id) : wxTreeItemData(),
		mRuby(wrap(this,rb_cWXTreeCtrlItem)), mTree(tree)
{
	SetId(id);
	tree->SetItemData(id,this);
}

RubyTreeCtrlItem::~RubyTreeCtrlItem()
{
	SetId(NULL);
}

wxString RubyTreeCtrlItem::GetText()
{
	checkDestroyed();
	return mTree->GetItemText(GetId());
}

wxColour RubyTreeCtrlItem::GetTextColour()
{
	checkDestroyed();
	return mTree->GetItemTextColour(GetId());
}

wxColour RubyTreeCtrlItem::GetBackgroundColour()
{
	checkDestroyed();
	return mTree->GetItemBackgroundColour(GetId());
}

wxFont RubyTreeCtrlItem::GetFont()
{
	checkDestroyed();
	return mTree->GetItemFont(GetId());
}


void RubyTreeCtrlItem::SetText(const wxString& text)
{
	checkDestroyed();
	mTree->SetItemText(GetId(),text);
}
void RubyTreeCtrlItem::SetTextColour(const wxColour& col)
{
	checkDestroyed();
	mTree->SetItemTextColour(GetId(),col);
}
void RubyTreeCtrlItem::SetBackgroundColour(const wxColour& col)
{
	checkDestroyed();
	mTree->SetItemBackgroundColour(GetId(),col);
}
void RubyTreeCtrlItem::SetFont(const wxFont& font)
{
	checkDestroyed();
	mTree->SetItemFont(GetId(),font);
}

void RubyTreeCtrlItem::Delete()
{
	checkDestroyed();
	mTree->Delete(GetId());
}


VALUE RubyTreeCtrlItem::AppendItem(const wxString& text, int image, int selImage)
{
	checkDestroyed();
	return wrap(mTree,mTree->AppendItem(GetId(),text,image,selImage));
}



void RubyTreeCtrlItem::checkDestroyed()
{
	if(!GetId().IsOk())
		rb_raise(rb_eTypeError,"%s has been destroyed",rb_class2name(rb_cWXTreeCtrlItem));
}


VALUE wrap(wxTreeCtrl* tree,wxTreeItemId id)
{
	if(!id.IsOk())
		return Qnil;
	if(tree->GetItemData(id))
	{
		return dynamic_cast<RubyTreeCtrlItem*>(tree->GetItemData(id))->getRuby();
	}else
	{
		RubyTreeCtrlItem *itemdata = new RubyTreeCtrlItem(tree,id);
		return itemdata->getRuby();
	}
}

#endif
void Init_WXTreeCtrl(VALUE rb_mWX)
{
#if wxUSE_TREECTRL
	using namespace RubyWX::TreeCtrl;
	rb_cWXTreeCtrl = rb_define_class_under(rb_mWX,"TreeCtrl",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXTreeCtrl,_alloc);

	rb_define_method(rb_cWXTreeCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXTreeCtrl,"root",RUBY_METHOD_FUNC(_root),-1);

	rb_cWXTreeCtrlItem = rb_define_class_under(rb_cWXTreeCtrl,"Item",rb_cObject);
	rb_undef_alloc_func(rb_cWXTreeCtrlItem);

	using namespace RubyWX::TreeCtrl::Item;
	rb_define_attr_method(rb_cWXTreeCtrlItem,"text",_getText,_setText);
	rb_define_method(rb_cWXTreeCtrlItem,"delete",RUBY_METHOD_FUNC(_Delete),0);

	rb_define_method(rb_cWXTreeCtrlItem,"append",RUBY_METHOD_FUNC(_AppendItem),-1);

#endif
}


