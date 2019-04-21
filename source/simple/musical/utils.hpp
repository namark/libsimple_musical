#ifndef SIMPLE_MUSICAL_UTILS_HPP
#define SIMPLE_MUSICAL_UTILS_HPP

#include <SDL2/SDL_endian.h>
#include <type_traits>

namespace simple::musical
{
	template <typename SizeType = uint32_t>
	struct buffer
	{
		using size_type = SizeType;
		uint8_t* data;
		size_type size;

		constexpr uint8_t* begin() noexcept { return data; }
		constexpr uint8_t* end() noexcept { return data + size; }
		constexpr const uint8_t* begin() const noexcept { return data; }
		constexpr const uint8_t* end() const noexcept{ return data + size; }
	};

	inline uint16_t endian_swap(uint16_t value) { return SDL_Swap16(value); }
	inline uint32_t endian_swap(uint32_t value) { return SDL_Swap32(value); }
	inline uint64_t endian_swap(uint64_t value) { return SDL_Swap64(value); }
	inline float    endian_swap(float value)    { return SDL_SwapFloat(value); }

	// from little endian to system endian
	inline uint16_t from_little_endian(uint16_t value) { return SDL_SwapLE16(value); }
	inline uint32_t from_little_endian(uint32_t value) { return SDL_SwapLE32(value); }
	inline uint64_t from_little_endian(uint64_t value) { return SDL_SwapLE64(value); }
	inline float    from_little_endian(float value)    { return SDL_SwapFloatLE(value); }

	// from big endian to system endian
	inline uint16_t from_big_endian(uint16_t value) { return SDL_SwapBE16(value); }
	inline uint32_t from_big_endian(uint32_t value) { return SDL_SwapBE32(value); }
	inline uint64_t from_big_endian(uint64_t value) { return SDL_SwapBE64(value); }
	inline float    from_big_endian(float value)    { return SDL_SwapFloatBE(value); }

	// from system endian to little endian
	inline uint16_t to_little_endian(uint16_t value) { return SDL_SwapLE16(value); }
	inline uint32_t to_little_endian(uint32_t value) { return SDL_SwapLE32(value); }
	inline uint64_t to_little_endian(uint64_t value) { return SDL_SwapLE64(value); }
	inline float    to_little_endian(float value)    { return SDL_SwapFloatLE(value); }

	// from system endian to big endian
	inline uint16_t to_big_endian(uint16_t value) { return SDL_SwapBE16(value); }
	inline uint32_t to_big_endian(uint32_t value) { return SDL_SwapBE32(value); }
	inline uint64_t to_big_endian(uint64_t value) { return SDL_SwapBE64(value); }
	inline float    to_big_endian(float value)    { return SDL_SwapFloatBE(value); }

	// yes "to_endian" is the same as "from_endian", just need to be careful what you pass through

} // namespace simple::musical

#endif /* end of include guard */
