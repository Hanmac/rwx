/*
 * wxDataView.cpp
 *
 *  Created on: 24.03.2012
 *      Author: hanmac
 */

#include "wxControl.hpp"
#include "wxDataView.hpp"
#include "wxBitmap.hpp"
#include "wxVariant.hpp"

VALUE rb_cWXDataView, rb_cWXDataViewEvent,rb_cWXDataViewItem;

#if wxUSE_DATAVIEWCTRL

wxClientData* DataViewClientHolder::getClientValue(const wxDataViewItem & item) const
{
	std::map<wxDataViewItem,wxClientData*>::const_iterator it = m_clientvalues.find(item);
	if(it != m_clientvalues.end())
		return it->second;
	return NULL;
}

void DataViewClientHolder::setClientValue( const wxDataViewItem& item, wxClientData *data )
{
	if(data){
		m_clientvalues.insert(std::make_pair(item,data));
	}else
		m_clientvalues.erase(item);
}

bool RubyDataViewNotifier::ItemAdded( const wxDataViewItem &parent, const wxDataViewItem &item )
{
	dynamic_cast<DataViewClientHolder*>(this->GetOwner())->setClientValue(item,new RubyClientData(wrapPtr((void*)(&item),rb_cWXDataViewItem)));
	return true;
}
bool RubyDataViewNotifier::ItemDeleted( const wxDataViewItem &parent, const wxDataViewItem &item )
{
	dynamic_cast<DataViewClientHolder*>(this->GetOwner())->setClientValue(item,NULL);
	return true;
}
bool RubyDataViewNotifier::ItemChanged( const wxDataViewItem &item )
{
	return true;
}

bool RubyDataViewNotifier::ValueChanged( const wxDataViewItem &item, unsigned int col )
{
	return true;
}
bool RubyDataViewNotifier::Cleared()
{

	return true;
}

void RubyDataViewNotifier::Resort()
{}



VALUE wrap(wxDataViewModel *model, const wxDataViewItem& item)
{
	if(!item.IsOk())
		return Qnil;
	wxClientData* cd = dynamic_cast<DataViewClientHolder*>(model)->getClientValue(item);
	return static_cast<RubyClientData*>(cd)->mRuby;
}

namespace RubyWX {
namespace DataView {
#define _self unwrap<wxDataViewCtrl*>(self)
//macro_attr(Path,wxString)

APP_PROTECT(wxDataViewCtrl)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	//_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	//_created = true;
	rb_call_super(argc,argv);
	return self;
}

VALUE _getSelection(VALUE self)
{
	return wrap(_self->GetModel(),_self->GetSelection());
}

VALUE _setSelection(VALUE self,VALUE item)
{
	//_self->Select(unwrap<wxDataViewItem>(item));
	return self;
}


namespace Event {
#undef _self
#define _self unwrapPtr<wxDataViewEvent>(self,rb_cWXDataViewEvent)
VALUE _getValue(VALUE self)
{
	//omg is this ugly
	wxVariant var;
	_self->GetModel()->GetValue(var,_self->GetItem(),_self->GetColumn());
	return wrap(var);
}
VALUE _setValue(VALUE self,VALUE val)
{
	_self->SetValue(unwrapVariant(val,
			_self->GetModel()->GetColumnType(_self->GetColumn())));
	return val;
}

macro_attr(Column,int)

VALUE _getItem(VALUE self)
{
	return wrap(_self->GetModel(),_self->GetItem());
}

VALUE _setItem(VALUE self,VALUE item)
{
	return item;
}

}

}
}

#endif
void Init_WXDataView(VALUE rb_mWX)
{
#if wxUSE_DATAVIEWCTRL
	using namespace RubyWX::DataView;
	rb_cWXDataView = rb_define_class_under(rb_mWX,"DataView",rb_cWXControl);
	//rb_define_alloc_func(rb_cWXDataView,_alloc);

	rb_define_method(rb_cWXDataView,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXDataView,"selection",_getSelection,_setSelection);

	rb_cWXDataViewEvent = rb_define_class_under(rb_cWXEvent,"DataView",rb_cWXEvent);
	registerEventType("dataview_selection_changed",wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED,rb_cWXDataViewEvent);
	registerEventType("dataview_value_changed",wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED,rb_cWXDataViewEvent);

	using namespace Event;
	rb_define_attr_method(rb_cWXDataViewEvent,"value",_getValue,_setValue);
	rb_define_attr_method(rb_cWXDataViewEvent,"column",_getColumn,_setColumn);
	rb_define_attr_method(rb_cWXDataViewEvent,"item",_getItem,_setItem);

//	rb_define_attr_method(rb_cWXFileDataViewEvent,"path",
//			Event::_getPath,Event::_setPath);

	rb_cWXDataViewItem = rb_define_class_under(rb_cWXDataView,"Item",rb_cObject);
	rb_undef_alloc_func(rb_cWXDataViewItem);

	registerInfo<wxDataViewCtrl>(rb_cWXDataView);
#endif

}

