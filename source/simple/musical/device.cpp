#include "device.h"
#include "simple/support/enum.hpp"
#include "simple/support/binary.hpp"

using simple::support::to_integer;
using simple::support::count_trailing_zeros;
using simple::support::count_ones;
using simple::support::bit_count;

namespace simple::musical
{

	template <typename P>
	P& common_device_parameters<P>::set_spec(const spec& desired, spec::changes allowed)
	{
		this->desired = desired;
		this->allowed = allowed;
		return static_cast<P&>(*this);
	}
	template basic_device_parameters& common_device_parameters<basic_device_parameters>::set_spec(const spec&, spec::changes);
	template device::parameters& common_device_parameters<device::parameters>::set_spec(const spec&, spec::changes);

	template <typename P>
	P& common_device_parameters<P>::set_samples_log2(uint_least8_t log2)
	{
		this->samples_log2 = log2;
		return static_cast<P&>(*this);
	}
	template basic_device_parameters& common_device_parameters<basic_device_parameters>::set_samples_log2(uint_least8_t);
	template device::parameters& common_device_parameters<device::parameters>::set_samples_log2(uint_least8_t);

	template <typename P>
	P& common_device_parameters<P>::set_name(const char* name)
	{
		this->name = name;
		return static_cast<P&>(*this);
	}
	template  basic_device_parameters& common_device_parameters<basic_device_parameters>::set_name(const char*);
	template  device::parameters& common_device_parameters<device::parameters>::set_name(const char*);

#if SDL_VERSION_ATLEAST(2,0,5)
	template<typename P>
	P& common_device_parameters<P>::set_capture(bool capture)
	{
		this->capture = capture;
		return static_cast<P&>(*this);
	}
	template basic_device_parameters& common_device_parameters<basic_device_parameters>::set_capture(bool);
	template device::parameters& common_device_parameters<device::parameters>::set_capture(bool);
#endif

	sdl_audio_device_handle::sdl_audio_device_handle(SDL_AudioDeviceID id) noexcept
		: id(id)
	{ }

	sdl_audio_device_handle::sdl_audio_device_handle(std::nullptr_t) noexcept
	{
		id = 0;
	}

	bool operator==(sdl_audio_device_handle one, sdl_audio_device_handle other) noexcept
	{
		return one.id == other.id;
	}

	bool operator!=(sdl_audio_device_handle one, sdl_audio_device_handle other) noexcept
	{
		return !(one == other);
	}

	// TODO: remove - not required by standard but emscriptenn 1.38.42 needs it -_-
	sdl_audio_device_handle::operator bool() noexcept
	{
		return (*this) != nullptr;
	}

	void sdl_audio_device_deleter::operator()(pointer handle) noexcept
	{
		SDL_CloseAudioDevice(handle.id);
	}

	device::parameters& device::parameters::set_callback(callback_type* callback, void* data)
	{
		this->callback = callback;
		this->data = data;
		return *this;
	}

	device::device(parameters params)
	: sdl_device_wrapper(sdl_audio_device_handle(SDL_OpenAudioDevice
	(
		params.name,
#if SDL_VERSION_ATLEAST(2,0,5)
		params.capture,
#else
		false,
#endif
		(
			params.desired.raw.samples = 1ull << params.samples_log2,
			params.desired.raw.callback = params.callback,
			params.desired.raw.userdata = params.data,
			&params.desired.raw
		),
		&_obtained.raw,
		to_integer(params.allowed)
	)))
	{
		assert(params.samples_log2 < bit_count(_obtained.raw.samples));
	}

	const spec& device::obtained() const noexcept
	{
		return _obtained;
	}

	constexpr int log2(int pow2)
	{
		assert((count_ones(pow2) == 1) && "Input must be a power of 2.");
		return count_trailing_zeros(pow2);
	}

	uint_least8_t device::samples_log2() const noexcept
	{
		return log2(_obtained.raw.samples);
	}

	auto device::samples() const noexcept
	{
		return _obtained.raw.samples;
	}

	uint8_t device::silence() const noexcept
	{
		return _obtained.raw.silence;
	}

	void device::lock() noexcept
	{
		SDL_LockAudioDevice(guts().get().id);
	}

	void device::unlock() noexcept
	{
		SDL_UnlockAudioDevice(guts().get().id);
	}

	void device::pause(bool value) noexcept
	{
		SDL_PauseAudioDevice(guts().get().id, value);
	}

	void device::play(bool value) noexcept
	{
		pause(!value);
	}

#if SDL_VERSION_ATLEAST(2,0,4)
	device_with_queue::device_with_queue(parameters params) :
		device
		({
			params.desired,
			params.samples_log2,
			params.allowed,
			params.name,
#if SDL_VERSION_ATLEAST(2,0,5)
			params.capture,
#endif
		})
	{ }

	bool device_with_queue::queue(const buffer_view<> buffer)
	{
		return sdlcore::utils::check_error(
			SDL_QueueAudio(guts().get().id, buffer.data, buffer.size));
	}

	uint32_t device_with_queue::queued()
	{
		return SDL_GetQueuedAudioSize(guts().get().id);
	}

	void device_with_queue::clear()
	{
		SDL_ClearQueuedAudio(guts().get().id);
	}

#endif

} // namespace simple::musical

