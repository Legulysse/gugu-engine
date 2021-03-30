#include "Window/ImageSetPanel.h"

#include "Window/MainWindow.h"
#include "Window/AnimationPanel.h"
#include "Dialog/DialogRenameResource.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Render/QSFMLCanvas.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Utility/Action.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/AnimSet.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>


ImageSetPanel::ImageSetPanel(MainWindow* _pMainWindow, const std::string& _strName)
: ToolWindow(_pMainWindow, _strName)
, m_pWindow(NULL)
, m_pCurrentImageSet(NULL)
{
    m_iZoomMin = 2;
    m_iZoomMax = 50;
    m_iZoomDefault = 10;
    m_fRealZoomCoeff = 1.f / (float)m_iZoomDefault;

    m_fZoomCurrent = 1.f;
    m_iViewOffsetX = 0;
    m_iViewOffsetY = 0;

    m_iCurrentRow = -1;

    //Top Layout
    QHBoxLayout* pLayoutTop = new QHBoxLayout(this);
    pLayoutTop->setContentsMargins(0, 0, 0, 0);
    //setLayout(pLayoutTop);

    //List ImageSets
    m_pListImageSets = new QListWidget(this);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(m_pListImageSets->sizePolicy().hasHeightForWidth());
    m_pListImageSets->setSizePolicy(sizePolicy);
    m_pListImageSets->setMaximumSize(QSize(220, 16777215));
    pLayoutTop->addWidget(m_pListImageSets);

    //Right side Layout
    QWidget* pWidgetContent = new QWidget;
    pWidgetContent->setFixedWidth(300);
    QVBoxLayout* pLayoutContent = new QVBoxLayout(pWidgetContent);
    pLayoutContent->setContentsMargins(0, 0, 0, 0);
    pLayoutTop->addWidget(pWidgetContent);
    //pLayoutTop->addLayout(pLayoutContent);

    //Images
    {
        QHBoxLayout* pLayoutImages = new QHBoxLayout();
        pLayoutContent->addLayout(pLayoutImages);

        m_pBoxTextures = new QComboBox;
        m_pBoxTextures->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        pLayoutImages->addWidget(m_pBoxTextures);

        std::vector<const gugu::ResourceInfo*> vecInfos;
        gugu::GetResources()->GetLoadedTextureInfos(vecInfos);

        m_pBoxTextures->addItem("", QVariant::fromValue(QString("")));
        for (size_t i = 0; i < vecInfos.size(); ++i)
        {
            m_pBoxTextures->addItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()), QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));
        }

        QPushButton* pButtonReload = new QPushButton("Reload");
        pLayoutImages->addWidget(pButtonReload);

        connect(m_pBoxTextures, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTextureChanged(int)));
        connect(pButtonReload, SIGNAL(clicked()), this, SLOT(OnReloadTexture()));
    }

    //Automatic generator
    {
        //From Indexes
        {
            QHBoxLayout* pLayoutGenerateFromIndexes = new QHBoxLayout();
            pLayoutContent->addLayout(pLayoutGenerateFromIndexes);

            QLabel* pLabelIndexesX = new QLabel;
            pLabelIndexesX->setText("Indexes X");
            pLayoutGenerateFromIndexes->addWidget(pLabelIndexesX);

            m_pEditGenerateIndexesX = new QLineEdit();
            m_pEditGenerateIndexesX->setFixedWidth(30);
            pLayoutGenerateFromIndexes->addWidget(m_pEditGenerateIndexesX);

            QLabel* pLabelIndexesY = new QLabel;
            pLabelIndexesY->setText("Indexes Y");
            pLayoutGenerateFromIndexes->addWidget(pLabelIndexesY);

            m_pEditGenerateIndexesY = new QLineEdit();
            m_pEditGenerateIndexesY->setFixedWidth(30);
            pLayoutGenerateFromIndexes->addWidget(m_pEditGenerateIndexesY);

            QPushButton* pButtonGenerate = new QPushButton("Generate from indexes");
            pLayoutGenerateFromIndexes->addWidget(pButtonGenerate);

            pLayoutGenerateFromIndexes->addStretch();

            connect(pButtonGenerate, SIGNAL(clicked()), this, SLOT(OnGenerateFromIndexes()));
        }

        //From Rect
        {
            QHBoxLayout* pLayoutGenerateFromRect = new QHBoxLayout();
            pLayoutContent->addLayout(pLayoutGenerateFromRect);

            QLabel* pLabelWidth = new QLabel;
            pLabelWidth->setText("W");
            pLayoutGenerateFromRect->addWidget(pLabelWidth);

            m_pEditGenerateRectWidth = new QLineEdit();
            m_pEditGenerateRectWidth->setFixedWidth(30);
            pLayoutGenerateFromRect->addWidget(m_pEditGenerateRectWidth);

            QLabel* pLabelHeight = new QLabel;
            pLabelHeight->setText("H");
            pLayoutGenerateFromRect->addWidget(pLabelHeight);

            m_pEditGenerateRectHeight = new QLineEdit();
            m_pEditGenerateRectHeight->setFixedWidth(30);
            pLayoutGenerateFromRect->addWidget(m_pEditGenerateRectHeight);

            QLabel* pLabelOffsetX = new QLabel;
            pLabelOffsetX->setText("X");
            pLayoutGenerateFromRect->addWidget(pLabelOffsetX);

            m_pEditGenerateRectOffsetX = new QLineEdit();
            m_pEditGenerateRectOffsetX->setFixedWidth(30);
            pLayoutGenerateFromRect->addWidget(m_pEditGenerateRectOffsetX);

            QLabel* pLabelOffsetY = new QLabel;
            pLabelOffsetY->setText("Y");
            pLayoutGenerateFromRect->addWidget(pLabelOffsetY);

            m_pEditGenerateRectOffsetY = new QLineEdit();
            m_pEditGenerateRectOffsetY->setFixedWidth(30);
            pLayoutGenerateFromRect->addWidget(m_pEditGenerateRectOffsetY);

            QPushButton* pButtonGenerate = new QPushButton("Generate from rect");
            pLayoutGenerateFromRect->addWidget(pButtonGenerate);

            pLayoutGenerateFromRect->addStretch();

            connect(pButtonGenerate, SIGNAL(clicked()), this, SLOT(OnGenerateFromRect()));
        }
    }

    //List Edition
    {
        QHBoxLayout* pLayoutEditionSet = new QHBoxLayout();
        pLayoutContent->addLayout(pLayoutEditionSet);

        QPushButton* pButtonAddRow = new QPushButton("Add Row");
        pButtonAddRow->setIcon(QIcon("Icons/add.png"));
        pLayoutEditionSet->addWidget(pButtonAddRow);

        QPushButton* pButtonRemoveRow = new QPushButton("Remove Row(s)");
        pButtonRemoveRow->setIcon(QIcon("Icons/cross.png"));
        pLayoutEditionSet->addWidget(pButtonRemoveRow);

        pLayoutEditionSet->addStretch();

        connect(pButtonAddRow, SIGNAL(clicked()), this, SLOT(OnAddRow()));
        connect(pButtonRemoveRow, SIGNAL(clicked()), this, SLOT(OnRemoveRow()));
    }

    //List SubImages
    {
        m_pListSubImages = new QTableWidget();
        m_pListSubImages->setColumnCount(5);
        //m_pListSubImages->setMaximumSize(QSize(280, 16777215));
        m_pListSubImages->horizontalHeader()->setDefaultSectionSize(50);
        //m_pListSubImages->verticalHeader()->setDefaultSectionSize(50);

        pLayoutContent->addWidget(m_pListSubImages);

        QTableWidgetItem* pTableWidgetItem1 = new QTableWidgetItem("Name");
        QTableWidgetItem* pTableWidgetItem2 = new QTableWidgetItem("X");
        QTableWidgetItem* pTableWidgetItem3 = new QTableWidgetItem("Y");
        QTableWidgetItem* pTableWidgetItem4 = new QTableWidgetItem("W");
        QTableWidgetItem* pTableWidgetItem5 = new QTableWidgetItem("H");
        m_pListSubImages->setHorizontalHeaderItem(0, pTableWidgetItem1);
        m_pListSubImages->setHorizontalHeaderItem(1, pTableWidgetItem2);
        m_pListSubImages->setHorizontalHeaderItem(2, pTableWidgetItem3);
        m_pListSubImages->setHorizontalHeaderItem(3, pTableWidgetItem4);
        m_pListSubImages->setHorizontalHeaderItem(4, pTableWidgetItem5);
        m_pListSubImages->setColumnWidth(0, 106);
        m_pListSubImages->setColumnWidth(1, 36);
        m_pListSubImages->setColumnWidth(2, 36);
        m_pListSubImages->setColumnWidth(3, 36);
        m_pListSubImages->setColumnWidth(4, 36);
        //m_pListSubImages->setSortingEnabled(true);
    }

    //Edition Layout
    QVBoxLayout* pLayoutViewer = new QVBoxLayout();
    pLayoutTop->addLayout(pLayoutViewer);

    //Edition Tools
    {
        QHBoxLayout* pLayoutEditionFrame = new QHBoxLayout();
        pLayoutViewer->addLayout(pLayoutEditionFrame);

        QToolButton* pButtonShrinkToPixels = new QToolButton();
        pButtonShrinkToPixels->setToolTip("Shrink Selection To Visible Pixels");
        pButtonShrinkToPixels->setIcon(QIcon("Icons/arrow_in.png"));
        pButtonShrinkToPixels->setAutoRaise(true);
        pLayoutEditionFrame->addWidget(pButtonShrinkToPixels);

        m_pSliderZoom = new QSlider();
        m_pSliderZoom->setToolTip("Zoom");
        m_pSliderZoom->setObjectName(QString::fromUtf8("horizontalSlider"));
        m_pSliderZoom->setOrientation(Qt::Horizontal);
        m_pSliderZoom->setRange(m_iZoomMin, m_iZoomMax);
        m_pSliderZoom->setSliderPosition(m_iZoomDefault);
        m_pSliderZoom->setSingleStep(1);
        m_pSliderZoom->setMinimumWidth(200);
        m_pSliderZoom->setMaximumWidth(200);
        m_pSliderZoom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLayoutEditionFrame->addWidget(m_pSliderZoom);

        //pLayoutEditionFrame->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pLayoutEditionFrame->addStretch();

        connect(pButtonShrinkToPixels   , SIGNAL(clicked())                     , this, SLOT(OnShrinkSelectionToPixels()));
        connect(m_pSliderZoom           , SIGNAL(valueChanged(int))              , this, SLOT(OnZoomSliderMoved(int)));
    }

    //Render Canvas + Scroll
    {
        QGridLayout* gridLayout = new QGridLayout();
        pLayoutViewer->addLayout(gridLayout);

        m_pHorizontalScrollBar = new QScrollBar();
        m_pHorizontalScrollBar->setOrientation(Qt::Horizontal);
        m_pHorizontalScrollBar->setRange(0, 0);
        m_pHorizontalScrollBar->setSingleStep(1);
        gridLayout->addWidget(m_pHorizontalScrollBar, 1, 0);

        m_pVerticalScrollBar = new QScrollBar();
        m_pVerticalScrollBar->setOrientation(Qt::Vertical);
        m_pVerticalScrollBar->setRange(0, 0);
        m_pVerticalScrollBar->setSingleStep(1);
        gridLayout->addWidget(m_pVerticalScrollBar, 0, 1);

        m_pRenderWidget = new gugu::QSFMLCanvas(this, QPoint(0, 0), QSize(400, 400));
        gridLayout->addWidget(m_pRenderWidget, 0, 0);

        gugu::EngineConfig oConfig;
        oConfig.backgroundColor = sf::Color(128,128,128,255);
        gugu::Window* pWindow = new gugu::Window;
        pWindow->Init(m_pRenderWidget, oConfig);
        pWindow->SetRenderer(gugu::GetEngine()->GetDefaultRenderer());
        m_pRenderWidget->SetWindow(pWindow);
        gugu::GetEngine()->AddWindow(pWindow);

        gugu::GetEngine()->SetGameWindow(pWindow);

        m_pWindow = pWindow;

        m_pWindow->GetMainCamera()->SetCenterOnTarget(true);
        m_kViewCenter = m_pWindow->GetMainCamera()->GetTarget();

        connect(m_pHorizontalScrollBar  , SIGNAL(valueChanged(int))              , this, SLOT(OnHScrollSliderMoved(int)));
        connect(m_pVerticalScrollBar    , SIGNAL(valueChanged(int))              , this, SLOT(OnVScrollSliderMoved(int)));

        m_pRenderWidget->SetCallbackResize(new gugu::ActionClass<ImageSetPanel>(this, &ImageSetPanel::OnRenderResized));
    }


    //Background
    sf::Image oImgBackground;
    oImgBackground.create(2, 2, sf::Color(160, 160, 160));

    gugu::Texture* pTxrBackground = gugu::GetResources()->GetCustomTexture("BackgroundTexture");
    pTxrBackground->GetSFTexture()->create(2, 2);
    pTxrBackground->GetSFTexture()->update(oImgBackground);

    m_pSpriteBackground = m_pWindow->GetUINode()->AddChild<gugu::ElementSprite>();
    m_pSpriteBackground->SetTexture(pTxrBackground);
    m_pSpriteBackground->SetVisible(false);

    m_pSpriteTexture = m_pWindow->GetUINode()->AddChild<gugu::ElementSprite>();
    m_pSpriteTexture->SetVisible(false);

    //Gizmo
    CreateGizmo();

    //Callbacks
    connect(m_pListImageSets        , SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))     , this, SLOT(OnImageSetChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(m_pListSubImages        , SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*))   , this, SLOT(OnSubImageChanged(QTableWidgetItem*,QTableWidgetItem*)));
    connect(m_pListSubImages        , SIGNAL(cellChanged(int, int))         , this, SLOT(OnSubImageCellChanged(int, int)));

    //Get Resources
    RefreshImageSets();
}

