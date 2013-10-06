#ifndef MP_UPDATE_CONTROLLER_H
#define MP_UPDATE_CONTROLLER_H

#include "UpdateModel.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace mp {

// takes a list of files in the format of the file name-file לה5
// compares the hashes with current files
// if the hash varies file pumped
// if a locale file no file is downloaded
// if the hash consists of zeros, file with the one you need to delete

class UpdateControler : public QObject
{
	Q_OBJECT

public:
	static UpdateControler& Inst();

signals:
	void UpdateFinished(bool restartRequired);

public slots:
	void Run(const QString& updateMd5);
	void ProcessUpdateReply(QNetworkReply* reply);
	void ProcessUpdateFileReply(QNetworkReply* reply);

private:
	UpdateControler();

	static UpdateControler *		m_instance;

	// Last update md5
	QString							m_lastUpdateMD5;
	// PLayer MD5 
	QString							m_exeMD5;
	// Dlownlaoded files count
	int								m_downloaded;
	// Update already in process
	bool							m_inprocess	;
	// Files to update
	UpdateModel						m_filesToUpdate;
};

}

#endif
