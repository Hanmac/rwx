/*
 * wxStream.cpp
 *
 *  Created on: 22.02.2012
 *      Author: hanmac
 */
#include "wxStream.hpp"
#if wxUSE_STREAMS
wxFileOffset RubyInputStream::OnSysSeek(wxFileOffset seek, wxSeekMode mode)
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("seek"),2,INT2NUM(seek),INT2NUM(mode)));
}

wxFileOffset RubyInputStream::OnSysTell() const
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("tell"),0));
}

bool RubyInputStream::IsSeekable() const
{
	return rb_respond_to(mRuby,rb_intern("seek"));
}

bool RubyInputStream::Eof() const
{
	return RTEST(rb_funcall(mRuby,rb_intern("eof?"),0));
}

bool RubyInputStream::CanRead() const
{
	return !Eof() && !RTEST(rb_funcall(mRuby,rb_intern("closed?"),0));
}

size_t RubyInputStream::OnSysRead(void *buffer, size_t size)
{
	VALUE str = rb_funcall(mRuby,rb_intern("read"),1,INT2NUM(size));
	size_t s = RSTRING_LEN(str);
	memcpy(buffer, RSTRING_PTR(str), s);
	return s;
}
#endif
