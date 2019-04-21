#include <cstdio>
#include <cerrno>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <atomic>

#include "simple/musical/initializer.h"
#include "simple/musical/device.h"
#include "simple/musical/wav.h"

using namespace simple::musical;
using namespace std::literals;

int main(int argc, const char** argv) try
{
	if(argc < 2)
	{
		std::puts("WAV file not specified");
		return -1;
	}

	initializer init;

	wav music(argv[1]);

	std::atomic<bool> done = false;

	device_with_callback beeper
	(
		basic_device_parameters{music.obtained()},
		[&done, &music, i = music.buffer().begin()](auto& device, auto buffer) mutable
		{
			const int remaining = music.buffer().end() - i;
			const int size = std::min<int>(remaining, buffer.size);
			const int extra = buffer.size - size;

			std::copy(i, i + size, buffer.begin());
			std::fill_n(buffer.begin() + size, extra, device.silence());

			i += size;
			if(i == music.buffer().end())
				done = true;
		}
	);

	beeper.play();

	while(!done)
		std::this_thread::sleep_for(50ms);

	return 0;
}
catch(...)
{
	if(errno)
		std::perror("ERROR");

	const char* sdl_error = SDL_GetError();
	if(*sdl_error)
		std::puts(sdl_error);

	throw;
}
