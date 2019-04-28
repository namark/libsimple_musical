#ifndef SIMPLE_MUSICAL_WAV_H
#define SIMPLE_MUSICAL_WAV_H
#include "simple/sdlcore/utils.hpp"
#include "utils.hpp"
#include "spec.hpp"

namespace simple::musical
{

	struct wav_deleter
	{
		void operator()(SDL_AudioSpec*) const noexcept;
	};
	using spec_ptr = sdlcore::utils::object_wrapper<SDL_AudioSpec, wav_deleter>;

	struct wav_data
	{
		protected:
		spec _obtained;
		buffer<> _buffer;
	};

	class wav : wav_data, public spec_ptr
	{
		public:

		explicit wav(const char* filename);
		const spec& obtained() const noexcept;
		const struct buffer<>& buffer() const noexcept;
	};

} // namespace simple::musical

#endif /* end of include guard */
