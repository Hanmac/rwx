/*
 * wxTreeCtrl.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#include "wxTreeCtrl.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"
#include "wxBitmap.hpp"

#include "wxPoint.hpp"

VALUE rb_cWXTreeCtrl,rb_cWXTreeCtrlItem,rb_cWXTreeCtrlEvent;
#if wxUSE_TREECTRL


namespace RubyWX {
namespace TreeCtrl {

#define _self unwrap<wxTreeCtrl*>(self)

macro_attr(Indent,unsigned int)
macro_attr(Spacing,unsigned int)

macro_attr(ImageList,wxImageList*)
macro_attr(StateImageList,wxImageList*)


APP_PROTECT(wxTreeCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
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
	VALUE result = wrap(_self,_self->AddRoot(unwrap<wxString>(str),cid,cselid));
	if(rb_block_given_p())
		rb_yield(result);
	return result;
}

VALUE _getSelection(VALUE self)
{
	return wrap(_self,_self->GetSelection());


}
VALUE _setSelection(VALUE self,VALUE item)
{
	_self->SelectItem(unwrap<wxTreeItemId>(item));
	return item;
}

namespace Item {
#undef _self
#define _self unwrapPtr<RubyTreeCtrlItem>(self, rb_cWXTreeCtrlItem)

macro_attr(State,int)
macro_attr(Bold,bool)
macro_attr(Text,wxString)
macro_attr(Font,wxFont)
macro_attr(TextColour,wxColour)
macro_attr(BackgroundColour,wxColour)

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
	VALUE result = _self->AppendItem(unwrap<wxString>(str),cid,cselid);

	if(rb_block_given_p())
		rb_yield(result);
	return result;

}

VALUE _PrependItem(int argc,VALUE *argv,VALUE self)
{
	VALUE str, id, selid;
	rb_scan_args(argc, argv, "12",&str, &id, &selid);
	int cid = -1,cselid = -1;
	if(!NIL_P(id))
		cid = NUM2INT(id);
	if(!NIL_P(selid))
		cid = NUM2INT(selid);
	VALUE result = _self->PrependItem(unwrap<wxString>(str),cid,cselid);

	if(rb_block_given_p())
		rb_yield(result);
	return result;

}

VALUE _GetNextSibling(VALUE self)
{
	return _self->GetNextSibling();
}

VALUE _GetPrevSibling(VALUE self)
{
	return _self->GetPrevSibling();
}

VALUE _compare(VALUE self,VALUE other)
{
	return _self->compare(unwrapPtr<RubyTreeCtrlItem>(self, rb_cWXTreeCtrlItem));
}

VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	_self->each();
	return self;
}


}

namespace Event {

#undef _self
#define _self unwrapPtr<wxTreeEvent>(self, rb_cWXTreeCtrlEvent)

macro_attr(Label,wxString)
macro_attr(ToolTip,wxString)

macro_attr(Point,wxPoint)

VALUE _GetItem(VALUE self)
{
	return wrap(static_cast<wxTreeCtrl*>(_self->GetEventObject()),_self->GetItem());
}
VALUE _SetItem(VALUE self,VALUE val)
{
	_self->SetItem(unwrap< wxTreeItemId >(val));
	return val;
}
VALUE _GetOldItem(VALUE self)
{
	return wrap(static_cast<wxTreeCtrl*>(_self->GetEventObject()),_self->GetOldItem());
}
VALUE _SetOldItem(VALUE self,VALUE val)
{
	_self->SetOldItem(unwrap< wxTreeItemId >(val));
	return val;
}

}

}
}

RubyTreeCtrlItem::RubyTreeCtrlItem(wxTreeCtrl* tree,wxTreeItemId id) : wxTreeItemData(),
		mRuby(wrapPtr(this,rb_cWXTreeCtrlItem)), mTree(tree)
{
	SetId(id);
	tree->SetItemData(id,this);
}

RubyTreeCtrlItem::~RubyTreeCtrlItem()
{
	SetId(NULL);
}

int RubyTreeCtrlItem::GetState()
{
	checkDestroyed();
	return mTree->GetItemState(GetId());
}

bool RubyTreeCtrlItem::GetBold()
{
	checkDestroyed();
	return mTree->IsBold(GetId());
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


void RubyTreeCtrlItem::SetState(int state)
{
	checkDestroyed();
	mTree->SetItemState(GetId(),state);
}

void RubyTreeCtrlItem::SetBold(bool bold)
{
	checkDestroyed();
	mTree->SetItemBold(GetId(),bold);
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

void RubyTreeCtrlItem::each()
{
	wxTreeItemIdValue cookie;
	wxTreeItemId id = mTree->GetFirstChild(GetId(), cookie);
	while(id) {
		rb_yield(wrap(mTree,id));
		id = mTree->GetNextChild(GetId(),cookie);
	}
}

VALUE RubyTreeCtrlItem::compare(RubyTreeCtrlItem *cother)
{
	if(!cother || GetId().IsOk() || !cother->GetId().IsOk() || mTree != cother->mTree)
		return Qnil;
	return INT2FIX(mTree->OnCompareItems(GetId(),cother->GetId()));

}

VALUE RubyTreeCtrlItem::AppendItem(const wxString& text, int image, int selImage)
{
	checkDestroyed();
	return wrap(mTree,mTree->AppendItem(GetId(),text,image,selImage));
}

VALUE RubyTreeCtrlItem::PrependItem(const wxString& text, int image, int selImage)
{
	checkDestroyed();
	return wrap(mTree,mTree->PrependItem(GetId(),text,image,selImage));
}

VALUE RubyTreeCtrlItem::GetNextSibling()
{
	checkDestroyed();
	return wrap(mTree,mTree->GetNextSibling(GetId()));
}

VALUE RubyTreeCtrlItem::GetPrevSibling()
{
	checkDestroyed();
	return wrap(mTree,mTree->GetPrevSibling(GetId()));
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
	rb_cWXTreeCtrl = rb_define_class_under(rb_mWX,"TreeCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXTreeCtrl,_alloc);

	rb_define_method(rb_cWXTreeCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXTreeCtrl,"root",RUBY_METHOD_FUNC(_root),-1);

	rb_define_attr_method(rb_cWXTreeCtrl,"indent",_getIndent,_setIndent);
	rb_define_attr_method(rb_cWXTreeCtrl,"spacing",_getSpacing,_setSpacing);

	rb_define_attr_method(rb_cWXTreeCtrl,"image_list",_getImageList,_setImageList);
	rb_define_attr_method(rb_cWXTreeCtrl,"state_image_list",_getStateImageList,_setStateImageList);

	rb_define_attr_method(rb_cWXTreeCtrl,"selection",_getSelection,_setSelection);


	registerInfo<wxTreeCtrl>(rb_cWXTreeCtrl);

	rb_cWXTreeCtrlItem = rb_define_class_under(rb_cWXTreeCtrl,"Item",rb_cObject);
	rb_undef_alloc_func(rb_cWXTreeCtrlItem);

	{

	using namespace Item;
	rb_define_attr_method(rb_cWXTreeCtrlItem,"state",_getState,_setState);
	rb_define_attr_method(rb_cWXTreeCtrlItem,"bold",_getBold,_setBold);
	rb_define_attr_method(rb_cWXTreeCtrlItem,"text",_getText,_setText);
	rb_define_attr_method(rb_cWXTreeCtrlItem,"font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXTreeCtrlItem,"text_color",_getTextColour,_setTextColour);
	rb_define_attr_method(rb_cWXTreeCtrlItem,"background_color",_getBackgroundColour,_setBackgroundColour);

	rb_define_method(rb_cWXTreeCtrlItem,"each",RUBY_METHOD_FUNC(_each),0);
	rb_define_method(rb_cWXTreeCtrlItem,"<=>",RUBY_METHOD_FUNC(_compare),1);
	rb_include_module(rb_cWXTreeCtrlItem,rb_mEnumerable);
	rb_include_module(rb_cWXTreeCtrlItem,rb_mComparable);

	rb_define_method(rb_cWXTreeCtrlItem,"next_sibling",RUBY_METHOD_FUNC(_GetNextSibling),0);
	rb_define_method(rb_cWXTreeCtrlItem,"prev_sibling",RUBY_METHOD_FUNC(_GetPrevSibling),0);


	rb_define_method(rb_cWXTreeCtrlItem,"delete",RUBY_METHOD_FUNC(_Delete),0);

	rb_define_method(rb_cWXTreeCtrlItem,"append",RUBY_METHOD_FUNC(_AppendItem),-1);

	}

	rb_cWXTreeCtrlEvent = rb_define_class_under(rb_cWXEvent,"TreeCtrl",rb_cWXEvent);
	registerEventType("tree_begin_drag", wxEVT_COMMAND_TREE_BEGIN_DRAG, rb_cWXTreeCtrlEvent);
	registerEventType("tree_begin_rdrag", wxEVT_COMMAND_TREE_BEGIN_RDRAG, rb_cWXTreeCtrlEvent);
	registerEventType("tree_begin_label_edit", wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, rb_cWXTreeCtrlEvent);
	registerEventType("tree_end_label_edit", wxEVT_COMMAND_TREE_END_LABEL_EDIT, rb_cWXTreeCtrlEvent);
	registerEventType("tree_delete_item", wxEVT_COMMAND_TREE_DELETE_ITEM, rb_cWXTreeCtrlEvent);
	registerEventType("tree_get_info", wxEVT_COMMAND_TREE_GET_INFO, rb_cWXTreeCtrlEvent);
	registerEventType("tree_set_info", wxEVT_COMMAND_TREE_SET_INFO, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_expanded", wxEVT_COMMAND_TREE_ITEM_EXPANDED, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_expanding", wxEVT_COMMAND_TREE_ITEM_EXPANDING, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_collapsed", wxEVT_COMMAND_TREE_ITEM_COLLAPSED, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_collaping", wxEVT_COMMAND_TREE_ITEM_COLLAPSING, rb_cWXTreeCtrlEvent);
	registerEventType("tree_sel_changed", wxEVT_COMMAND_TREE_SEL_CHANGED, rb_cWXTreeCtrlEvent);
	registerEventType("tree_sel_changing", wxEVT_COMMAND_TREE_SEL_CHANGING, rb_cWXTreeCtrlEvent);
	registerEventType("tree_key_down", wxEVT_COMMAND_TREE_KEY_DOWN, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_activated", wxEVT_COMMAND_TREE_ITEM_ACTIVATED, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_right_click", wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_middle_click", wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK, rb_cWXTreeCtrlEvent);
	registerEventType("tree_end_drag", wxEVT_COMMAND_TREE_END_DRAG, rb_cWXTreeCtrlEvent);
	registerEventType("tree_state_image_click", wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_gettooltip", wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP, rb_cWXTreeCtrlEvent);
	registerEventType("tree_item_menu", wxEVT_COMMAND_TREE_ITEM_MENU, rb_cWXTreeCtrlEvent);


	using namespace Event;
	rb_define_attr_method(rb_cWXTreeCtrlEvent,"label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXTreeCtrlEvent,"tooltip",_getToolTip,_setToolTip);
	rb_define_attr_method(rb_cWXTreeCtrlEvent,"point",_getPoint,_setPoint);
	rb_define_attr_method(rb_cWXTreeCtrlEvent,"item",_GetItem,_SetItem);
	rb_define_attr_method(rb_cWXTreeCtrlEvent,"old_item",_GetOldItem,_SetOldItem);
#endif
}


