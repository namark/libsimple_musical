#include "format.h"

namespace simple::musical
{

	bool is_float(format f) noexcept
	{
		const auto sdlf = static_cast<SDL_AudioFormat>(f);
		return SDL_AUDIO_ISFLOAT(sdlf);
	}

	bool is_big_endian(format f) noexcept
	{
		const auto sdlf = static_cast<SDL_AudioFormat>(f);
		return SDL_AUDIO_ISBIGENDIAN(sdlf);
	}

	bool is_signed(format f) noexcept
	{
		const auto sdlf = static_cast<SDL_AudioFormat>(f);
		return SDL_AUDIO_ISSIGNED(sdlf);
	}

	int bit_size(format f) noexcept
	{
		const auto sdlf = static_cast<SDL_AudioFormat>(f);
		return SDL_AUDIO_BITSIZE(sdlf);
	}

} // namespace simple::musical
