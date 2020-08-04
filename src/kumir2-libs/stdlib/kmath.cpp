#include "kumirstdlib.hpp"

#include <time.h>
#if !defined(APPLE) && !defined(USE_MINGW_TOOLCHAIN)
#include <random>
#endif

#include <math.h>
#include <float.h>

namespace Kumir
{

void Random::init()
{
#if !defined(WIN32)
	unsigned int seed = 0xDEADBEEF;
	FILE *urandom = fopen("/dev/urandom", "rb");
	fread(&seed, 1, sizeof(unsigned), urandom);
	fclose(urandom);
	srand(seed);
#elif defined(USE_MINGW_TOOLCHAIN)
	srand(time(NULL));
#endif
}

inline unsigned int Random::get_max()
{
#if !defined(WIN32) || defined(USE_MINGW_TOOLCHAIN)
	return RAND_MAX;
#else
	return std::random_device::max() - std::random_device::min();
#endif
}


unsigned int Random::get_sample()
{
#if !defined(WIN32) || defined(USE_MINGW_TOOLCHAIN)
	return rand();
#else
	std::random_device rd;
	return rd() - rd::min();
#endif
}


int Random::irand(int a, int b)
{
	if (a >= b) {
		if (a > b) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0;
		}
		return a;
	}

	unsigned int d = b - a + 1;
	unsigned int rd = get_max(), rq = rd / d + (rd % d + 1) / d;
	assert (0 < rq);

	for (;;) {
		unsigned int v = get_sample(), res = v / rq;
		if (res < d) {
			return a + res;
		}
	}
}

int Random::irnd(int x)
{
	return irand(1, x);
}


real Random::rrand(real a, real b)
{
	if (!(a < b)) {
		if (!(a <= b)) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0.0;
		}
		return a;
	}

	real d = b - a;
	if (!isfinite(d)) {
		Core::abort(Core::fromUtf8("Слишком широкий диапазон чисел"));
		return 0.0;
	}

	real rd = get_max() + 1.0;
	unsigned int v = get_sample();
	real res = v / rd * d;
	if (v < res) {
		res = v;
	}
	return res;
}

real Random::rrnd(real x)
{
	return rrand(0, x);
}


int System::time()
{
	int hours = 0, mins = 0, secs = 0, msecs = 0;

#if defined(WIN32) || defined(_WIN32)
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetSystemTime(&st);
	hours = st.wHour;
	mins  = st.wMinute;
	secs  = st.wSecond;
	msecs = st.wMilliseconds;
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	tzset();
	struct tm dummy;
	time_t epoch = ::time(NULL);
	struct tm *loc = localtime_r(&epoch, &dummy);
	hours = loc->tm_hour;
	mins  = loc->tm_min;
	secs  = loc->tm_sec;
	msecs = tv.tv_usec / 1000;
#endif

	int result = ((hours * 60 + mins) * 60 + secs) * 1000 + msecs;
	return result % 86400000;
}

}

