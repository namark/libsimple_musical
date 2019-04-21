#include "wav.h"

namespace simple::musical
{

	void wav_deleter::operator()(SDL_AudioSpec* spec) const noexcept
	{
		SDL_FreeWAV(static_cast<uint8_t*>(spec->userdata));
	}

	wav::wav(const char* filename)
		: wav_data{},
		spec_ptr(SDL_LoadWAV
		(
			filename,
			&_obtained.raw,
			((_obtained.raw.userdata = _buffer.data), &_buffer.data),
			&_buffer.size)
		)
	{ }

	const spec& wav::obtained() const noexcept
	{
		return _obtained;
	}

	const class buffer<>& wav::buffer() const noexcept
	{
		return _buffer;
	}

} // namespace simple::musical
