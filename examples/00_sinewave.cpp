#include <cstdio>
#include <cerrno>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>

#include "simple/musical/initializer.h"
#include "simple/musical/device.h"

using namespace simple::musical;
using namespace std::literals;

int main() try
{
	initializer init;

	device_with_callback beeper
	(
		basic_device_parameters
		{ spec{}
			.set_channels(spec::channels::mono)
			.set_format(format::int8)
		},
		[time = float()](auto&, auto buffer) mutable
		{
			std::generate(buffer.begin(), buffer.end(), [&]()
			{
				time += 0.05;
				return std::sin(time) * 127;
			});
		}
	);

	beeper.play();

	std::this_thread::sleep_for(500ms);
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