ImageSetPanel::~ImageSetPanel()
{
    qDebug("Release ImageSetPanel");

    gugu::GetEngine()->RemoveWindow(m_pWindow);
    //gugu::SafeDelete(m_pWindow);
}

void ImageSetPanel::PopulateMenu(QMenuBar* _pMenuBar)
{
    ToolWindow::PopulateMenu(_pMenuBar);

    //Menu
    QAction* pActionImageSetNew = new QAction(this);
    pActionImageSetNew->setText("New ImageSet");
    m_pMenu->addAction(pActionImageSetNew);

    QObject::connect(pActionImageSetNew, SIGNAL(triggered()), this, SLOT(OnNewImageSet()));

    QAction* pActionImageSetRename = new QAction(this);
    pActionImageSetRename->setText("Rename Current");
    m_pMenu->addAction(pActionImageSetRename);

    QObject::connect(pActionImageSetRename, SIGNAL(triggered()), this, SLOT(OnRenameImageSet()));

    QAction* pActionImageSetSave = new QAction(this);
    pActionImageSetSave->setText("Save Current");
    m_pMenu->addAction(pActionImageSetSave);

    QObject::connect(pActionImageSetSave, SIGNAL(triggered()), this, SLOT(OnSaveImageSet()));

    m_pMenu->addSeparator();

    QAction* pActionImageSetDelete = new QAction(this);
    pActionImageSetDelete->setText("Delete Current");
    m_pMenu->addAction(pActionImageSetDelete);

    QObject::connect(pActionImageSetDelete, SIGNAL(triggered()), this, SLOT(OnDeleteImageSet()));
}

