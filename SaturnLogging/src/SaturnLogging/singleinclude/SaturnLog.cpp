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

#include "SaturnLog.h"
#include <iostream>
#include <fstream>

#pragma warning (disable : 26812) // The enum type type-name is unscoped. Prefer 'enum class' over 'enum' (Enum.3)

namespace Saturn::Loggging {

	void Log::LogEx( const char& msg, Severity severity )
	{
		std::cout << msg << std::endl;
	}

	void Log::LogEx( Message msg )
	{
		printf( msg );
	}

	void Log::LogEx( Message msg, Severity severity )
	{
		if( severity == Severity::Trace )
		{
			LogEx( msg );
			EndLine();
			return;
		}

		if( severity == Severity::Info )
		{
			LogEx( GREEN );
			LogEx( msg );
			LogEx( "\033[0m\t\t" );
			EndLine();
			return;
		}

		if( severity == Severity::Warn )
		{
			LogEx( YELLOW );
			LogEx( msg );
			LogEx( "\033[0m\t\t" );
			EndLine();
			return;
		}

		if( severity == Severity::Error )
		{
			LogEx( RED );
			LogEx( msg );
			LogEx( "\033[0m\t\t" );
			EndLine();
			return;
		}

		if( severity == Severity::Critical )
		{
			LogEx( RED );
			LogEx( msg );
			LogEx( "\033[0m\t\t" );
			EndLine();
			return;
		}
	}

	void Log::EndLine()
	{
		printf( "\n" );
	}

	Logger::Logger()
	{
		fileSink = new FileSink();
	}

	Logger::~Logger()
	{
		fileSink->Flush();
		delete fileSink;
	}

	void Logger::Critical( Message msg )
	{
		Error( msg );
	}

	void Logger::Error( Message msg )
	{
		Log::LogEx( RED );
		Log::LogEx( msg );
		Log::LogEx( RESET );
		Log::EndLine();

		AddMsg( msg );
	}

	void Logger::Warn( Message msg )
	{
		Log::LogEx( YELLOW );
		Log::LogEx( msg );
		Log::LogEx( RESET );
		Log::EndLine();

		AddMsg( msg );
	}

	void Logger::Info( Message msg )
	{
		Log::LogEx( GREEN );
		Log::LogEx( msg );
		Log::LogEx( RESET );
		Log::EndLine();

		AddMsg( msg );
	}

	void Logger::Trace( Message msg )
	{
		Log::LogEx( msg );
		Log::EndLine();

		AddMsg( msg );
	}

	void Logger::AddMsg( Message msg )
	{
		fileSink->Add( msg );
		fileSink->Print();
	}

	void FileSink::Flush()
	{
		Msgs.clear();
		MsgAmt = 0;
	}

	void FileSink::Print()
	{
		std::ofstream file( "Log.log", std::ios::binary );

		for( int i = 0; i < Msgs.size(); i++ )
		{
			file << Msgs.at( i ) << '\n';
		}

	}

	void FileSink::Add( Message msg )
	{
		Msgs.push_back( msg );
		MsgAmt++;
	}

}
