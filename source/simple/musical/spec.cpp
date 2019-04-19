#include <climits>
#include <cassert>
#include <bitset>
#include <type_traits>

#include "spec.h"
#include "simple/support/int_literals.hpp"
#include "simple/support/enum.hpp"
#include "simple/support/binary.hpp"
#include "simple/support/int_literals.hpp"


namespace simple::musical
{
	using namespace support;
	using namespace literals;

	spec::spec(int frequency, format format, channels channels, size_t samples_log2)
		: raw{frequency, to_integer(format), to_integer(channels), 0,
			static_cast<decltype(raw.samples)>(1u << samples_log2)}
	{
		assert(samples_log2 < bit_count(raw.samples));
	}

	int spec::get_frequency() const
	{
		return raw.freq;
	}
	spec& spec::set_frequency(int frequency)
	{
		raw.freq = frequency;
		return *this;
	}

	format spec::get_format() const
	{
		return static_cast<format>(raw.format);
	}
	spec& spec::set_format(format format)
	{
		raw.format = to_integer(format);
		return *this;
	}

	spec::channels spec::get_channels() const
	{
		return static_cast<channels>(raw.channels);
	}
	spec& spec::set_channels(channels channels)
	{
		raw.channels = to_integer(channels);
		return *this;
	}

	constexpr int log2(int pow2)
	{
		assert((count_ones(pow2) == 1) && "Input must be a power of 2.");
		return count_trailing_zeros(pow2);
	}

	size_t spec::get_samples_log2() const
	{
		return log2(raw.samples);
	}

	spec& spec::set_samples_log2(size_t samples_log2)
	{
		raw.samples = static_cast<decltype(raw.samples)>(1u << samples_log2);
		return *this;
	}

	auto spec::get_samples() const
	{
		return raw.samples;
	}

} // namespace simple::musical