void ImageSetPanel::CreateGizmo()
{
    sf::Image oImgSelection;
    oImgSelection.create(2, 2, sf::Color(255, 0, 255, 100));

    sf::Image oImgSelectionEdge;
    oImgSelectionEdge.create(2, 2, sf::Color(255, 0, 0, 255));

    gugu::Texture* pTxrSelection = gugu::GetResources()->GetCustomTexture("Selection");
    pTxrSelection->GetSFTexture()->create(2,2);
    pTxrSelection->GetSFTexture()->update(oImgSelection);

    gugu::Texture* pTxrSelectionEdge = gugu::GetResources()->GetCustomTexture("SelectionEdge");
    pTxrSelectionEdge->GetSFTexture()->create(2,2);
    pTxrSelectionEdge->GetSFTexture()->update(oImgSelectionEdge);

    m_pSpriteSelection = m_pWindow->GetUINode()->AddChild<gugu::ElementSprite>();
    m_pSpriteSelection->SetTexture(pTxrSelection);
    m_pSpriteSelection->SetVisible(false);
    m_pSpriteSelection->SetZIndex(100);

    m_pSpriteSelection->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteSelection->InitInteractions();
    m_pSpriteSelection->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass<ImageSetPanel>(this, &ImageSetPanel::OnDragSelection));

    sf::Vector2f kSizeEdge(6.f, 6.f);

    m_pSpriteEdgeTopLeft = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeTopLeft->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeTopLeft->SetUnifiedOrigin(gugu::UDim2(1.f, 0.f, 1.f, 0.f));
    m_pSpriteEdgeTopLeft->SetUnifiedPosition(gugu::UDim2(0.f, 0.f, 0.f, 0.f));
    m_pSpriteEdgeTopLeft->SetSize(kSizeEdge);

    m_pSpriteEdgeTop = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeTop->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeTop->SetUnifiedOrigin(gugu::UDim2(.5f, 0.f, 1.f, 8.f));
    m_pSpriteEdgeTop->SetUnifiedPosition(gugu::UDim2(0.5f, 0.f, 0.f, 0.f));
    m_pSpriteEdgeTop->SetSize(kSizeEdge);

    m_pSpriteEdgeTopRight = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeTopRight->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeTopRight->SetUnifiedOrigin(gugu::UDim2(0.f, 0.f, 1.f, 0.f));
    m_pSpriteEdgeTopRight->SetUnifiedPosition(gugu::UDim2(1.f, 0.f, 0.f, 0.f));
    m_pSpriteEdgeTopRight->SetSize(kSizeEdge);

    m_pSpriteEdgeRight = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeRight->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeRight->SetUnifiedOrigin(gugu::UDim2(0.f, -8.f, .5f, 0.f));
    m_pSpriteEdgeRight->SetUnifiedPosition(gugu::UDim2(1.f, 0.f, 0.5f, 0.f));
    m_pSpriteEdgeRight->SetSize(kSizeEdge);

    m_pSpriteEdgeBottomRight = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeBottomRight->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeBottomRight->SetUnifiedOrigin(gugu::UDim2(0.f, 0.f, 0.f, 0.f));
    m_pSpriteEdgeBottomRight->SetUnifiedPosition(gugu::UDim2(1.f, 0.f, 1.f, 0.f));
    m_pSpriteEdgeBottomRight->SetSize(kSizeEdge);

    m_pSpriteEdgeBottom = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeBottom->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeBottom->SetUnifiedOrigin(gugu::UDim2(0.5f, 0.f, 0.f, -8.f));
    m_pSpriteEdgeBottom->SetUnifiedPosition(gugu::UDim2(.5f, 0.f, 1.f, 0.f));
    m_pSpriteEdgeBottom->SetSize(kSizeEdge);

    m_pSpriteEdgeBottomLeft = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeBottomLeft->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeBottomLeft->SetUnifiedOrigin(gugu::UDim2(1.f, 0.f, 0.f, 0.f));
    m_pSpriteEdgeBottomLeft->SetUnifiedPosition(gugu::UDim2(0.f, 0.f, 1.f, 0.f));
    m_pSpriteEdgeBottomLeft->SetSize(kSizeEdge);

    m_pSpriteEdgeLeft = m_pSpriteSelection->AddChild<gugu::ElementSprite>();
    m_pSpriteEdgeLeft->SetTexture(pTxrSelectionEdge);
    m_pSpriteEdgeLeft->SetUnifiedOrigin(gugu::UDim2(1.f, 8.f, 0.5f, 0.f));
    m_pSpriteEdgeLeft->SetUnifiedPosition(gugu::UDim2(0.f, 0.f, .5f, 0.f));
    m_pSpriteEdgeLeft->SetSize(kSizeEdge);

    m_pSpriteEdgeTopLeft->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeTop->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeTopRight->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeRight->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeBottomRight->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeBottom->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeBottomLeft->AddInteractionFlag(gugu::EInteraction::Drag);
    m_pSpriteEdgeLeft->AddInteractionFlag(gugu::EInteraction::Drag);

    m_pSpriteEdgeTopLeft->InitInteractions();
    m_pSpriteEdgeTop->InitInteractions();
    m_pSpriteEdgeTopRight->InitInteractions();
    m_pSpriteEdgeRight->InitInteractions();
    m_pSpriteEdgeBottomRight->InitInteractions();
    m_pSpriteEdgeBottom->InitInteractions();
    m_pSpriteEdgeBottomLeft->InitInteractions();
    m_pSpriteEdgeLeft->InitInteractions();

    m_pSpriteEdgeTopLeft->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeTopLeft));
    m_pSpriteEdgeTop->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeTop));
    m_pSpriteEdgeTopRight->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeTopRight));
    m_pSpriteEdgeRight->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeRight));
    m_pSpriteEdgeBottomRight->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeBottomRight));
    m_pSpriteEdgeBottom->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeBottom));
    m_pSpriteEdgeBottomLeft->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeBottomLeft));
    m_pSpriteEdgeLeft->GetInteractions()->AddCallback(gugu::EInteraction::Drag, new gugu::ActionClass1P<ImageSetPanel, gugu::ElementSprite*>
                                                            (this, &ImageSetPanel::OnDragEdge, m_pSpriteEdgeLeft));
}

