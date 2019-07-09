#ifndef SIMPLE_MUSICAL_INITIALIZER_H
#define SIMPLE_MUSICAL_INITIALIZER_H
#include "simple/sdlcore/initializer.h"

// TODO: miniaudio backend https://github.com/dr-soft/miniaudio

namespace simple::musical
{

	class initializer : public sdlcore::initializer
	{
		public:
		initializer();
	};

} // namespace simple::musical

#endif /* end of include guard */
