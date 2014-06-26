#ifndef FROD_PLUGIN_H
#define FROD_PLUGIN_H

#include <QtCore/qglobal.h>

extern "C" Q_DECL_EXPORT void StartPlugin();
extern "C" Q_DECL_EXPORT void StopPlugin();
extern "C" Q_DECL_EXPORT bool IsPluginRunning();

#endif