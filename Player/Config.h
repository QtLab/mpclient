#ifndef MP_CONFIG_H
#define MP_CONFIG_H

#include <QString>
#include <QFont>
#include <QSize>
#include <QSettings>

namespace mp {

class Config : public QObject
{
	Q_OBJECT

public:
	static Config& Inst();

	QString UserId() const;
	QString Source() const;

	QString Version() const;
	void SetVersion(const QString& version);

	const QFont& DefaultFont() const;

	void SetVolume(qreal value, const QString& streamName);
	qreal Volume(const QString& streamName) const;

	QSize TabSize(const QString& name, const QSize& defaultSize = QSize(580, 351)) const;
	void SetTabSize(const QString& name, const QSize& size);

	QString PathToSaveTracks() const;
	void SetPathToSaveTracks(const QString& path);

signals:
	void VolumeChanged(qreal value, const QString& streamName);

private:
	void Load();

private:
	Config();
	static Config*		m_instance;

	QSettings			m_settings;
	QFont				m_defaultFont;
};

}

#endif