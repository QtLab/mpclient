#ifndef MP_CONFIG_H
#define MP_CONFIG_H

#include <QString>

namespace mp {

class Config 
{
public:
	static Config& Inst();

	const QString& UserId() const;
	const QString& Source() const;

private:
	void Load();

private:
	Config();
	static Config*		m_instance;

	QString				m_userId;
	QString				m_source;
};

}

#endif