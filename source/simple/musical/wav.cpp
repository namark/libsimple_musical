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
			&_buffer.data,
			&_buffer.size
		))
	{ guts()->userdata = _buffer.data; }

	wav::wav(const byte* memory, int size)
		: wav_data{},
		spec_ptr(SDL_LoadWAV_RW
		(
			SDL_RWFromConstMem(memory, size),
			1,
			&_obtained.raw,
			&_buffer.data,
			&_buffer.size
		))
	{ guts()->userdata = _buffer.data; }


	const spec& wav::obtained() const noexcept
	{
		return _obtained;
	}

	const buffer_view<>& wav::buffer() const noexcept
	{
		return _buffer;
	}

} // namespace simple::musical
