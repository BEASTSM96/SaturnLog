/*
MIT License

Copyright (c) 2020 BEAST

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Base.h"
#include <vector>

namespace LOGNAMESPACE {

	using Message = const char*;

	enum Severity : u8
	{
		Trace,
		Info,
		Warn,
		Error,
		Critical
	};

	class LogSink
	{
	public:
		virtual void Flush() {} ;
		virtual void Print() {} ;
		virtual void Add(Message msg) {} ;
	protected:
		int MsgAmt = 0;
		std::vector<Message> Msgs;
	};

	class FileSink : LogSink
	{
	public:
		virtual void Flush() override;
		virtual void Print() override;
		virtual void Add(Message msg) override;
	};

	class Logger
	{
	public:
		Logger();
		~Logger();

		void Critical( Message msg );
		void Error( Message msg );
		void Warn( Message msg );
		void Info( Message msg );
		void Trace( Message msg );
		void AddMsg( Message msg );
	public:
		FileSink* fileSink;
	};

	class Log
	{
	public:
		template<typename T>
		static void log( const T& msg, Severity severity )
		{
			LogEx( msg, severity );
		}

		static void LogEx( const char& msg, Severity severity );
		static void LogEx( Message msg, Severity severity );
		static void LogEx( Message msg );
		static void LogEx( char* msg, Severity severity );

		static void EndLine();
	};
}
