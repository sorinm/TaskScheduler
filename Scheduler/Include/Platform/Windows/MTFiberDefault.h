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

#ifndef __MT_FIBER_DEFAULT__
#define __MT_FIBER_DEFAULT__

#include "MTAtomic.h"

namespace MT
{

	//
	// Fibers implementation using system fibers
	// Beware! Windows Fibers are wasteful use of Virtual Memory space for the stack. ( 1Mb reserved for each Fiber )
	//
	class Fiber
	{
		void* funcData;
		TThreadEntryPoint func;

		void* fiber;

		static void __stdcall FiberFuncInternal(void* pFiber)
		{
			Fiber* self = (Fiber*)pFiber;
			self->func(self->funcData);
		}

	public:

		MT_NOCOPYABLE(Fiber);

		Fiber()
			: fiber(nullptr)
		{
		}

		~Fiber()
		{
			if (fiber)
			{
				if (func)
				{
					DeleteFiber(fiber);
				}
				fiber = nullptr;
			}
		}


		void CreateFromCurrentThreadAndRun(Thread & thread, TThreadEntryPoint entryPoint, void *userData)
		{
			MT_USED_IN_ASSERT(thread);
			MT_ASSERT(fiber == nullptr, "Fiber already created");
			MT_ASSERT(thread.IsCurrentThread(), "Can't create fiber from this thread");

			func = nullptr;
			funcData = nullptr;

			fiber = ::ConvertThreadToFiberEx(nullptr, MW_FIBER_FLAG_FLOAT_SWITCH);
			MT_ASSERT(fiber != nullptr, "Can't create fiber");

			entryPoint(userData);
		}


		void Create(size_t stackSize, TThreadEntryPoint entryPoint, void* userData)
		{
			MT_ASSERT(fiber == nullptr, "Fiber already created");

			func = entryPoint;
			funcData = userData;
			fiber = ::CreateFiber( stackSize, FiberFuncInternal, this );
			MT_ASSERT(fiber != nullptr, "Can't create fiber");
		}

		static void SwitchTo(Fiber & from, Fiber & to)
		{
			MT_USED_IN_ASSERT(from);

			HardwareFullMemoryBarrier();

			MT_ASSERT(from.fiber != nullptr, "Invalid from fiber");
			MT_ASSERT(to.fiber != nullptr, "Invalid to fiber");

			::SwitchToFiber( (void*)to.fiber );
		}


	};

}

#endif
