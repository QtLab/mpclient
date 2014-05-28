#ifndef MP_PLUGINS_MANAGER_H
#define MP_PLUGINS_MANAGER_H

#include <QObject>
#include <QMap>

namespace mp {

class Plugin;
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
	Plugin * CreatePlugin(const QString& pluginName);
	Plugin * GetPlugin(const QString& pluginName) const;
	void Cleanup();	

private:
	typedef			QMap<QString, Plugin *>	PluginsMap;
	PluginsMap		m_pluginsMap;
};

}

#endif