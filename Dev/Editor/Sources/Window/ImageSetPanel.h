#ifndef GUGU_IMAGESETPANEL_H
#define GUGU_IMAGESETPANEL_H


#include "Window/ToolWindow.h"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <map>


class QListWidgetItem;
class QTableWidgetItem;
class QListWidget;
class QTableWidget;
class QScrollBar;
class QComboBox;
class QLineEdit;
class QSlider;


namespace gugu {
    class Window;
    class ElementSprite;
    class QSFMLCanvas;
    class ImageSet;
    class SubImage;
}


class ImageSetPanel : public ToolWindow
{
    Q_OBJECT

public:

            ImageSetPanel       (MainWindow* _pMainWindow, const std::string& _strName);
    virtual ~ImageSetPanel      ();

    void OpenImageSetFromID     (const std::string& _strID, bool _bUpdateUI);
    void OpenImageSetFromItem   (QListWidgetItem* _pItem, bool _bUpdateUI);
    void SelectSubImage         (int _iIndex);

    void AddSubImage            (const std::string& _strName, bool _bSkipFirstIncrement, int _iLeft, int _iTop, int _iWidth, int _iHeight);

    void OnDragSelection        ();
    void OnDragEdge             (gugu::ElementSprite* _pSpriteEdge);

    virtual void PopulateMenu   (QMenuBar* _pMenuBar);

public slots:

    void OnImageSetChanged(QListWidgetItem*,QListWidgetItem*);
    void OnSubImageChanged(QTableWidgetItem*,QTableWidgetItem*);
    void OnTextureChanged(int _iIndex);
    void OnSubImageCellChanged(int _iRow, int _iColumn);

    void OnReloadTexture();
    void OnGenerateFromIndexes();
    void OnGenerateFromRect();
    void OnShrinkSelectionToPixels();
    void OnZoomSliderMoved(int);
    void OnHScrollSliderMoved(int);
    void OnVScrollSliderMoved(int);

    void OnAddRow();
    void OnRemoveRow();

    void OnNewImageSet();
    void OnRenameImageSet();
    void OnSaveImageSet();
    void OnDeleteImageSet();

private:

    void CreateGizmo();
    void RefreshImageSets();
    void RefreshSubImages();
    void RefreshRenderView();
    void OnRenderResized();

    void ComputePostDrag();

    gugu::SubImage* GetCurrentSubImage  ();
    gugu::SubImage* GetSubImage         (int _iRow);

    bool            RemoveSubImage      (int _iRow);
    void            RemoveSubImageImpl  (int _iRow);

private:

    gugu::Window*           m_pWindow;
    gugu::ElementSprite*    m_pSpriteBackground;
    gugu::ElementSprite*    m_pSpriteTexture;

    gugu::ElementSprite*    m_pSpriteSelection;
    gugu::ElementSprite*    m_pSpriteEdgeTopLeft;
    gugu::ElementSprite*    m_pSpriteEdgeTop;
    gugu::ElementSprite*    m_pSpriteEdgeTopRight;
    gugu::ElementSprite*    m_pSpriteEdgeRight;
    gugu::ElementSprite*    m_pSpriteEdgeBottomRight;
    gugu::ElementSprite*    m_pSpriteEdgeBottom;
    gugu::ElementSprite*    m_pSpriteEdgeBottomLeft;
    gugu::ElementSprite*    m_pSpriteEdgeLeft;

    gugu::QSFMLCanvas*  m_pRenderWidget;
    QListWidget*        m_pListImageSets;
    QTableWidget*       m_pListSubImages;
    QScrollBar*         m_pVerticalScrollBar;
    QScrollBar*         m_pHorizontalScrollBar;
    QSlider*            m_pSliderZoom;
    QComboBox*          m_pBoxTextures;

    QLineEdit* m_pEditGenerateIndexesX;
    QLineEdit* m_pEditGenerateIndexesY;
    QLineEdit* m_pEditGenerateRectWidth;
    QLineEdit* m_pEditGenerateRectHeight;
    QLineEdit* m_pEditGenerateRectOffsetX;
    QLineEdit* m_pEditGenerateRectOffsetY;

    int m_iZoomMin;         //min zoom level
    int m_iZoomMax;         //max zoom level
    int m_iZoomDefault;     //default
    float m_fRealZoomCoeff; //multiplier for real zoom level

    float           m_fZoomCurrent;     //Zoom multiplier
    int             m_iViewOffsetX;
    int             m_iViewOffsetY;
    sf::Vector2f    m_kViewCenter;

    gugu::ImageSet* m_pCurrentImageSet;

    //std::vector<gugu::SubImage*> m_vecGridIndexes;
    std::map<QTableWidgetItem*, gugu::SubImage*> m_mapSubImages;
    int m_iCurrentRow;
};


#endif // IMAGESETPANEL_H