void ImageSetPanel::RefreshImageSets()
{
    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetLoadedImageSetInfos(vecInfos);

    m_pListImageSets->blockSignals(true);
    m_pListImageSets->clear();
    for (size_t i = 0; i < vecInfos.size(); ++i)
    {
        QListWidgetItem* pItem = new QListWidgetItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()));
        pItem->setToolTip(QString::fromStdString(vecInfos[i]->fileInfo.GetPathName()));
        pItem->setData(Qt::UserRole, QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));

        m_pListImageSets->addItem(pItem);
    }
    m_pListImageSets->blockSignals(false);
}

void ImageSetPanel::OpenImageSetFromID(const std::string& _strID, bool _bUpdateUI)
{
    QString qID = QString::fromStdString(_strID);
    QListWidgetItem* pItem = NULL;

    for (int i = 0; i < m_pListImageSets->count(); ++i)
    {
        if (m_pListImageSets->item(i)->data(Qt::UserRole).toString() == qID)
        {
            pItem = m_pListImageSets->item(i);
            break;
        }
    }

    OpenImageSetFromItem(pItem, _bUpdateUI);
}

void ImageSetPanel::OpenImageSetFromItem(QListWidgetItem* _pItem, bool _bUpdateUI)
{
    std::string strResourceID = "";

    if (_pItem)
    {
        strResourceID = _pItem->data(Qt::UserRole).toString().toStdString();
    }

    if (_bUpdateUI)
    {
        m_pListImageSets->blockSignals(true);
        m_pListImageSets->setCurrentItem(_pItem);
        m_pListImageSets->blockSignals(false);
    }

    m_pSpriteBackground->SetVisible(false);
    m_pSpriteSelection->SetVisible(false);
    m_pSpriteTexture->SetVisible(false);

    m_pHorizontalScrollBar->setRange(0, 0);
    m_pVerticalScrollBar->setRange(0, 0);

    m_pBoxTextures->blockSignals(true);
    m_pBoxTextures->setCurrentIndex(0);
    m_pBoxTextures->blockSignals(false);

    m_pSliderZoom->setSliderPosition(m_iZoomDefault);
    m_pHorizontalScrollBar->setSliderPosition(0);
    m_pVerticalScrollBar->setSliderPosition(0);

    m_pCurrentImageSet = gugu::GetResources()->GetImageSet(strResourceID);

    RefreshSubImages();

    if (m_pCurrentImageSet)
    {
        if (m_pCurrentImageSet->GetTexture())
        {
            m_pSpriteTexture->SetVisible(true);
            m_pSpriteTexture->SetTexture(m_pCurrentImageSet->GetTexture());

            m_pSpriteBackground->SetVisible(true);
            m_pSpriteBackground->SetSize(m_pSpriteTexture->GetSize());

            m_pBoxTextures->blockSignals(true);
            QString qstrID = QString::fromStdString(m_pCurrentImageSet->GetTexture()->GetID());
            int iIndex = m_pBoxTextures->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);
            m_pBoxTextures->setCurrentIndex(iIndex);
            m_pBoxTextures->blockSignals(false);
        }

        SelectSubImage(-1);

        RefreshRenderView();
    }
}

void ImageSetPanel::RefreshSubImages()
{
    m_pListSubImages->blockSignals(true);
    m_pListSubImages->setRowCount(0);

    m_mapSubImages.clear();

    if (m_pCurrentImageSet)
    {
        m_pListSubImages->setRowCount(m_pCurrentImageSet->GetSubImageCount());

        for (size_t i = 0; i < m_pCurrentImageSet->GetSubImageCount(); ++i)
        {
            gugu::SubImage* pSubImage = m_pCurrentImageSet->GetSubImage(i);
            sf::IntRect oSFRect = pSubImage->GetRect();

            m_pListSubImages->setItem(i, 0, new QTableWidgetItem( QString::fromStdString(pSubImage->GetName()) ));
            m_pListSubImages->setItem(i, 1, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.left)) ));
            m_pListSubImages->setItem(i, 2, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.top)) ));
            m_pListSubImages->setItem(i, 3, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.width)) ));
            m_pListSubImages->setItem(i, 4, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.height)) ));

            m_mapSubImages[m_pListSubImages->item(i, 0)] = pSubImage;
        }
        m_pListSubImages->resizeRowsToContents();
    }

    m_pListSubImages->blockSignals(false);
}

