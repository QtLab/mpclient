#ifndef MP_PLUGINS_MANAGER_H
#define MP_PLUGINS_MANAGER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>

namespace mp {

class Plugin;
typedef QSharedPointer<Plugin> PluginPtr;

class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager();
	~PluginManager();

	void StartStartupPlugin();

	void StartUserIdlePlugin();
	void StopUserIdlePlugin();

	void StartUpdateCompletedPlugin();
	
	void StartShowtdownPlugin();

	bool AnyLongRunningPluginStarted() const;
	void UnloadAllPlugins();
	
private:
	PluginPtr CreatePlugin(const QString& pluginName);
	PluginPtr GetPlugin(const QString& pluginName) const;
	void Cleanup();	

private:
	typedef			QMap<QString, PluginPtr>	PluginsMap;
	PluginsMap		m_pluginsMap;
};

}

#endif