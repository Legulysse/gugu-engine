#ifndef GUGU_MAINWINDOW_H
#define GUGU_MAINWINDOW_H


#include <QtWidgets/QMainWindow>


class QTabWidget;
class QMenuBar;

class ToolWindow;
class AnimationPanel;
class ImageSetPanel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

            MainWindow  (QWidget *parent = 0);
    virtual ~MainWindow ();

    void InitTools          ();
    void AddToolWindow      (ToolWindow* _pTool);
    void AttachToolWindow   (ToolWindow* _pTool);
    void DetachToolWindow   (ToolWindow* _pTool);

    void RefreshMenu        ();

    AnimationPanel* GetAnimationPanel();
    ImageSetPanel*  GetImageSetPanel();

private slots:

    void UpdateEngine();

    void OnSaveAll();
    void OnOpenAbout();

private:

    QTimer* timer;

    AnimationPanel* m_pAnimationPanel;
    ImageSetPanel*  m_pImageSetPanel;

    QTabWidget* m_pCentralTabWidget;
    QMenuBar*   m_pMenuBar;
};


#endif