void ImageSetPanel::SelectSubImage(int _iIndex)
{
    m_iCurrentRow = -1;
    m_pSpriteSelection->SetVisible(false);

    if (m_pListSubImages->rowCount() > 0 && _iIndex >= 0 && _iIndex < m_pListSubImages->rowCount())
    {
        int iLeft   = gugu::FromString<int>(m_pListSubImages->item(_iIndex,1)->text().toStdString());
        int iTop    = gugu::FromString<int>(m_pListSubImages->item(_iIndex,2)->text().toStdString());
        int iWidth  = gugu::FromString<int>(m_pListSubImages->item(_iIndex,3)->text().toStdString());
        int iHeight = gugu::FromString<int>(m_pListSubImages->item(_iIndex,4)->text().toStdString());

        m_pSpriteSelection->SetVisible(true);
        m_pSpriteSelection->SetPosition(iLeft, iTop);
        m_pSpriteSelection->SetSize(iWidth, iHeight);

        m_iCurrentRow = _iIndex;
    }
    else
    {
        m_pListSubImages->blockSignals(true);
        m_pListSubImages->setCurrentCell(-1,-1);
        m_pListSubImages->blockSignals(false);
        m_pListSubImages->clearSelection();
    }
}

void ImageSetPanel::OnImageSetChanged(QListWidgetItem* pCurrent, QListWidgetItem* pPrevious)
{
    if (pCurrent)
        OpenImageSetFromItem(pCurrent, false);
}

void ImageSetPanel::OnSubImageChanged(QTableWidgetItem* pCurrent, QTableWidgetItem* pPrevious)
{
    if (pCurrent)
        SelectSubImage(pCurrent->row());
}

void ImageSetPanel::OnSubImageCellChanged(int _iRow, int _iColumn)
{
    //qDebug(m_pListSubImages->item(_iRow,_iColumn)->text().toStdString().c_str());

    //TODO: check the new value is a number
    //if ()
    {
        gugu::SubImage* pEditedSubImage = GetCurrentSubImage();
        if (pEditedSubImage)
        {
            std::string strName = m_pListSubImages->item(_iRow,0)->text().toStdString();
            int iLeft   = gugu::FromString<int>(m_pListSubImages->item(_iRow,1)->text().toStdString());
            int iTop    = gugu::FromString<int>(m_pListSubImages->item(_iRow,2)->text().toStdString());
            int iWidth  = gugu::FromString<int>(m_pListSubImages->item(_iRow,3)->text().toStdString());
            int iHeight = gugu::FromString<int>(m_pListSubImages->item(_iRow,4)->text().toStdString());

            pEditedSubImage->SetName(strName);
            pEditedSubImage->SetRect(sf::IntRect(iLeft, iTop, iWidth, iHeight));

            if (m_iCurrentRow == _iRow)
            {
                m_pSpriteSelection->SetVisible(true);
                m_pSpriteSelection->SetPosition(iLeft, iTop);
                m_pSpriteSelection->SetSize(iWidth, iHeight);
            }
        }
    }
}

void ImageSetPanel::OnTextureChanged(int _iIndex)
{
    if (!m_pCurrentImageSet)
        return;

    std::string strID = m_pBoxTextures->itemData(_iIndex).toString().toStdString();

    gugu::Texture* pTexture = gugu::GetResources()->GetTexture(strID);
    if (pTexture)
    {
        m_pCurrentImageSet->SetTexture(pTexture);

        m_pSpriteTexture->SetVisible(true);
        m_pSpriteTexture->SetTexture(m_pCurrentImageSet->GetTexture());

        m_pSpriteBackground->SetVisible(true);
        m_pSpriteBackground->SetSize(m_pSpriteTexture->GetSize());

        SelectSubImage(-1);

        RefreshRenderView();
    }
}

void ImageSetPanel::OnReloadTexture()
{
    if (!m_pSpriteTexture)
        return;

    gugu::Texture* pTexture = m_pSpriteTexture->GetTexture();
    if (pTexture)
    {
        if (pTexture->ReloadFromFile())
        {
            m_pSpriteTexture->SetTexture(pTexture);
            m_pSpriteBackground->SetSize(m_pSpriteTexture->GetSize());

            RefreshRenderView();
        }
    }
}

void ImageSetPanel::OnGenerateFromIndexes()
{
    if (!m_pCurrentImageSet || !m_pSpriteTexture)
        return;

    int iIndexesX = 0;
    gugu::FromString<int>(m_pEditGenerateIndexesX->text().toStdString(), iIndexesX);
    int iIndexesY = 0;
    gugu::FromString<int>(m_pEditGenerateIndexesY->text().toStdString(), iIndexesY);

    sf::Texture* pSFTexture = m_pSpriteTexture->GetTexture()->GetSFTexture();

    if (pSFTexture && iIndexesX > 0 && iIndexesY > 0)
    {
        m_pCurrentImageSet->DeleteAllSubImages();

        int iWidth  = (int)pSFTexture->getSize().x / iIndexesX;
        int iHeight = (int)pSFTexture->getSize().y / iIndexesY;

        for (int y = 0; y < iIndexesY; ++y)
        {
            for (int x = 0; x < iIndexesX; ++x)
            {
                std::string strName = "x" + gugu::ToString(x) + "_y" + gugu::ToString(y);

                gugu::SubImage* pNewSubImage = m_pCurrentImageSet->AddSubImage(strName);
                if (pNewSubImage)
                {
                    sf::IntRect oSFRect = sf::IntRect(x * iWidth, y * iHeight, iWidth, iHeight);
                    pNewSubImage->SetRect(oSFRect);
                }
            }
        }

        RefreshSubImages();
    }
}

void ImageSetPanel::OnGenerateFromRect()
{
    if (!m_pCurrentImageSet || !m_pSpriteTexture)
        return;

    int iWidth = 0;
    gugu::FromString<int>(m_pEditGenerateRectWidth->text().toStdString(), iWidth);
    int iHeight = 0;
    gugu::FromString<int>(m_pEditGenerateRectHeight->text().toStdString(), iHeight);

    int iOffsetX = 0;
    gugu::FromString<int>(m_pEditGenerateRectOffsetX->text().toStdString(), iOffsetX);
    int iOffsetY = 0;
    gugu::FromString<int>(m_pEditGenerateRectOffsetY->text().toStdString(), iOffsetY);

    sf::Texture* pSFTexture = m_pSpriteTexture->GetTexture()->GetSFTexture();

    if (pSFTexture && iWidth > 0 && iHeight > 0 && iOffsetX >= 0 && iOffsetY >= 0)
    {
        m_pCurrentImageSet->DeleteAllSubImages();

        int x = 0;
        int iLeft = 0;
        while (iLeft + iWidth <= pSFTexture->getSize().x)
        {
            int y = 0;
            int iTop = 0;
            while (iTop + iHeight <= pSFTexture->getSize().y)
            {
                std::string strName = "x" + gugu::ToString(x) + "_y" + gugu::ToString(y);

                gugu::SubImage* pNewSubImage = m_pCurrentImageSet->AddSubImage(strName);
                if (pNewSubImage)
                {
                    sf::IntRect oSFRect = sf::IntRect(iLeft, iTop, iWidth, iHeight);
                    pNewSubImage->SetRect(oSFRect);
                }

                ++y;
                iTop += iHeight + iOffsetY;
            }

            ++x;
            iLeft += iWidth + iOffsetX;
        }

        RefreshSubImages();
    }
}

