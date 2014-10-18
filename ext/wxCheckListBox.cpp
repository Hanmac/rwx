/*
 * wxCheckListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxCheckListBox.hpp"

VALUE rb_cWXCheckListBox;

#if wxUSE_CHECKLISTBOX

namespace RubyWX {
namespace CheckListBox {
#define _self unwrap<wxCheckListBox*>(self)

APP_PROTECT(wxCheckListBox)

/*
 * call-seq:
 *   CheckListBox.new(parent, name, [options])
 *   CheckListBox.new(parent, [options])
 *
 * creates a new CheckListBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxArrayString choices;
		int style(0);
		int selection(-1);
		bool selflag(false);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);
			selflag = set_hash_option(hash,"selection",selection);

			ListBox::set_style_flags(hash,style);
		}
		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,
				choices,style
			);

			if(selflag && check_index(selection,_self->GetCount()))
				_self->SetSelection(selection);
		}
	}
	rb_call_super(argc,argv);
	return self;
}


DLL_LOCAL VALUE _each_checked_size(VALUE self)
{
	wxArrayInt data;
	return UINT2NUM(_self->GetCheckedItems(data));
}


/*
 * call-seq:
 *   each_checked -> Enumerator
 *   each_checked { |idx, text| } -> self
 *
 * iterates the checked items of the list box.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_checked(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_checked_size));

	wxArrayInt data;
	_self->GetCheckedItems(data);

	for(wxArrayInt::iterator it = data.begin(); it != data.end();++it)
		rb_yield_values(2,INT2NUM(*it),wrap(_self->GetString(*it)));

	return self;
}


DLL_LOCAL VALUE _getCheckedItems(VALUE self)
{
	wxArrayInt data;
	_self->GetCheckedItems(data);
	return wrap(data);
}

/*
 * call-seq:
 *   get_item_checked(pos) -> true/false
 *
 * Determines whether an item is checked.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _getItemChecked(VALUE self,VALUE idx)
{
	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		return wrap(_self->IsChecked(cidx));
	return Qnil;
}

/*
 * call-seq:
 *   set_item_selection(pos,val) -> self
 *
 * sets the item at the position as checked if val is true.
 * ===Arguments
 * * pos of the item. Integer
 * * val if item should be checked. true/false
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/
DLL_LOCAL VALUE _setItemChecked(VALUE self,VALUE idx,VALUE val)
{
	rb_check_frozen(self);

	int cidx = NUM2INT(idx);
	if(check_index(cidx,_self->GetCount()))
		_self->Check(cidx,RTEST(val));

	return self;
}



}
}

#endif
DLL_LOCAL void Init_WXCheckListBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
#endif

#if wxUSE_CHECKLISTBOX
	using namespace RubyWX::CheckListBox;
	rb_cWXCheckListBox = rb_define_class_under(rb_mWX,"CheckListBox",rb_cWXListBox);
	rb_define_alloc_func(rb_cWXCheckListBox,_alloc);

	rb_define_method(rb_cWXCheckListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_define_method(rb_cWXCheckListBox,"each_checked",RUBY_METHOD_FUNC(_each_checked),0);
	rb_define_method(rb_cWXCheckListBox,"checked_items",RUBY_METHOD_FUNC(_getCheckedItems),0);

	rb_define_method(rb_cWXListBox,"get_item_checked",RUBY_METHOD_FUNC(_getItemChecked),1);
	rb_define_method(rb_cWXListBox,"set_item_checked",RUBY_METHOD_FUNC(_setItemChecked),2);


	registerEventType("checklistbox", wxEVT_CHECKLISTBOX,rb_cWXCommandEvent);

	registerInfo<wxCheckListBox>(rb_cWXCheckListBox);
#endif

}


