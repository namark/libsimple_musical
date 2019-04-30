#ifndef SIMPLE_MUSICAL_DEVICE_H
#define SIMPLE_MUSICAL_DEVICE_H
#include <functional>

#include "simple/sdlcore/utils.hpp"
#include "simple/support/enum_flags_operators.hpp"

#include "utils.hpp"
#include "spec.hpp"

namespace simple::musical
{
	// hmmm, all these "just so"s below make me think i would have been better off using composition,
	// but then the sdl object wrapper would need to be exposed or befriended by all that use it.

	template <typename Parameters>
	struct common_device_parameters
	{
		spec desired;
		uint_least8_t samples_log2 = 10;
		spec::changes allowed = spec::changes::none;
		const char* name = nullptr;

		Parameters& set_spec(const spec& desired, spec::changes allowed = spec::changes::none);
		Parameters& set_samples_log2(uint_least8_t log2);
		Parameters& set_name(const char* name);

#if SDL_VERSION_ATLEAST(2,0,5)
		bool capture = false;
		Parameters& set_capture(bool);
#endif
	};

	struct basic_device_parameters : public common_device_parameters<basic_device_parameters>
	{ };


	class sdl_audio_device_handle
	{
		SDL_AudioDeviceID id;
		sdl_audio_device_handle(SDL_AudioDeviceID) noexcept;
		friend class device;
		friend class device_with_queue;
		friend class sdl_audio_device_deleter;
		public:

		sdl_audio_device_handle(std::nullptr_t = nullptr) noexcept;
		friend bool operator==(sdl_audio_device_handle, sdl_audio_device_handle) noexcept;
		friend bool operator!=(sdl_audio_device_handle, sdl_audio_device_handle) noexcept;
	};

	class sdl_audio_device_deleter
	{
		public:
		using pointer = sdl_audio_device_handle;
		void operator()(pointer handle) noexcept;
	};

	struct spec_holder // just so spec can be initialized before device wrapper
	{
		spec _obtained;
	};

	using sdl_device_wrapper = sdlcore::utils::object_wrapper<sdl_audio_device_handle, sdl_audio_device_deleter>;

	class device : spec_holder, public sdl_device_wrapper
	{
		public:

		struct parameters : public common_device_parameters<parameters>
		{
			using callback_type = void(void*, uint8_t* stream, int);
			callback_type* callback = nullptr;
			void* data = nullptr;

			parameters& set_callback(callback_type* callback, void* data = nullptr);
		};

		device(parameters params);

		const spec& obtained() const noexcept;
		uint_least8_t samples_log2() const noexcept;
		auto samples() const noexcept;  // -> some integer type
		uint8_t silence() const noexcept;

		void lock() noexcept;
		void unlock() noexcept;
		void pause(bool = true) noexcept;
		void play(bool = true) noexcept;
	};

	template <typename Callback>
	struct callback_holder // just so device is destroyed before callback
	{
		Callback callback;
	};


	// hmmm, if construct the callback first, then if device throws can't move it back,
	// but if construct it second, it will be destroyed before device which is using it
	//
	// construct second and pause device in destructor?
	// construct first then try/catch and move back?
	template <typename Callback>
	class device_with_callback : callback_holder<Callback>, public device
	{
		static void callback_wrapper(void* data, uint8_t* buf, int len)
		{
			device_with_callback& this_device = *static_cast<device_with_callback*>(data);
			std::invoke(this_device.callback, this_device, buffer_view<int>{buf, len});
		}

		public:
		using parameters = basic_device_parameters;

		template <typename C>
		device_with_callback(parameters params, C&& callback) :
			callback_holder<Callback>{std::forward<C>(callback)},
			device(device::parameters
			{
				params.desired,
				params.samples_log2,
				params.allowed,
				params.name,
#if SDL_VERSION_ATLEAST(2,0,5)
				params.capture,
#endif
				&callback_wrapper,
				this
			})
		{}
	};

	template <typename C>
	device_with_callback(basic_device_parameters, C&&) -> device_with_callback<C>;

#if SDL_VERSION_ATLEAST(2,0,4)
	class device_with_queue : public device
	{
		public:
		using parameters = basic_device_parameters;
		device_with_queue(parameters params);
		bool queue(const buffer_view<>);
		uint32_t queued();
		void clear();
	};
#endif

} // namespace simple::musical

#endif /* end of include guard */
