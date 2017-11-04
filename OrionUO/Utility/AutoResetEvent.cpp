// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

AutoResetEvent::AutoResetEvent(bool initial)
: flag_(initial)
{
}

void AutoResetEvent::Set()
{
	std::lock_guard<std::mutex> _(protect_);
	flag_ = true;
	signal_.notify_one();
}

void AutoResetEvent::Reset()
{
	std::lock_guard<std::mutex> _(protect_);
	flag_ = false;
}

bool AutoResetEvent::WaitOne()
{
	std::unique_lock<std::mutex> lk(protect_);
	while (!flag_) // prevent spurious wakeups from doing harm
		signal_.wait(lk);
	flag_ = false; // waiting resets the flag
	return true;
}


AutoResetEvent event;

void otherthread()
{
	event.WaitOne();
	printf("Hello from other thread!\n");
}


int main()
{
	std::thread h(otherthread);
	printf("Hello from the first thread\n");
	event.Set();

	h.join();
}
