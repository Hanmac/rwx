/*
 * wxPropertyCell.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyCell.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"
#include "wxBitmap.hpp"

VALUE rb_cWXPropertyCell;

#if wxUSE_PROPGRID
#define _self unwrap<wxPGCell*>(self)

namespace RubyWX {
namespace PropertyCell {

macro_attr(Text,wxString)
macro_attr(Font,wxFont)
macro_attr(Bitmap,wxBitmap)
macro_attr(FgCol,wxColor)
macro_attr(BgCol,wxColor)

/*
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
DLL_LOCAL VALUE _inspect(VALUE self)
{
	VALUE array[3];
	array[0]=rb_str_new2("#<%s:(%s)>");
	array[1]=rb_class_of(self);
	array[2]=_getText(self);
	return rb_f_sprintf(3,array);
}


}
}
#endif
DLL_LOCAL void Init_WXPropertyCell(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
#if 0
	rb_define_attr(rb_cWXPropertyCell,"text",1,1);
	rb_define_attr(rb_cWXPropertyCell,"font",1,1);
	rb_define_attr(rb_cWXPropertyCell,"bitmap",1,1);
	rb_define_attr(rb_cWXPropertyCell,"fg_col",1,1);
	rb_define_attr(rb_cWXPropertyCell,"bg_col",1,1);
#endif

	using namespace RubyWX::PropertyCell;
	rb_cWXPropertyCell = rb_define_class_under(rb_mWX,"PropertyCell",rb_cObject);
	rb_undef_alloc_func(rb_cWXPropertyCell);

	rb_undef_method(rb_cWXPropertyCell,"_load");
	rb_undef_method(rb_cWXPropertyCell,"_dump");

	rb_define_method(rb_cWXPropertyCell,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_attr_method(rb_cWXPropertyCell,"text",_getText,_setText);
	rb_define_attr_method(rb_cWXPropertyCell,"font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXPropertyCell,"bitmap",_getBitmap,_setBitmap);
	rb_define_attr_method(rb_cWXPropertyCell,"fg_col",_getFgCol,_setFgCol);
	rb_define_attr_method(rb_cWXPropertyCell,"bg_col",_getBgCol,_setBgCol);


	registerInfo<wxPGCell>(rb_cWXPropertyCell);
#endif

}
