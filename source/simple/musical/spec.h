#ifndef SIMPLE_MUSICAL_SPEC_H
#define SIMPLE_MUSICAL_SPEC_H
#include <SDL2/SDL.h>

#include "simple/support/enum_flags_operators.hpp"

#include "format.h"

namespace simple::musical
{

	class spec
	{
		public:
		enum class changes : int
		{
			none,
			frequency = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
			format = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
			channel = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
			any = SDL_AUDIO_ALLOW_ANY_CHANGE
		};

		enum class channels : uint8_t
		{
			mono = 1,
			stereo = 2,
			quad = 4,
			surround_5_1 = 6
		};

		struct frequency { enum : int
		{
			telephone = 8'000,
			telephone_x2 = 16'000,
			wideband_telephone = telephone_x2,

			cd_quarter = 11'025,
			cd_half = 22'050,
			cd = 44'100,
			cs_x2 = cd * 2,
			cd_x4 = cd * 4,
			cd_x8 = cd * 8,
			cd_x64 = cd * 64,
			dsd = cd_x64,
			dsd_x2 = dsd * 2,
			dsd_x4 = dsd * 4,
			dsd_x8 = dsd * 8,

			hdcd = cd_x4,
			DXD = cd_x8,
			sacd = cd_x64,

			miniDV = 32'000,

			cd_xa = 37'800,

			ntsc_locked = 44'056,

			denon = 47'250,

			standard = 48'000,
			standard_x2 = standard * 2,
			standard_x4 = standard * 4,

			dvd = standard_x2,
			dvd_x2 = standard_x4,

			soundstream_first = 50'000,

			mitsubishi_x_80 = 50'400,

			_64k = 64'000,
			some_hardware = _64k,
			pioneer_sx_s30dab = _64k,
			hammerfall_dsp_9632 = _64k,
		}; };

		spec(int frequency = frequency::standard, format = format::int16, channels = channels::stereo, size_t samples_log2 = 10);

		int get_frequency() const;
		spec& set_frequency(int);

		format get_format() const;
		spec& set_format(format);

		channels get_channels() const;
		spec& set_channels(channels);

		size_t get_samples_log2() const;
		spec& set_samples_log2(size_t);

		auto get_samples() const;

		private:
		SDL_AudioSpec raw;
		friend class device;
	};

} // namespace simple::musical

template<> struct simple::support::define_enum_flags_operators<simple::musical::spec::changes>
	: std::true_type {};

#endif /* end of include guard */
