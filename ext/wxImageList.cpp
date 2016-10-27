/*
 * wxImageList.cpp
 *
 *  Created on: 11.05.2015
 *      Author: hanmac
 */

#include "wxImageList.hpp"
#include "wxColor.hpp"

#define _self unwrap<wxImageList*>(self)

VALUE rb_cWXImageList;

/*
class RubyImageList {

public:

	wxImageList* GetImageList() const { m_imagelist; }

	bool Create( int width, int height, bool mask = true, int initialCount = 1 ); {
		m_width_ruby = width;
		m_height_ruby = height;
		return wxImageList::Create(width, height, mask, initialCount);
	}
	bool Create() { return wxImageList::Create(); }

	int GetWidth() { return m_width_ruby; };
	int GetHeight() { return m_height_ruby; };

private:
	int m_width_ruby;
	int m_height_ruby;
	wxImageList* m_imagelist;
}
//*/


template <>
VALUE wrap< wxImageList >(wxImageList *imagelist )
{
	return wrapTypedPtr(imagelist,rb_cWXImageList);
}

template <>
wxImageList* unwrap< wxImageList* >(const VALUE &imagelist)
{
	if(rb_obj_is_kind_of(imagelist, rb_cWXImageList)){
		return unwrapTypedPtr<wxImageList>(imagelist,rb_cWXImageList);
	}

	wxImageList *result = new wxImageList;
	VALUE dup(rb_Array(imagelist));

	std::size_t count = RARRAY_LEN(dup);

	if(count == 0) {
		result->Create();
		return result;
	}

	int width(0), height(0);
	if(is_wrapable< wxSize >(RARRAY_AREF(dup,0))) {
		wxSize size = unwrap<wxSize>(RARRAY_AREF(dup,0));
		width = size.GetWidth();
		height = size.GetHeight();
	} else {
		wxBitmap bitmap = unwrap<wxBitmap>(RARRAY_AREF(dup,0));
		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
	}

	result->Create(width, height);

	for(std::size_t i = 0;i < count;++i) {
		wxBitmap bitmap = unwrap<wxBitmap>(RARRAY_AREF(dup,i));
#if wxUSE_IMAGE
			bitmap = bitmap.ConvertToImage().Scale(width, height);
#endif
		result->Add(bitmap);
		
	}
	return result;
}

namespace RubyWX {
namespace ImageList {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrapTypedPtr(new wxImageList, self);
}

singlereturn(GetImageCount)
singlefunc(RemoveAll)

/*
 * call-seq:
 *   ImageList.new([width, height])
 *
 * creates a new ImageList Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE width, height;
	rb_scan_args(argc, argv, "02",&width,&height);

	if(!NIL_P(width)) {
		_self->Create(RB_NUM2UINT(width), RB_NUM2UINT(height));
	} else {
		_self->Create();
	}

	return self;
}

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);

	return result;
}



/*
 * call-seq:
 *   each_item -> Enumerator
 *   each_item { | bitmap| } -> self
 *
 * iterates the bitmaps of this control.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetImageCount));
	for(int i = 0; i < _self->GetImageCount(); ++i)
		rb_yield(wrap(_self->GetBitmap(i)));
	return self;
}


DLL_LOCAL VALUE _add(int argc,VALUE *argv,VALUE self)
{
	VALUE bitmap, mask, opt;
	rb_scan_args(argc, argv, "11:",&bitmap,&mask,&opt);
	
	wxBitmap cbitmap(unwrap<wxBitmap>(bitmap));

	if(RTEST(rb_hash_aref(opt, RB_ID2SYM(rb_intern("scale"))))) {
		if(_self->GetImageCount() > 0) {
#if wxUSE_IMAGE
			int width(0), height(0);
			_self->GetSize(0, width, height);
			cbitmap = cbitmap.ConvertToImage().Scale(width, height);
#endif
		}
	}
	
	if(NIL_P(mask)) {
		_self->Add(cbitmap);
	} else if(is_wrapable< wxColor >(mask)) {
		_self->Add(cbitmap, unwrap<wxColor>(mask));
	} else {
		_self->Add(cbitmap, unwrap<wxBitmap>(mask));
	}
	
	return self;
}

DLL_LOCAL VALUE _get(VALUE self, VALUE idx)
{
	int cidx = RB_NUM2INT(idx);
	if(check_index(cidx,_self->GetImageCount()))
		return wrap(_self->GetBitmap(cidx));
	return Qnil;
}

}
}


DLL_LOCAL void Init_WXImageList(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
#endif

	using namespace RubyWX::ImageList;
	rb_cWXImageList = rb_define_class_under(rb_mWX,"ImageList",rb_cObject);
	rb_define_alloc_func(rb_cWXImageList,_alloc);

	rb_define_method(rb_cWXImageList,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_undef_method(rb_cWXImageList,"initialize_copy");

	rb_define_method(rb_cWXImageList,"add",RUBY_METHOD_FUNC(_add),-1);
	rb_define_method(rb_cWXImageList,"[]",RUBY_METHOD_FUNC(_get),1);

	rb_define_method(rb_cWXImageList,"clear",RUBY_METHOD_FUNC(_RemoveAll),0);

	rb_define_method(rb_cWXImageList,"each",RUBY_METHOD_FUNC(_each),0);
	rb_include_module(rb_cWXImageList,rb_mEnumerable);

	rb_undef_method(rb_cWXImageList,"marshal_dump");
	rb_undef_method(rb_cWXImageList,"marshal_load");


	registerType<wxImageList>(rb_cWXImageList,true);


}


