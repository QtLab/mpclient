#ifndef MP_UPDATE_CONTROLLER_H
#define MP_UPDATE_CONTROLLER_H

#include "Prerequirements.h"
#include "UpdateModel.h"

namespace mp {
namespace controller {

// takes a list of files in the format of the file name-file ��5
// compares the hashes with current files
// if the hash varies file pumped
// if a locale file no file is downloaded
// if the hash consists of zeros, file with the one you need to delete

class UpdateController : public QObject
{
	Q_OBJECT

public:
	UpdateController();
	bool InProcess() const;
	bool IsActivatedByUser() const;

public slots:
	void CheckForUpdate(bool activatedByUser = false);

private slots:
	void ProcessUpdateList();
	void FileDownloaded(bool success, QVariant tag);

private:
	bool ProcessNextFile();
	void Cleanup();

signals:
	void UpdateFinished(bool success, bool restartRequired);

private:
	bool							m_activatedByUser;
	// Timer for update
	QTimer *						m_updateTimer;
	// Processing files count
	int								m_filesInProcess;
	// Newtwork access
	DownlaodManager *				m_downloadMgr;
	// Files to update
	model::UpdateModel				m_updateModel;
};

}
}

#endif
