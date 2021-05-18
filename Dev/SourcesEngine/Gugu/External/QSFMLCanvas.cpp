#ifdef GUGU_QT

////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Render/QSFMLCanvas.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Action.h"
#include "Gugu/System/SystemUtility.h"

// Platform-specific headers
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

#include <QResizeEvent>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime)
: QWidget(Parent)
, myInitialized(false)
, m_callbackResize(nullptr)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the widget geometry
    move(Position);
    resize(Size);

    // Setup the timer
    myTimer.setInterval(FrameTime);

    m_window = nullptr;
}

QSFMLCanvas::~QSFMLCanvas()
{
    SafeDelete(m_callbackResize);
}

void QSFMLCanvas::SetWindow(gugu::Window* _pWindow)
{
    m_window = _pWindow;
}

void QSFMLCanvas::SetCallbackResize(Action* _pAction)
{
    SafeDelete(m_callbackResize);
    m_callbackResize = _pAction;
}

void QSFMLCanvas::OnInit()
{
    // Nothing to do by default...
}

void QSFMLCanvas::OnUpdate()
{
    // Nothing to do by default...
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;

        m_window-> ComputeSize(m_window->GetSize().x, m_window->GetSize().y);
    }
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    OnUpdate();

    //Note for Editor : For now, Render in Engine Step instead of here
    //if (m_window)
    //{
    //    m_window->Clear();
    //    m_window->Refresh(DeltaTime(20));   //TODO: clean this value
    //}
    //else
    //{
    //    display();
    //}
}

void QSFMLCanvas::resizeEvent(QResizeEvent* _pEvent)
{
    if (m_window)
        m_window->ComputeSize(_pEvent->size().width(), _pEvent->size().height());

    if (m_callbackResize)
        m_callbackResize->Call();
}

}   // namespace gugu

#endif
