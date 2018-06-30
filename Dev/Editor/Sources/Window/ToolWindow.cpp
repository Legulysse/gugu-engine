#include "Window/ToolWindow.h"

#include "Window/MainWindow.h"

#include "Gugu/Utility/System.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>


ToolWindow::ToolWindow(MainWindow* _pMainWindow, const std::string& _strName)
: QWidget(_pMainWindow)
, m_strName(_strName)
, m_pMainWindow(_pMainWindow)
, m_pParentTab(NULL)
, m_pParentWindow(NULL)
, m_pMenu(NULL)
, m_pActionAttach(NULL)
, m_pActionDetach(NULL)
{
}

ToolWindow::~ToolWindow()
{
}

void ToolWindow::NotifyCurrentTab(QWidget* _pTab)
{
    m_pParentTab = _pTab;
}

void ToolWindow::NotifyCurrentWindow(QWidget* _pWindow)
{
    m_pParentWindow = _pWindow;
}

void ToolWindow::OnAttach()
{
    m_pMainWindow->AttachToolWindow(this);
}

void ToolWindow::OnDetach()
{
    m_pMainWindow->DetachToolWindow(this);
}

void ToolWindow::PopulateMenu(QMenuBar* _pMenuBar)
{
    m_pMenu = new QMenu(m_strName.c_str());
    _pMenuBar->addMenu(m_pMenu);

    m_pActionAttach = new QAction(this);
    m_pActionAttach->setText("Attach");
    m_pMenu->addAction(m_pActionAttach);

    QObject::connect(m_pActionAttach, SIGNAL(triggered()), this, SLOT(OnAttach()));

    m_pActionDetach = new QAction(this);
    m_pActionDetach->setText("Detach");
    m_pMenu->addAction(m_pActionDetach);

    QObject::connect(m_pActionDetach, SIGNAL(triggered()), this, SLOT(OnDetach()));

    m_pMenu->addSeparator();

    if (m_pParentTab)
    {
        m_pActionAttach->setVisible(false);
        m_pActionDetach->setVisible(true);
    }
    else
    {
        m_pActionAttach->setVisible(true);
        m_pActionDetach->setVisible(false);
    }
}

std::string ToolWindow::GetName()
{
    return m_strName;
}

QWidget* ToolWindow::GetParentTab()
{
    return m_pParentTab;
}

QWidget* ToolWindow::GetParentWindow()
{
    return m_pParentWindow;
}
