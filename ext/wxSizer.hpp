/*
 * wxSizer.hpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */

#ifndef WXSIZER_HPP_
#define WXSIZER_HPP_

#include "wxBoxSizer.hpp"

extern VALUE rb_cWXSizer;

void Init_WXSizer(VALUE rb_mWX);

template <>
inline VALUE wrap< wxSizer >(wxSizer* window)
{
	if(wxBoxSizer *box = dynamic_cast<wxBoxSizer*>(window))
		return wrap(box);
	return wrap(window,rb_cWXSizer);
}


template <>
inline wxSizer* wrap< wxSizer* >(const VALUE &vwindow)
{
	return unwrapPtr<wxSizer>(vwindow, rb_cWXSizer);
}

template <>
inline wxSizerFlags wrap< wxSizerFlags >(const VALUE &hash)
{
	wxSizerFlags result;
	if(!rb_obj_is_kind_of(hash,rb_cHash))
		return result;
	VALUE val;

	if(RTEST(rb_hash_aref(hash,ID2SYM(rb_intern("expand")))))
		result.Expand();
	if(!NIL_P(val=rb_hash_aref(hash,ID2SYM(rb_intern("proportion")))))
			result.Proportion(NUM2INT(val));
	if(!NIL_P(val=rb_hash_aref(hash,ID2SYM(rb_intern("align")))))
	{
		if(SYMBOL_P(val))
		{
			if(SYM2ID(val) == rb_intern("left"))
				result.Left();
			if(SYM2ID(val) == rb_intern("right"))
				result.Right();
			if(SYM2ID(val) == rb_intern("bottom"))
				result.Bottom();
			if(SYM2ID(val) == rb_intern("top"))
				result.Top();
			if(SYM2ID(val) == rb_intern("center"))
				result.Center();

		}
	}

	return result;
}

#endif /* WXSIZER_HPP_ */
