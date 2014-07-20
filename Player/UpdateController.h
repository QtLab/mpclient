#ifndef MP_UPDATE_CONTROLLER_H
#define MP_UPDATE_CONTROLLER_H

#include "Prerequirements.h"
#include "UpdateModel.h"

namespace mp {
namespace controller {

class UpdateResult
{
public:
	UpdateResult(bool success, bool	restartRequired, bool activatedByUser)
		:m_success(success)
		,m_restartRequired(restartRequired)
		,m_activatedByUser(activatedByUser)
	{
	}

	bool Success() const { return m_success; }
	bool RestartRequired() const { return m_restartRequired; }
	bool ActivatedByUser() const { return m_activatedByUser; }

private:
	bool	m_success;
	bool	m_restartRequired;
	bool	m_activatedByUser;
};

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
	bool InProcess() const;

public slots:
	void CheckForUpdate(bool activatedByUser = false);

private slots:
	void ProcessUpdateList();
	void FileDownloaded(bool success, QVariant tag);

private:
	bool ProcessNextFile();
	void Cleanup();

signals:
	void UpdateFinished(const UpdateResult&);

private:
	bool							m_activatedByUser;
	bool							m_aborted;
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
