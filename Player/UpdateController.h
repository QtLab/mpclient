#ifndef MP_UPDATE_CONTROLLER_H
#define MP_UPDATE_CONTROLLER_H

#include "Prerequirements.h"
#include "UpdateModel.h"
#include "DownlaodManager.h"

namespace mp {

// takes a list of files in the format of the file name-file לה5
// compares the hashes with current files
// if the hash varies file pumped
// if a locale file no file is downloaded
// if the hash consists of zeros, file with the one you need to delete

class UpdateController : public QObject
{
	Q_OBJECT

public:
	UpdateController();

signals:
	void UpdateFinished(bool restartRequired);

public slots:
	bool InProcess() const;
	void Run(const QString& updateMd5);
	void ProcessUpdateList();
	void FileDownloaded();

private:
	// Last update md5
	QString							m_lastUpdateMD5;
	// PLayer MD5 
	QString							m_exeMD5;
	// Files to update
	UpdateModel						m_filesToUpdate;
	// Netwrok access manager
	DownlaodManager					m_networkAccess;
};

}

#endif
