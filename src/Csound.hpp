#include "rack.hpp"

#if defined _WIN32 || defined __CYGWIN__
	#ifdef __GNUC__
		#define DLL_EXPORT __attribute__ ((dllexport))
	#else
		#define DLL_EXPORT __declspec(dllexport)
	#endif
#endif

using namespace rack;

extern Plugin *plugin;
DLL_EXPORT void init(rack::Plugin *p);

extern Model *modelCsoundReverb;