void ImageSetPanel::OnShrinkSelectionToPixels()
{
    if (!m_pCurrentImageSet)
        return;

    gugu::SubImage* pCurrentSubImage = GetCurrentSubImage();
    if (!pCurrentSubImage)
        return;

    if (!m_pSpriteTexture || !m_pSpriteTexture->GetTexture() || !m_pSpriteTexture->GetTexture()->GetSFTexture())
        return;

    sf::Texture* pSFTexture = m_pSpriteTexture->GetTexture()->GetSFTexture();
    sf::Image oSFImage = pSFTexture->copyToImage();

    int iShrinkLeft = 0;
    int iShrinkTop = 0;
    int iShrinkWidth = 0;
    int iShrinkHeight = 0;
    bool bShrink = true;

    sf::IntRect oSFRect = pCurrentSubImage->GetRect();
    int xMin = gugu::Max(0, oSFRect.left);
    int xMax = gugu::Min((int)pSFTexture->getSize().x, oSFRect.left + oSFRect.width);
    int yMin = gugu::Max(0, oSFRect.top);
    int yMax = gugu::Min((int)pSFTexture->getSize().y, oSFRect.top + oSFRect.height);

    //Shrink left to right
    bShrink = true;
    for (int x = xMin; bShrink && x < xMax; ++x)
    {
        for (int y = yMin; y < yMax; ++y)
            if (oSFImage.getPixel(x,y).a > 0)
                bShrink = false;
        if (bShrink)
            ++iShrinkLeft;
    }

    //Shrink right to left
    bShrink = true;
    for (int x = xMax-1; bShrink && x >= xMin; --x)
    {
        for (int y = yMin; y < yMax; ++y)
            if (oSFImage.getPixel(x,y).a > 0)
                bShrink = false;
        if (bShrink)
            ++iShrinkWidth;
    }

    //Shrink top to bottom
    bShrink = true;
    for (int y = yMin; bShrink && y < yMax; ++y)
    {
        for (int x = xMin; x < xMax; ++x)
            if (oSFImage.getPixel(x,y).a > 0)
                bShrink = false;
        if (bShrink)
            ++iShrinkTop;
    }

    //Shrink bottom to top
    bShrink = true;
    for (int y = yMax-1; bShrink && y >= yMin; --y)
    {
        for (int x = xMin; x < xMax; ++x)
            if (oSFImage.getPixel(x,y).a > 0)
                bShrink = false;
        if (bShrink)
            ++iShrinkHeight;
    }

    //Apply
    sf::Vector2f kPosition, kSize;
    kPosition.x = oSFRect.left + iShrinkLeft;
    kPosition.y = oSFRect.top + iShrinkTop;
    kSize.x = gugu::Max(1, oSFRect.width - iShrinkLeft - iShrinkWidth);
    kSize.y = gugu::Max(1, oSFRect.height - iShrinkTop - iShrinkHeight);

    m_pSpriteSelection->SetPosition(kPosition);
    m_pSpriteSelection->SetSize(kSize);

    ComputePostDrag();
}

void ImageSetPanel::OnZoomSliderMoved(int _iValue)
{
    m_fZoomCurrent = (float)_iValue * m_fRealZoomCoeff;
    RefreshRenderView();
}

void ImageSetPanel::OnHScrollSliderMoved(int _iValue)
{
    m_iViewOffsetX = _iValue;
    RefreshRenderView();
}

void ImageSetPanel::OnVScrollSliderMoved(int _iValue)
{
    m_iViewOffsetY = _iValue;
    RefreshRenderView();
}

void ImageSetPanel::OnAddRow()
{
    AddSubImage("Image_", false, 5, 5, 20, 20);
}

void ImageSetPanel::AddSubImage(const std::string& _strName, bool _bSkipFirstIncrement, int _iLeft, int _iTop, int _iWidth, int _iHeight)
{
    if (!m_pCurrentImageSet)
        return;

    //TODO: Refactoriser et centraliser
    int iNameIndex = 1;
    bool bIsFirstIncrement = true;
    bool bRetry = true;
    while (bRetry)
    {
        std::string strImageName = _strName;

        if (!_bSkipFirstIncrement || !bIsFirstIncrement)
        {
            int iDigits = 1;
            int iDivide = iNameIndex;
            while (iDivide > 9)
            {
                ++iDigits;
                iDivide = iDivide / 10;
            }

            for (int i = 3; i > iDigits; --i)
            {
                strImageName += "0";
            }
            strImageName += gugu::ToString(iNameIndex);
        }

        bIsFirstIncrement = false;

        gugu::SubImage* pNewSubImage = m_pCurrentImageSet->AddSubImage(strImageName);
        if (pNewSubImage)
        {
            sf::IntRect oSFRect = sf::IntRect(_iLeft, _iTop, _iWidth, _iHeight);
            pNewSubImage->SetRect(oSFRect);

            int iRow = m_pListSubImages->rowCount();

            m_pListSubImages->blockSignals(true);
            m_pListSubImages->insertRow(iRow);
            m_pListSubImages->setItem(iRow, 0, new QTableWidgetItem( QString::fromStdString(strImageName) ));
            m_pListSubImages->setItem(iRow, 1, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.left)) ));
            m_pListSubImages->setItem(iRow, 2, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.top)) ));
            m_pListSubImages->setItem(iRow, 3, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.width)) ));
            m_pListSubImages->setItem(iRow, 4, new QTableWidgetItem( QString::fromStdString(gugu::ToString(oSFRect.height)) ));
            m_pListSubImages->resizeRowsToContents();
            m_pListSubImages->blockSignals(false);

            m_mapSubImages[m_pListSubImages->item(iRow, 0)] = pNewSubImage;

            bRetry = false;
        }
        else
        {
            ++iNameIndex;
            if (iNameIndex > 999)
                bRetry = false;
        }
    }
}

