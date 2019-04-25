#include <ncine/AppConfiguration.h>

inline void setDataPath(ncine::AppConfiguration &config)
{
#ifdef __ANDROID__
	const char *extStorage = getenv("EXTERNAL_STORAGE");
	nctl::String dataPath;
	dataPath = extStorage ? extStorage : "/sdcard";
	dataPath += "/ncine/";
	config.dataPath() = dataPath;
#else
	#ifdef NCINE_TESTS_DATA_DIR
	config.dataPath() = NCINE_TESTS_DATA_DIR;
	#endif
#endif
}
