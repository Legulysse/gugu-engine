#ifndef GUGU_TOOLWINDOW_H
#define GUGU_TOOLWINDOW_H


#include <QtWidgets/QWidget>


class QMenu;
class QMenuBar;
class QAction;

class MainWindow;


class ToolWindow : public QWidget
{
    Q_OBJECT

public:

            ToolWindow  (MainWindow* _pMainWindow, const std::string& _strName);
    virtual ~ToolWindow ();

    void NotifyCurrentTab       (QWidget* _pTab);
    void NotifyCurrentWindow    (QWidget* _pWindow);

    virtual void PopulateMenu   (QMenuBar* _pMenuBar);

    std::string GetName();

    QWidget* GetParentTab();
    QWidget* GetParentWindow();

private slots:

    void OnAttach();
    void OnDetach();

protected:

    std::string m_strName;
    MainWindow* m_pMainWindow;
    QWidget*    m_pParentTab;
    QWidget*    m_pParentWindow;
    QMenu*      m_pMenu;
    QAction*    m_pActionAttach;
    QAction*    m_pActionDetach;
};


#endif // GUGU_TOOLWINDOW_H