void ImageSetPanel::OnRemoveRow()
{
    m_pListSubImages->blockSignals(true);
    QList<QTableWidgetSelectionRange> ranges = m_pListSubImages->selectedRanges();
    bool bAbort = false;
    while (!ranges.empty() && !bAbort)
    {
        int iBegin = ranges.front().topRow();
        int iCount = ranges.front().rowCount();

        do
        {
            if (!RemoveSubImage(iBegin))    //User may cancel a deletion
                bAbort = true;
        }
        while ( --iCount && !bAbort);

        ranges = m_pListSubImages->selectedRanges();
    }
    m_pListSubImages->blockSignals(false);

    SelectSubImage(-1);
}

void ImageSetPanel::OnRenderResized()
{
    RefreshRenderView();
}

void ImageSetPanel::RefreshRenderView()
{
    //compute view size
    sf::Vector2f kSizeWindow = sf::Vector2f(m_pWindow->GetSize());
    sf::Vector2f kSizeView = kSizeWindow / m_fZoomCurrent;
    m_pWindow->GetMainCamera()->SetSize(kSizeView);

    //compute sliders size
    if (m_pSpriteTexture->GetSize().x - kSizeView.x > 0.001f)
    {
        m_pHorizontalScrollBar->setRange(0, (int)m_pSpriteTexture->GetSize().x - (int)kSizeView.x);
        m_iViewOffsetX = gugu::Min(m_iViewOffsetX, m_pHorizontalScrollBar->maximum());
    }
    else
        m_pHorizontalScrollBar->setRange(0, 0);

    if (m_pSpriteTexture->GetSize().y - kSizeView.y > 0.001f)
    {
        m_pVerticalScrollBar->setRange(0, (int)m_pSpriteTexture->GetSize().y - (int)kSizeView.y);
        m_iViewOffsetY = gugu::Min(m_iViewOffsetY, m_pVerticalScrollBar->maximum());
    }
    else
        m_pVerticalScrollBar->setRange(0, 0);

    //compute view position
    if (m_pHorizontalScrollBar->maximum() > 0)
    {
        float fRatio = (float)m_iViewOffsetX / (float)m_pHorizontalScrollBar->maximum();
        float fMin = kSizeView.x / 2.f;
        float fMax = m_pSpriteTexture->GetSize().x - kSizeView.x / 2.f;

        m_kViewCenter.x = fMin + (fMax-fMin) * fRatio;
    }
    else
    {
        m_kViewCenter.x = kSizeView.x / 2.f;
    }

    if (m_pVerticalScrollBar->maximum() > 0)
    {
        float fRatio = (float)m_iViewOffsetY / (float)m_pVerticalScrollBar->maximum();
        float fMin = kSizeView.y / 2.f;
        float fMax = m_pSpriteTexture->GetSize().y - kSizeView.y / 2.f;

        m_kViewCenter.y = fMin + (fMax-fMin) * fRatio;
    }
    else
    {
        m_kViewCenter.y = kSizeView.y / 2.f;
    }

    m_pWindow->GetMainCamera()->SetTarget(m_kViewCenter);
}

void ImageSetPanel::OnDragSelection()
{
    //Ensure snap by casting to integers
    m_pSpriteSelection->SetPosition(sf::Vector2f((int)m_pSpriteSelection->GetPosition().x, (int)m_pSpriteSelection->GetPosition().y));

    ComputePostDrag();
}

void ImageSetPanel::OnDragEdge(gugu::ElementSprite* _pSpriteEdge)
{
    sf::Vector2f kSize                  = m_pSpriteSelection->GetSize();
    sf::Vector2f kPositionTopLeft       = m_pSpriteSelection->GetPosition();
    sf::Vector2f kPositionBottomRight   = kPositionTopLeft + kSize;
    sf::Vector2f kPositionEdge          = _pSpriteEdge->GetPosition();

    //Ensure snap by casting to integers
    kPositionEdge.x = (int)kPositionEdge.x;
    kPositionEdge.y = (int)kPositionEdge.y;

    if (_pSpriteEdge == m_pSpriteEdgeBottomLeft || _pSpriteEdge == m_pSpriteEdgeLeft || _pSpriteEdge == m_pSpriteEdgeTopLeft)
    {
        kPositionTopLeft.x = gugu::Min(kPositionTopLeft.x + kPositionEdge.x, kPositionBottomRight.x -1.f);      //-1 to ensure no size of 0
    }
    if (_pSpriteEdge == m_pSpriteEdgeTopLeft || _pSpriteEdge == m_pSpriteEdgeTop || _pSpriteEdge == m_pSpriteEdgeTopRight)
    {
        kPositionTopLeft.y = gugu::Min(kPositionTopLeft.y + kPositionEdge.y, kPositionBottomRight.y -1.f);      //-1 to ensure no size of 0
    }

    kSize = kPositionBottomRight - kPositionTopLeft;

    if (_pSpriteEdge == m_pSpriteEdgeTopRight || _pSpriteEdge == m_pSpriteEdgeRight || _pSpriteEdge == m_pSpriteEdgeBottomRight)
    {
        kSize.x = gugu::Max(1.f, kPositionEdge.x);  //+1 to ensure no size of 0
    }
    if (_pSpriteEdge == m_pSpriteEdgeBottomRight || _pSpriteEdge == m_pSpriteEdgeBottom || _pSpriteEdge == m_pSpriteEdgeBottomLeft)
    {
        kSize.y = gugu::Max(1.f, kPositionEdge.y);  //+1 to ensure no size of 0
    }

    //Ensure snap by casting to integers
    kPositionTopLeft.x = (int)kPositionTopLeft.x;
    kPositionTopLeft.y = (int)kPositionTopLeft.y;
    kSize.x = (int)kSize.x;
    kSize.y = (int)kSize.y;

    m_pSpriteSelection->SetPosition(kPositionTopLeft);
    m_pSpriteSelection->SetSize(kSize);

    ComputePostDrag();
}

void ImageSetPanel::ComputePostDrag()
{
    gugu::SubImage* pCurrentSubImage = GetCurrentSubImage();
    if (pCurrentSubImage)
    {
        sf::IntRect oSFRect = sf::IntRect(m_pSpriteSelection->GetPosition().x,
                                          m_pSpriteSelection->GetPosition().y,
                                          m_pSpriteSelection->GetSize().x,
                                          m_pSpriteSelection->GetSize().y);
        pCurrentSubImage->SetRect(oSFRect);

        m_pListSubImages->blockSignals(true);
        m_pListSubImages->item(m_iCurrentRow,1)->setText( QString::fromStdString(gugu::ToString(oSFRect.left)) );
        m_pListSubImages->item(m_iCurrentRow,2)->setText( QString::fromStdString(gugu::ToString(oSFRect.top)) );
        m_pListSubImages->item(m_iCurrentRow,3)->setText( QString::fromStdString(gugu::ToString(oSFRect.width)) );
        m_pListSubImages->item(m_iCurrentRow,4)->setText( QString::fromStdString(gugu::ToString(oSFRect.height)) );
        m_pListSubImages->blockSignals(false);
    }
}

