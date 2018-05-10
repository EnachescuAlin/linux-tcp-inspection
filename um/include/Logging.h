#ifndef TCP_INSPECTION_LOGGING
#define TCP_INSPECTION_LOGGING

#include <cstdio>

class Logging
{
public:
	Logging()
	{
		m_file = fopen("log.txt", "w");
	}

	~Logging()
	{
		if (m_file != NULL) {
			fclose(m_file);
			m_file = NULL;
		}
	}

	FILE* GetFile()
	{
		return m_file;
	}

	bool IsEnabled() const
	{
		return m_enabled && m_file != NULL;
	}

private:
	FILE *m_file;
	bool m_enabled = true;
};

#define LOG(format, ...)														\
	if (g_logging.IsEnabled()) {												\
		fprintf(g_logging.GetFile(), format "\n", __FUNCTION__, ##__VA_ARGS__);	\
		fflush(g_logging.GetFile());											\
	}

#define LOG_TRACE(format, ...)													\
	LOG("[TRACE] [%s] " format, ##__VA_ARGS__)

#define LOG_INFO(format, ...)													\
	LOG("[ INFO] [%s] " format, ##__VA_ARGS__)

#define LOG_WARN(format, ...)													\
	LOG("[ WARN] [%s] " format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...)													\
	LOG("[ERROR] [%s] " format, ##__VA_ARGS__)

#define LOG_FATAL(format, ...)													\
	LOG("[FATAL] [%s] " format, ##__VA_ARGS__)

#ifdef TCP_INSPECTION_LOGGING_MAIN
Logging g_logging;
#else
extern Logging g_logging;
#endif

#endif
