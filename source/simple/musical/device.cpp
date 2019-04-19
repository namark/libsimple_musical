#include "device.h"
#include "simple/support/enum.hpp"

using simple::support::to_integer;

namespace simple::musical
{

	template <typename P>
	P& common_device_parameters<P>::set_spec(const spec& desired, spec::changes allowed)
	{
		this->desired = desired;
		this->allowed = allowed;
		return static_cast<P&>(*this);
	}

	template <typename P>
	P& common_device_parameters<P>::set_name(const char* name)
	{
		this->name = name;
		return static_cast<P&>(*this);
	}

#if SDL_VERSION_ATLEAST(2,0,5)
	template<typename P>
	P& common_device_parameters<P>set_capture(bool capture)
	{
		this->capture = name;
		return static_cast<P&>(*this);
	}
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
			params.desired.raw.callback = params.callback,
			params.desired.raw.userdata = params.data,
			&params.desired.raw
		),
		&_obtained.raw,
		to_integer(params.allowed)
	)))
	{ }

	const spec& device::obtained() const noexcept
	{
		return _obtained;
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

#if SDL_VERSION_ATLEAST(2,0,5)
	class device_with_queue : public device
	{
		public:
		struct parameters : public common_device_parameters<parameters>
		{ };

		void queue(const uint8_t * data, int len);
	};
	device_with_queue::device_with_queue(parameters params) :
		device
		({
			params.desired,
			params.allowed,
			params.name,
#if SDL_VERSION_ATLEAST(2,0,5)
			params.capture,
#endif
		})
	{ }
#endif

} // namespace simple::musical

