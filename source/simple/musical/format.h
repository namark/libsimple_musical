#ifndef SIMPLE_MUSICAL_FORMAT_H
#define SIMPLE_MUSICAL_FORMAT_H
#include <SDL2/SDL.h>

namespace simple::musical
{

	enum class format : SDL_AudioFormat
	{
		int8 = AUDIO_S8, // signed 8-bit samples
		uint8 = AUDIO_U8, // unsigned 8-bit samples
		int16_lsb = AUDIO_S16LSB, // signed 16-bit samples in little-endian byte order
		int16_msb = AUDIO_S16MSB, // signed 16-bit samples in big-endian byte order
		int16_sys = AUDIO_S16SYS, // signed 16-bit samples in native byte order
		int16 = AUDIO_S16, // AUDIO_S16LSB
		uint16_lsb = AUDIO_U16LSB, // unsigned 16-bit samples in little-endian byte order
		uint16_msb = AUDIO_U16MSB, // unsigned 16-bit samples in big-endian byte order
		uint16_sys = AUDIO_U16SYS, // unsigned 16-bit samples in native byte order
		uint16 = AUDIO_U16, // AUDIO_U16LSB
		int32_lsb = AUDIO_S32LSB, // 32-bit integer samples in little-endian byte order
		int32_msb = AUDIO_S32MSB, // 32-bit integer samples in big-endian byte order
		int32_sys = AUDIO_S32SYS, // 32-bit integer samples in native byte order
		int32 = AUDIO_S32, // AUDIO_S32LSB
		float32_lsb = AUDIO_F32LSB, // 32-bit floating point samples in little-endian byte order
		float32_msb = AUDIO_F32MSB, // 32-bit floating point samples in big-endian byte order
		float32_sys = AUDIO_F32SYS, // 32-bit floating point samples in native byte order
		float32 = AUDIO_F32, // AUDIO_F32LSB
	};

	bool is_float(format) noexcept;
	bool is_big_endian(format) noexcept;
	bool is_signed(format) noexcept;
	int bit_size(format) noexcept;

} // namespace simple::musical

#endif /* end of include guard */
