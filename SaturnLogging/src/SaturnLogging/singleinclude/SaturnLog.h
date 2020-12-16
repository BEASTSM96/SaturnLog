/*MIT License

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


#ifndef LOG_H
#define LOG_H


#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>


#pragma warning (disable : 26812) // The enum type type-name is unscoped. Prefer 'enum class' over 'enum' (Enum.3)

#define LOGNAMESPACE Saturn::Logging
#define NOEXCEPT noexcept
#define NORETURN [[ noreturn ]]


//https://stackoverflow.com/questions/1961209/making-some-text-in-printf-appear-in-green-and-red
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

namespace LOGNAMESPACE {
	using u8 = unsigned char;

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
		virtual void Flush() { };
		virtual void Print() { };
		virtual void Add( Message msg ) { };
	protected:
		int MsgAmt = 0;
		std::vector<Message> Msgs;
	};

	class FileSink : LogSink
	{
	public:
		virtual void Flush() override
		{
			Msgs.clear();
			MsgAmt = 0;
		}

		virtual void Print() override
		{
			std::ofstream file( "Log.log", std::ios::binary );

			for( int i = 0; i < Msgs.size(); i++ )
			{
				file << Msgs.at( i ) << '\n';
			}
		}

		virtual void Add( Message msg ) override
		{
			Msgs.push_back( msg );
			MsgAmt++;
		}
	};

	class Log
	{
	public:
		template<typename T>
		static void log( const T& msg, Severity severity )
		{
			LogEx( msg, severity );
		}

		static void LogEx( const char& msg, Severity severity )
		{


		}

		static void LogEx( Message msg )
		{
			printf( msg );
		}

		static void LogEx( Message msg, Severity severity )
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

		static void EndLine()
		{
			printf( "\n" );
		}
	};


	class Logger
	{
	public:
		Logger()
		{
			fileSink = new FileSink();
		}

		~Logger()
		{
			fileSink->Flush();
			delete fileSink;
		}

		void Critical( Message msg )
		{
			Error( msg );
		}

		void Error( Message msg )
		{
			Log::LogEx( RED );
			Log::LogEx( msg );
			Log::LogEx( RESET );
			Log::EndLine();

			AddMsg( msg );
		}

		void Warn( Message msg )
		{
			Log::LogEx( YELLOW );
			Log::LogEx( msg );
			Log::LogEx( RESET );
			Log::EndLine();

			AddMsg( msg );
		}

		void Info( Message msg )
		{
			Log::LogEx( GREEN );
			Log::LogEx( msg );
			Log::LogEx( RESET );
			Log::EndLine();

			AddMsg( msg );
		}

		void Trace( Message msg )
		{
			Log::LogEx( msg );
			Log::EndLine();

			AddMsg( msg );
		}

		void AddMsg( Message msg )
		{
			fileSink->Add( msg );
			fileSink->Print();
		}
	public:
		FileSink* fileSink;
	};

	class MessageCallback
	{
	public:
		virtual void OnMessageCallback( LOGNAMESPACE::Message msg ) { }

		virtual void OnMessageDeleted( LOGNAMESPACE::Message msg ) { }

		virtual void OnFlush() { }
	protected:
		Logger logger;
		std::vector<Message> Messages;
	};

	class Timer
	{
		using Time = std::chrono::steady_clock;
		using TimePoint = std::chrono::steady_clock::time_point;
		using Duration = std::chrono::duration<double>;
		using MilliSeconds = std::chrono::milliseconds;
		std::chrono::time_point<Time> Start_timer;
	public:
		Timer() : Start_timer{ Time::now() }
		{
		}

		Duration Elapsed() const 
		{
			return std::chrono::duration<double>(Start_timer - Time::now());
		}

		TimePoint GetTimeNow() 
		{
			return Time::now();
		}


		float fGetTimeNow() 
		{
			auto elapsed = std::chrono::duration<double>(Start_timer - Time::now());
			return std::chrono::duration_cast<MilliSeconds>(elapsed);
		}

		void Reset() 
		{
			Start_timer = Time::now();
		}

		~Timer() {}
	};

}


#endif