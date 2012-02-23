/*
 * wxStream.cpp
 *
 *  Created on: 22.02.2012
 *      Author: hanmac
 */
#include "wxStream.hpp"

RubyBaseStream::RubyBaseStream(VALUE obj) : mRuby(obj) {}

wxFileOffset RubyBaseStream::OnSysSeek(wxFileOffset seek, wxSeekMode mode)
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("seek"),2,INT2NUM(seek),INT2NUM(mode)));
}
wxFileOffset RubyBaseStream::OnSysTell() const
{
	return NUM2INT(rb_funcall(mRuby,rb_intern("tell"),0));
}


RubyInputStream::RubyInputStream(VALUE obj) : RubyBaseStream(obj) {}

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
	//std::cout << "read1" << std::endl;
	buffer = wrap<char*>(rb_funcall(mRuby,rb_intern("read"),1,INT2NUM(size)));
	//std::cout << "read2" << std::endl;
	//return size;
	return strlen((char*)buffer);
}


wxFileOffset RubyInputStream::SeekI(wxFileOffset pos, wxSeekMode mode)
{
	return RubyBaseStream::OnSysSeek(pos, mode);
}
wxFileOffset RubyInputStream::TellI() const
{
	return RubyBaseStream::OnSysTell();
}

