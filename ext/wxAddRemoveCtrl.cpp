/*
 * wxAddRemoveCtrl.cpp
 *
 *  Created on: 27.04.2015
 *      Author: hanmac
 */


#include "wxAddRemoveCtrl.hpp"

VALUE rb_cWXAddRemoveCtrl;

#if wxUSE_ADDREMOVECTRL

#define _self unwrap<wxAddRemoveCtrl*>(self)

class RubyAddRemoveAdaptor : public RubyClientData, public wxAddRemoveAdaptor {
public:
	RubyAddRemoveAdaptor(VALUE self) : RubyClientData(self) {
	};
    virtual wxWindow* GetItemsCtrl() const {
    	return unwrap<wxWindow*>(call_function("item_ctrl"));
    };

    virtual bool CanAdd() const {
    	return RTEST(call_function("can_add?"));
    };

    virtual bool CanRemove() const {
    	return RTEST(call_function("can_remove?"));
    };

    virtual void OnAdd() { call_function("on_add");};

    virtual void OnRemove() { call_function("on_remove");};
private:
	VALUE call_function(const char *id) const {
		return rb_funcall(mRuby,rb_intern(id),0);
	}
};


namespace RubyWX {
namespace AddRemoveCtrl {

APP_PROTECT(wxAddRemoveCtrl)

/*
 * call-seq:
 *   AddRemoveCtrl.new(parent, name, [options])
 *   AddRemoveCtrl.new(parent, [options])
 *
 * creates a new AddRemoveCtrl widget.
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
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		wxWindowID id(wxID_ANY);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,style
			);

		}

	}
	rb_call_super(argc,argv);

	_self->SetAdaptor(new RubyAddRemoveAdaptor(self));
	return self;
}

VALUE _itemlist(VALUE self) {
	return Qnil;
}


VALUE _can_add(VALUE self) {
	return Qfalse;
}

VALUE _can_remove(VALUE self) {
	return Qfalse;
}

VALUE _on_add(VALUE self) {
	return Qnil;
}
VALUE _on_remove(VALUE self) {
	return Qnil;
}



}
}

#endif

DLL_LOCAL void Init_WXAddRemoveCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXPanel = rb_define_class_under(rb_mWX,"Panel",rb_cWXWindow);
#endif

#if wxUSE_ADDREMOVECTRL

	using namespace RubyWX::AddRemoveCtrl;
	rb_cWXAddRemoveCtrl = rb_define_class_under(rb_mWX,"AddRemoveCtrl",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXAddRemoveCtrl,_alloc);

	rb_define_method(rb_cWXAddRemoveCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXAddRemoveCtrl,"item_ctrl",RUBY_METHOD_FUNC(_itemlist),0);
	rb_define_method(rb_cWXAddRemoveCtrl,"can_add?",RUBY_METHOD_FUNC(_can_add),0);
	rb_define_method(rb_cWXAddRemoveCtrl,"can_remove?",RUBY_METHOD_FUNC(_can_remove),0);
	rb_define_method(rb_cWXAddRemoveCtrl,"on_add",RUBY_METHOD_FUNC(_on_add),0);
	rb_define_method(rb_cWXAddRemoveCtrl,"on_remove",RUBY_METHOD_FUNC(_on_remove),0);

	registerInfo<wxAddRemoveCtrl>(rb_cWXAddRemoveCtrl);

#endif
}
