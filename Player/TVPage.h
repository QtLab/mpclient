#ifndef MP_TV_PAGE_WIDGET_H
#define MP_TV_PAGE_WIDGET_H

#include "QuickPageBase.h"

namespace mp {
namespace view {

class TVPage : public QuickPageBase
{
	Q_OBJECT

public:
	TVPage(QWidget* parent, QAbstractItemModel* categories
							,QAbstractItemModel* tvSourcesCurrentGenre
							,QAbstractItemModel* tvSourcesSearchResults
							,QAbstractItemModel* topTvSources);
	virtual ~TVPage();

	// User leave the page
	void Enter();

	// User leave the page
	bool Leave();

	// Languege changed
	void RetranslateUI();
	QString Name() const;
	QString ToolTip() const;

	bool Resizable() const;
	QSize RestoreSize() const;
	void SaveSize(const QSize& size);

	void SetCurrentGenre(int id, const QString& name);
	void SetCurrentCategory(int id, bool topVisible, int bannerId, const QString& bannerLogo);
	void ShowTVSource(const QString& url);

private slots:
	void CloseCurrentTVSource();
	void ContentLoaded(bool ok);

signals:
	void TVCurrentCategoryChanged(int);
	void ShowGenres(int id);
	void SearchFilterChanged(QString);
	void GotoTVSource(int id);

private:
	WebView *		m_webView;
	QSize			m_pageSize;
	QWebPage *		m_loadingWebPage;
	QWebPage *		m_contentWebPage;
};

} // end namespace view
} // end namespace mp

#endif