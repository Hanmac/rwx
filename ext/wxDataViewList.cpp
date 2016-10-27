/*
 * wxDataViewList.cpp
 *
 *  Created on: 24.03.2012
 *      Author: hanmac
 */


#include "wxDataViewList.hpp"
#include "wxVariant.hpp"

VALUE rb_cWXDataViewList;

#if wxUSE_DATAVIEWCTRL

class RubyDataViewListStore : public wxDataViewListStore, public DataViewClientHolder
{
public:
	RubyDataViewListStore() : wxDataViewListStore(), DataViewClientHolder()
	{
		AddNotifier(new RubyDataViewNotifier);
	}
};

namespace RubyWX {
namespace DataViewList {
#define _self unwrap<wxDataViewListCtrl*>(self)
//macro_attr(Path,wxString)

APP_PROTECT(wxDataViewListCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
		}

		_self->Create(unwrap<wxWindow*>(parent),id);
		_self->AssociateModel(new RubyDataViewListStore);
	}

	rb_call_super(argc,argv);
	
	return self;
}

singlefunc(DeleteAllItems);

DLL_LOCAL VALUE _AppendTextColumn(int argc,VALUE *argv,VALUE self)
{
	VALUE label,hash;
	rb_scan_args(argc, argv, "10",&label,&hash);

	_self->AppendTextColumn(unwrap<wxString>(label));
	return self;
}

DLL_LOCAL VALUE _AppendToggleColumn(int argc,VALUE *argv,VALUE self)
{
	VALUE label,hash;
	rb_scan_args(argc, argv, "11",&label,&hash);

	_self->AppendToggleColumn(unwrap<wxString>(label));
	return self;
}

DLL_LOCAL VALUE _AppendIconTextColumn(int argc,VALUE *argv,VALUE self)
{
	VALUE label,hash;
	rb_scan_args(argc, argv, "11",&label,&hash);

	_self->AppendIconTextColumn(unwrap<wxString>(label));
	return self;
}

DLL_LOCAL VALUE _AppendProgressColumn(int argc,VALUE *argv,VALUE self)
{
	VALUE label,hash;
	rb_scan_args(argc, argv, "11",&label,&hash);

	_self->AppendProgressColumn(unwrap<wxString>(label));
	return self;
}


DLL_LOCAL VALUE _AppendItem(int argc,VALUE *argv,VALUE self)
{
	VALUE arg;
	rb_scan_args(argc, argv, "*",&arg);

	unsigned int count = _self->GetColumnCount();
	if(count > (unsigned int)argc)
		rb_raise(rb_eArgError,"to few arguments need more then %d.",(int)count);

	wxVector<wxVariant> vecvar;

	for(unsigned int i = 0; i < count;++i)
		vecvar.push_back(unwrapVariant(RARRAY_AREF(arg,i),_self->GetColumn(i)->GetRenderer()->GetVariantType()));
	_self->AppendItem(vecvar,0);
	return wrap(_self->GetModel(),_self->GetStore()->GetItem(_self->GetStore()->GetCount() - 1));
}

DLL_LOCAL VALUE _PrependItem(int argc,VALUE *argv,VALUE self)
{
	VALUE arg;
	rb_scan_args(argc, argv, "*",&arg);

	unsigned int count = _self->GetColumnCount();
	if(count > (unsigned int)argc)
		rb_raise(rb_eArgError,"to few arguments need more then %d.",(int)count);

	wxVector<wxVariant> vecvar;

	for(unsigned int i = 0; i < count;++i){
		vecvar.push_back(unwrapVariant(RARRAY_AREF(arg,i),_self->GetColumn(i)->GetRenderer()->GetVariantType()));
	}
	_self->PrependItem(vecvar,0);
	return wrap(_self->GetModel(),_self->GetStore()->GetItem(0));
}

DLL_LOCAL VALUE _InsertItem(int argc,VALUE *argv,VALUE self)
{
	VALUE index,arg;
	rb_scan_args(argc, argv, "1*",&index,&arg);

	unsigned int count = _self->GetColumnCount() ;
	if((count + 1) > (unsigned int)argc)
		rb_raise(rb_eArgError,"to few arguments need more then %d.",(int)count + 1);

	wxVector<wxVariant> vecvar;

	for(unsigned int i = 0; i < count;++i)
		vecvar.push_back(unwrapVariant(RARRAY_AREF(arg,i),_self->GetModel()->GetColumnType(i)));
	_self->InsertItem(RB_NUM2INT(index),vecvar,0);
	return wrap(_self->GetModel(),_self->GetStore()->GetItem(RB_NUM2INT(index)));
}


}
}




#endif
DLL_LOCAL void Init_WXDataViewList(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXDataView = rb_define_class_under(rb_mWX,"DataView",rb_cWXControl);

#endif
#if wxUSE_DATAVIEWCTRL
	using namespace RubyWX::DataViewList;
	rb_cWXDataViewList = rb_define_class_under(rb_mWX,"DataViewList",rb_cWXDataView);
	rb_define_alloc_func(rb_cWXDataViewList,_alloc);

	rb_define_method(rb_cWXDataViewList,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

//	rb_define_method(rb_cWXDataViewList,"append_text_column",RUBY_METHOD_FUNC(_AppendTextColumn),-1);
//	rb_define_method(rb_cWXDataViewList,"append_toggle_column",RUBY_METHOD_FUNC(_AppendToggleColumn),-1);
//	rb_define_method(rb_cWXDataViewList,"append_icontext_column",RUBY_METHOD_FUNC(_AppendIconTextColumn),-1);
//	rb_define_method(rb_cWXDataViewList,"append_progress_column",RUBY_METHOD_FUNC(_AppendProgressColumn),-1);


	rb_define_method(rb_cWXDataViewList,"append_item",RUBY_METHOD_FUNC(_AppendItem),-1);
	rb_define_method(rb_cWXDataViewList,"prepend_item",RUBY_METHOD_FUNC(_PrependItem),-1);
	rb_define_method(rb_cWXDataViewList,"insert_item",RUBY_METHOD_FUNC(_InsertItem),-1);

	rb_define_method(rb_cWXDataViewList,"delete_items",RUBY_METHOD_FUNC(_DeleteAllItems),0);


//	rb_define_attr_method(rb_cWXDataView,"path",_getPath,_setPath);

	registerInfo< wxDataViewListCtrl >(rb_cWXDataViewList);

#endif

}



