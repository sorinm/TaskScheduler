// The MIT License (MIT)
//
// 	Copyright (c) 2015 Sergey Makeev, Vadim Slyusarev
//
// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
// 	all copies or substantial portions of the Software.
//
// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// 	THE SOFTWARE.

#pragma once


#ifdef MT_INSTRUMENTED_BUILD


namespace MT
{

	class IProfilerEventListener
	{

	public:

		IProfilerEventListener() {};
		virtual ~IProfilerEventListener() {};

		// Called from worker thread context when worker thread created 
		virtual void OnThreadCreated(uint32 workerIndex) = 0;

		// Called from worker thread context when worker thread started
		virtual void OnThreadStarted(uint32 workerIndex) = 0;

		// Called from worker thread context when worker thread stopped
		virtual void OnThreadStoped(uint32 workerIndex) = 0;

		// Called from worker thread context when worker thread start to idle
		virtual void OnThreadIdleBegin(uint32 workerIndex) = 0;

		// Called from worker thread context when worker thread return to work
		virtual void OnThreadIdleEnd(uint32 workerIndex) = 0;

		// Called from the worker thread that has finished to execute the task
		virtual void OnTaskFinished(MT::Color::Type debugColor, const mt_char* debugID) = 0;

		// Called from the worker thread that has began to execute the task (new task or after old task was yielded)
		virtual void OnTaskResumed(MT::Color::Type debugColor, const mt_char* debugID) = 0;

		// Called from the worker thread that has yield the current task
		virtual void OnTaskYielded(MT::Color::Type debugColor, const mt_char* debugID) = 0;
	};

}


#endif
