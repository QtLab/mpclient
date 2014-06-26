#include "PluginManager.h"
#include "Path.h"

#include <QLibrary>

namespace mp {

class Plugin
{
public:
	typedef void (*StartPlugin)();
	typedef void (*StopPlugin)();
	typedef bool (*IsPluginRunning)();

	Plugin(const QString& fileName)
		:m_library(fileName)
	{
		m_library.load();
	}

	~Plugin()
	{
		m_library.unload();
	}

	void Start()
	{
		if(m_library.isLoaded() || m_library.load())
		{
			StartPlugin startPlugin = (StartPlugin)m_library.resolve("StartPlugin");
			if(startPlugin)
			{
				startPlugin();
			}
		}
	}

	void Stop()
	{
		if(m_library.isLoaded() || m_library.load())
		{
			StopPlugin stopPlugin = (StopPlugin)m_library.resolve("StopPlugin");
			if(stopPlugin)
				stopPlugin();
		}
	}

	bool IsRunning()
	{
		if(m_library.isLoaded() || m_library.load())
		{
			IsPluginRunning isRunning = (IsPluginRunning)m_library.resolve("IsPluginRunning");

			if(isRunning)
				return isRunning();
		}

		return false;
	}

	bool IsLoaded() const
	{
		bool loaded = m_library.isLoaded();
		return loaded;
	}
private:
	QLibrary	m_library;
};

const QString StartupPluginName				= "startup";
const QString UserIdlePluginName			= "idle";
const QString UpdateCompletedPluginName		= "upcompleted";
const QString ShowtdownPluginName			= "showtdown";

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
	Cleanup();
}

void PluginManager::StartStartupPlugin()
{
	CreatePlugin(StartupPluginName)->Start();
}

void PluginManager::StartUserIdlePlugin()
{
	CreatePlugin(UserIdlePluginName)->Start();
}

void PluginManager::StopUserIdlePlugin()
{
	PluginPtr plugin = GetPlugin(UserIdlePluginName);

	if(plugin)
	{
		plugin->Stop();
	}
}

void PluginManager::StartUpdateCompletedPlugin()
{
	CreatePlugin(UserIdlePluginName)->Start();
}

void PluginManager::StartShowtdownPlugin()
{
	CreatePlugin(ShowtdownPluginName)->Start();
}

bool PluginManager::AnyLongRunningPluginStarted() const
{
	PluginPtr plugin = GetPlugin(UserIdlePluginName);

	if(plugin)
	{
		return plugin->IsRunning();
	}

	return false;
}

void PluginManager::UnloadAllPlugins()
{
	Cleanup();
}

PluginPtr PluginManager::CreatePlugin(const QString& pluginName)
{
	PluginPtr plugin = GetPlugin(pluginName);

	if(plugin == NULL)
	{
		plugin = PluginPtr(new Plugin(Path::PluginFile(pluginName)));

		if(plugin->IsLoaded())
		{
			m_pluginsMap.insert(pluginName, plugin);
		}
	}

	return plugin;
}

PluginPtr PluginManager::GetPlugin(const QString& pluginName) const
{
	PluginsMap::const_iterator i = m_pluginsMap.find(pluginName);

	if(i != m_pluginsMap.end())
	{
		return i.value();
	}

	return PluginPtr();
}

void PluginManager::Cleanup()
{
	m_pluginsMap.clear();
}

}