#include "Window/MainWindow.h"

#include "Window/AnimationPanel.h"
#include "Window/ImageSetPanel.h"
#include "Dialog/DialogAbout.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Utility/DeltaTime.h"
#include "Gugu/Utility/System.h"

#include <QtCore/QTimer>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    //Engine Update
    timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateEngine()));
    timer->start(20);

    //Main UI
    setWindowTitle("Gugu::Editor");
    setWindowIcon(QIcon("Icons/application2.png"));
    //setWindowIcon(QIcon("Icons/smiley_cool.png"));

    resize(1024, 768);
    setMinimumSize(QSize(800, 600));

    m_pMenuBar = new QMenuBar(this);
    m_pMenuBar->setGeometry(QRect(0, 0, 1024, 20));
    setMenuBar(m_pMenuBar);

    QStatusBar* pStatusBar = new QStatusBar(this);
    pStatusBar->setObjectName(QString::fromUtf8("statusBar"));
    setStatusBar(pStatusBar);

    //Central Widget
    QWidget* pCentralWidget = new QWidget(this);
    setCentralWidget(pCentralWidget);

    QVBoxLayout* pCentralLayout = new QVBoxLayout(pCentralWidget);
    pCentralLayout->setSpacing(6);
    pCentralLayout->setContentsMargins(1, 1, 1, 1);
    pCentralWidget->setLayout(pCentralLayout);

    m_pCentralTabWidget = new QTabWidget(pCentralWidget);
    m_pCentralTabWidget->setTabShape(QTabWidget::Rounded);
    pCentralLayout->addWidget(m_pCentralTabWidget);
}

void MainWindow::InitTools()
{
    //Panels
    m_pImageSetPanel    = new ImageSetPanel(this, "ImageSets");
    m_pAnimationPanel   = new AnimationPanel(this, "AnimSets");

    AddToolWindow(m_pImageSetPanel);
    AddToolWindow(m_pAnimationPanel);

    m_pCentralTabWidget->setCurrentIndex(0);

    RefreshMenu();
}

MainWindow::~MainWindow()
{
    qDebug("Release MainWindow");

    //Delete Additional Windows if needed
    QWidget* pWindowImageSetPanel = m_pImageSetPanel->GetParentWindow();
    gugu::SafeDelete(pWindowImageSetPanel);
    QWidget* pWindowAnimationPanel = m_pAnimationPanel->GetParentWindow();
    gugu::SafeDelete(pWindowAnimationPanel);

    //Stop Engine Update
    timer->stop();
}

void MainWindow::AddToolWindow(ToolWindow* _pTool)
{
    AttachToolWindow(_pTool);
}

void MainWindow::AttachToolWindow(ToolWindow* _pTool)
{
    //Create Tab
    QWidget* pTab = new QWidget(this);

    QVBoxLayout* pTabLayout = new QVBoxLayout(pTab);
    pTabLayout->setSpacing(6);
    pTabLayout->setContentsMargins(3, 3, 3, 3);
    pTab->setLayout(pTabLayout);

    pTabLayout->addWidget(_pTool);

    m_pCentralTabWidget->addTab(pTab, _pTool->GetName().c_str());

    //RemoveWindow
    if (_pTool->GetParentWindow())
    {
        QWidget* pWindow = _pTool->GetParentWindow();
        _pTool->NotifyCurrentWindow(NULL);
        gugu::SafeDelete(pWindow);
    }

    //Finalize
    _pTool->NotifyCurrentTab(pTab);

    RefreshMenu();
}

void MainWindow::DetachToolWindow(ToolWindow* _pTool)
{
    //Create Window
    QWidget* pWindow = new QWidget(NULL, Qt::Widget);
    pWindow->setWindowTitle(_pTool->GetName().c_str());
    pWindow->setWindowIcon(QIcon("Icons/application2.png"));
    pWindow->resize(1024, 768);
    pWindow->setMinimumSize(QSize(800, 600));
    pWindow->show();

    QVBoxLayout* pCentralLayout = new QVBoxLayout(pWindow);
    pWindow->setLayout(pCentralLayout);

    QMenuBar* pMenuBar = new QMenuBar(pWindow);
    pMenuBar->setGeometry(QRect(0, 0, 1024, 20));
    pCentralLayout->setMenuBar(pMenuBar);

    pCentralLayout->addWidget(_pTool);

    QStatusBar* pStatusBar = new QStatusBar();
    pCentralLayout->addWidget(pStatusBar);

    //Remove Tab
    if (_pTool->GetParentTab())
    {
        int iIndex = m_pCentralTabWidget->indexOf(_pTool->GetParentTab());
        _pTool->NotifyCurrentTab(NULL);
        m_pCentralTabWidget->removeTab(iIndex);
    }

    //Finalize
    _pTool->NotifyCurrentWindow(pWindow);
    _pTool->PopulateMenu(pMenuBar);

    RefreshMenu();
}

void MainWindow::RefreshMenu()
{
    m_pMenuBar->clear();

    //Menu Editor
    QMenu* pMenuEditor = new QMenu(m_pMenuBar);
    pMenuEditor->setTitle("Editor");
    m_pMenuBar->addMenu(pMenuEditor);

    QAction* pActionSaveAll = new QAction(pMenuEditor);
    pActionSaveAll->setText("Save All");
    pMenuEditor->addAction(pActionSaveAll);

    QObject::connect(pActionSaveAll, SIGNAL(triggered()), this, SLOT(OnSaveAll()));

    pMenuEditor->addSeparator();

    QAction* pActionClose = new QAction(pMenuEditor);
    pActionClose->setText("Close");
    pMenuEditor->addAction(pActionClose);

    QObject::connect(pActionClose, SIGNAL(triggered()), this, SLOT(close()));

    //Tool Panels
    if (m_pImageSetPanel->GetParentTab())
        m_pImageSetPanel->PopulateMenu(m_pMenuBar);

    if (m_pAnimationPanel->GetParentTab())
        m_pAnimationPanel->PopulateMenu(m_pMenuBar);

    //Menu About
    QMenu* pMenuAbout = new QMenu(m_pMenuBar);
    pMenuAbout->setTitle("?");
    m_pMenuBar->addMenu(pMenuAbout);

    QAction* pActionAbout = new QAction(pMenuAbout);
    pActionAbout->setText("About...");
    pMenuAbout->addAction(pActionAbout);

    QObject::connect(pActionAbout, SIGNAL(triggered()), this, SLOT(OnOpenAbout()));
}

AnimationPanel* MainWindow::GetAnimationPanel()
{
    return m_pAnimationPanel;
}

ImageSetPanel* MainWindow::GetImageSetPanel()
{
    return m_pImageSetPanel;
}

void MainWindow::UpdateEngine()
{
    gugu::GetEngine()->Step(gugu::DeltaTime(timer->interval()));
}

void MainWindow::OnSaveAll()
{
    gugu::GetResources()->SaveAll();
}

void MainWindow::OnOpenAbout()
{
    DialogAbout oDialog;
    oDialog.exec();
}

