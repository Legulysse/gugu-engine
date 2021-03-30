#ifndef GUGU_ANIMATIONPANEL_H
#define GUGU_ANIMATIONPANEL_H


#include "Window/ToolWindow.h"


class QListWidgetItem;
class QTableWidgetItem;
class QListWidget;
class QTableWidget;
class QScrollBar;
class QComboBox;
class QLineEdit;

namespace gugu {
    class Window;
    class ElementSprite;
    class ElementSpriteAnimated;
    class QSFMLCanvas;
    class ImageSet;
    class SubImage;
    class AnimSet;
    class Animation;
    class AnimationFrame;
}


class AnimationPanel : public ToolWindow
{
    Q_OBJECT

public:

            AnimationPanel      (MainWindow* _pMainWindow, const std::string& _strName);
    virtual ~AnimationPanel     ();

    void OpenAnimSetFromID      (const std::string& _strID, bool _bUpdateUI);
    void OpenAnimSetFromItem    (QListWidgetItem* _pItem, bool _bUpdateUI);
    void SelectAnimation        (const std::string& _strName, bool _bUpdateUI);
    void SelectFrame            (int _iFrameIndex, bool _bUpdateUI);

    void RefreshImageSets       ();

    void GetFramesUsingSubImage     (gugu::SubImage* _pSubImage, std::vector<gugu::AnimationFrame*>& _vecAnimFrames);
    void UpdateAnimationsImageSet   (gugu::ImageSet* _pImageSetFrom, gugu::ImageSet* _pImageSetTo);
    void PurgeSubImageInAnimSets    (gugu::SubImage* _pSubImage);

    virtual void PopulateMenu   (QMenuBar* _pMenuBar);

public slots:

    void OnNewAnimSet();
    void OnRenameAnimSet();
    void OnSaveAnimSet();
    void OnDeleteAnimSet();

private slots:

    void OnAnimSetChanged   (QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious);
    void OnAnimationChanged (QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious);
    void OnFrameChanged     (QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious);

    void OnAddAnimation     ();
    void OnRemoveAnimation  ();
    void OnRenameAnimation  ();
    void OnAddFrame         ();
    void OnRemoveFrame      ();

    void OnDefaultImageSetChanged   (int _iIndex);

    void OnFrameImageSetChanged     (int _iIndex);
    void OnFrameSubImageChanged     (const QString& _strSubImage);
    void OnFrameTextureChanged      (int _iIndex);
    void OnFrameDurationChanged     (const QString& _strDuration);

    void OnGenerateFromRange    ();
    void OnGenerateFromFolder   ();
    void OnApplyDefaultDuration ();

    void OnCheckAutoPlay        (int _iState);

private:

    void RefreshAnimSets    ();
    void RefreshAnimations  ();
    void RefreshSubImages   ();
    void RefreshFrames      ();
    void RefreshFrame       ();

private:

    gugu::Window*                   m_pWindow;
    gugu::ElementSpriteAnimated*    m_pSpriteAnimated;

    gugu::QSFMLCanvas*      m_pRenderWidget;
    QListWidget*            m_pListAnimSets;
    QListWidget*            m_pListAnimations;
    QListWidget*            m_pListFrames;
    QComboBox*              m_pBoxDefaultImageSets;
    QComboBox*              m_pBoxFrameImageSets;
    QComboBox*              m_pBoxFrameSubImages;
    QComboBox*              m_pBoxFrameTextures;
    QLineEdit*              m_pEditDefaultDuration;
    QLineEdit*              m_pEditFrameDuration;

    gugu::AnimationFrame*   m_pCurrentFrame;
    bool                    m_bAutoPlay;
};


#endif