gugu::SubImage* ImageSetPanel::GetCurrentSubImage()
{
    return GetSubImage(m_iCurrentRow);
}

gugu::SubImage* ImageSetPanel::GetSubImage(int _iRow)
{
    if (_iRow >= 0)
    {
        for (std::map<QTableWidgetItem*, gugu::SubImage*>::iterator iteCurrent = m_mapSubImages.begin(); iteCurrent != m_mapSubImages.end(); ++iteCurrent)
        {
            if ((*iteCurrent).first->row() == _iRow)
                return (*iteCurrent).second;
        }
    }
    return NULL;
}

bool ImageSetPanel::RemoveSubImage(int _iRow)
{
    for (std::map<QTableWidgetItem*, gugu::SubImage*>::iterator iteCurrent = m_mapSubImages.begin(); iteCurrent != m_mapSubImages.end(); ++iteCurrent)
    {
        if ((*iteCurrent).first->row() == _iRow)
        {
            gugu::SubImage* pSubImage = (*iteCurrent).second;
            if (pSubImage)
            {
                std::vector<gugu::AnimationFrame*> vecAnimFrames;
                m_pMainWindow->GetAnimationPanel()->GetFramesUsingSubImage(pSubImage, vecAnimFrames);
                if (vecAnimFrames.size() > 0)
                {
                    QString strSubImageName = QString(pSubImage->GetName().c_str());
                    QString strAnimationName = QString(vecAnimFrames[0]->GetAnimation()->GetAnimSet()->GetFileInfo().GetName().c_str());
                    strAnimationName +=" > ";
                    strAnimationName += QString(vecAnimFrames[0]->GetAnimation()->GetName().c_str());

                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setWindowIcon(QIcon("Icons/exclamation.png"));
                    msgBox.setWindowTitle("Warning");
                    msgBox.setText("The SubImage \""+ strSubImageName +"\" is used by this Animation : "+ strAnimationName +"\nDeleting this SubImage will update the Animation (Without saving it).");
                    msgBox.setInformativeText("Do you wish to continue ?");
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Ok)
                    {
                        RemoveSubImageImpl(_iRow);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    RemoveSubImageImpl(_iRow);
                    return true;
                }
            }

            return false;
        }
    }

    return false;
}

void ImageSetPanel::RemoveSubImageImpl(int _iRow)
{
    for (std::map<QTableWidgetItem*, gugu::SubImage*>::iterator iteCurrent = m_mapSubImages.begin(); iteCurrent != m_mapSubImages.end(); ++iteCurrent)
    {
        if ((*iteCurrent).first->row() == _iRow)
        {
            gugu::SubImage* pSubImage = (*iteCurrent).second;
            if (pSubImage)
            {
                m_pMainWindow->GetAnimationPanel()->PurgeSubImageInAnimSets(pSubImage);

                m_pCurrentImageSet->DeleteSubImage(pSubImage);
            }
            m_mapSubImages.erase(iteCurrent);
            m_pListSubImages->removeRow(_iRow);

            return;
        }
    }
}

void ImageSetPanel::OnNewImageSet()
{
    //TODO: Refactoriser et centraliser
    std::string strResourceName = "NewImageSet_";
    int iNameIndex = 1;
    bool bRetry = true;
    while (bRetry)
    {
        std::string strTestName = strResourceName;

        int iDigits = 1;
        int iDivide = iNameIndex;
        while (iDivide > 9)
        {
            ++iDigits;
            iDivide = iDivide / 10;
        }

        for (int i = 3; i > iDigits; --i)
        {
            strTestName += "0";
        }
        strTestName += gugu::ToString(iNameIndex);

        strTestName += ".imageset.xml";
        if (!gugu::GetResources()->HasResource(strTestName))
        {
            strResourceName = strTestName;
            bRetry = false;
        }
        else
        {
            ++iNameIndex;
            if (iNameIndex > 999)
            {
                strResourceName = strTestName;
                bRetry = false;
            }
        }
    }

    gugu::FileInfo oFileInfo;
    oFileInfo.SetPath(gugu::GetResources()->GetPathAssets());
    oFileInfo.SetName(strResourceName);

    gugu::ImageSet* pImageSet = new gugu::ImageSet;

    bool bSuccess = gugu::GetResources()->AddResource(pImageSet, oFileInfo);
    if (bSuccess)
    {
        RefreshImageSets();
        OpenImageSetFromID(pImageSet->GetID(), true);

        OnRenameImageSet();

        m_pMainWindow->GetAnimationPanel()->RefreshImageSets();
    }
}

void ImageSetPanel::OnRenameImageSet()
{
    if (m_pCurrentImageSet)
    {
        DialogRenameResource oDialog(m_pCurrentImageSet);
        oDialog.exec();

        RefreshImageSets();
        OpenImageSetFromID(m_pCurrentImageSet->GetID(), true);

        std::vector<gugu::AnimSet*> vecAnimSets;
        m_pMainWindow->GetAnimationPanel()->UpdateAnimationsImageSet(m_pCurrentImageSet, m_pCurrentImageSet);

        m_pMainWindow->GetAnimationPanel()->RefreshImageSets();
    }
}

void ImageSetPanel::OnSaveImageSet()
{
    if (m_pCurrentImageSet)
    {
        m_pCurrentImageSet->SaveToFile();
    }
}

void ImageSetPanel::OnDeleteImageSet()
{
    if (m_pCurrentImageSet)
    {
        QString strName(m_pCurrentImageSet->GetFileInfo().GetPathName().c_str());

        //TODO: centralize this (same code in AnimSet Editor)
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon("Icons/exclamation.png"));
        msgBox.setWindowTitle("Confirm");
        msgBox.setText("This file will be deleted :\n"+ strName +"");
        msgBox.setInformativeText("Do you confirm this operation ?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        //TODO: This is a hack to force the msgbox size (http://www.qtcentre.org/threads/22298-QMessageBox-Controlling-the-width?p=113348#post113348)
        //msgBox.setMinimumWidth(300);
        QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        int ret = msgBox.exec();

        if (ret == QMessageBox::Ok)
        {
            m_pMainWindow->GetAnimationPanel()->UpdateAnimationsImageSet(m_pCurrentImageSet, NULL);

            gugu::GetResources()->DeleteResource(m_pCurrentImageSet);

            RefreshImageSets();
            OpenImageSetFromItem(NULL, true);

            m_pMainWindow->GetAnimationPanel()->RefreshImageSets();
        }
    }
}

