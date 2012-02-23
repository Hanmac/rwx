/*
 * wxStream.hpp
 *
 *  Created on: 22.02.2012
 *      Author: hanmac
 */

#ifndef WXSTREAM_HPP_
#define WXSTREAM_HPP_

#include "main.hpp"

class RubyBaseStream : virtual public wxStreamBase{
public:
	RubyBaseStream(VALUE obj);

protected:
	wxFileOffset OnSysSeek(wxFileOffset seek, wxSeekMode mode);
	wxFileOffset OnSysTell() const;



	VALUE mRuby;
};

class RubyInputStream : virtual public wxInputStream, public RubyBaseStream
{
public:
	RubyInputStream(VALUE obj);

	bool Eof() const;
	bool CanRead() const;

	bool IsSeekable() const {
		return true;
	}

	wxFileOffset SeekI(wxFileOffset pos, wxSeekMode mode = wxFromStart);
	wxFileOffset TellI() const;
protected:
	size_t OnSysRead(void *buffer, size_t size);

};

class RubyOutputStream :virtual public wxOutputStream, public RubyBaseStream
{
public:
	RubyOutputStream(VALUE obj);

	bool IsSeekable() const {
		return true;
	}
protected:
	size_t OnSysWrite(const void *buffer, size_t bufsize);
};

#endif /* WXSTREAM_HPP_ */
