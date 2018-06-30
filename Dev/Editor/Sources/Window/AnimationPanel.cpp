#include "Window/AnimationPanel.h"

#include "Dialog/DialogGenerateAnimRange.h"
#include "Dialog/DialogRename.h"
#include "Dialog/DialogRenameResource.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Render/QSFMLCanvas.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Utility/System.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QCheckBox>


AnimationPanel::AnimationPanel(MainWindow* _pMainWindow, const std::string& _strName)
: ToolWindow(_pMainWindow, _strName)
, m_pWindow(NULL)
, m_pSpriteAnimated(NULL)
, m_pCurrentFrame(NULL)
, m_bAutoPlay(true)
{
    //Top Layout
    QHBoxLayout* pLayoutTop = new QHBoxLayout(this);
    pLayoutTop->setContentsMargins(0, 0, 0, 0);

    //List AnimSets
    {
        m_pListAnimSets = new QListWidget(this);
        pLayoutTop->addWidget(m_pListAnimSets);

        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        m_pListAnimSets->setSizePolicy(sizePolicy);
        m_pListAnimSets->setMaximumSize(QSize(220, 16777215));

        connect(m_pListAnimSets, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(OnAnimSetChanged(QListWidgetItem*,QListWidgetItem*)));
    }

    //Edition Layout
    QWidget* pWidgetEdition = new QWidget;
    pWidgetEdition->setFixedWidth(220);

    QVBoxLayout* pLayoutEdition = new QVBoxLayout(pWidgetEdition);
    pLayoutEdition->setContentsMargins(0, 0, 0, 0);
    pLayoutTop->addWidget(pWidgetEdition);

    //Default ImageSet
    {
        m_pBoxDefaultImageSets = new QComboBox;
        pLayoutEdition->addWidget(m_pBoxDefaultImageSets);

        connect(m_pBoxDefaultImageSets, SIGNAL(currentIndexChanged(int)), this, SLOT(OnDefaultImageSetChanged(int)));
    }

    //Animations Tools
    {
        QHBoxLayout* pLayout = new QHBoxLayout();
        pLayoutEdition->addLayout(pLayout);

        QPushButton* pButtonAdd = new QPushButton("Add");
        pButtonAdd->setIcon(QIcon("Icons/add.png"));
        pLayout->addWidget(pButtonAdd);

        QPushButton* pButtonRename = new QPushButton("Edit");
        pButtonRename->setIcon(QIcon("Icons/pencil.png"));
        pLayout->addWidget(pButtonRename);

        QPushButton* pButtonRemove = new QPushButton("Remove");
        pButtonRemove->setIcon(QIcon("Icons/cross.png"));
        pLayout->addWidget(pButtonRemove);

        pLayout->addStretch();

        connect(pButtonAdd,     SIGNAL(clicked()), this, SLOT(OnAddAnimation()));
        connect(pButtonRename,  SIGNAL(clicked()), this, SLOT(OnRenameAnimation()));
        connect(pButtonRemove,  SIGNAL(clicked()), this, SLOT(OnRemoveAnimation()));
    }

    //List Animations
    {
        m_pListAnimations = new QListWidget;
        pLayoutEdition->addWidget(m_pListAnimations);

        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        m_pListAnimations->setSizePolicy(sizePolicy);
        m_pListAnimations->setMaximumSize(QSize(16777215, 200));

        connect(m_pListAnimations   , SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)) , this, SLOT(OnAnimationChanged(QListWidgetItem*,QListWidgetItem*)));
    }

    //Frames Tools
    {
        //Generate from range
        QHBoxLayout* pLayoutGenerateRange = new QHBoxLayout();
        pLayoutEdition->addLayout(pLayoutGenerateRange);

        QPushButton* pButtonGenerateRange = new QPushButton("Generate from range");
        pLayoutGenerateRange->addWidget(pButtonGenerateRange);

        connect(pButtonGenerateRange, SIGNAL(clicked()), this, SLOT(OnGenerateFromRange()));

        //Generate from folder
        QHBoxLayout* pLayoutGenerateFolder = new QHBoxLayout();
        pLayoutEdition->addLayout(pLayoutGenerateFolder);

        QPushButton* pButtonGenerateFolder = new QPushButton("Generate from folder");
        pLayoutGenerateFolder->addWidget(pButtonGenerateFolder);

        connect(pButtonGenerateFolder, SIGNAL(clicked()), this, SLOT(OnGenerateFromFolder()));

        //Default Duration
        QHBoxLayout* pLayoutFrameDuration = new QHBoxLayout;
        pLayoutEdition->addLayout(pLayoutFrameDuration);

        QLabel* pTextFrameDuration = new QLabel;
        pTextFrameDuration->setText("Default Duration");
        pLayoutFrameDuration->addWidget(pTextFrameDuration);

        m_pEditDefaultDuration = new QLineEdit;
        m_pEditDefaultDuration->setFixedWidth(30);
        m_pEditDefaultDuration->setText(QString("%1").arg(0.2f));
        pLayoutFrameDuration->addWidget(m_pEditDefaultDuration);

        QPushButton* pButtonApplyDuration = new QPushButton("Reset Frames");
        pLayoutFrameDuration->addWidget(pButtonApplyDuration);

        connect(pButtonApplyDuration, SIGNAL(clicked()), this, SLOT(OnApplyDefaultDuration()));

        //Basics (add, remove)
        QHBoxLayout* pLayoutBasics = new QHBoxLayout();
        pLayoutEdition->addLayout(pLayoutBasics);

        QPushButton* pButtonAdd = new QPushButton("Add Frame");
        pButtonAdd->setIcon(QIcon("Icons/add.png"));
        pLayoutBasics->addWidget(pButtonAdd);

        QPushButton* pButtonRemove = new QPushButton("Remove Frame");
        pButtonRemove->setIcon(QIcon("Icons/cross.png"));
        pLayoutBasics->addWidget(pButtonRemove);

        //pLayoutBasics->addStretch();

        connect(pButtonAdd,     SIGNAL(clicked()), this, SLOT(OnAddFrame()));
        connect(pButtonRemove,  SIGNAL(clicked()), this, SLOT(OnRemoveFrame()));
    }

    //List Frames
    {
        m_pListFrames = new QListWidget;
        pLayoutEdition->addWidget(m_pListFrames);

        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        m_pListFrames->setSizePolicy(sizePolicy);
        m_pListFrames->setMaximumSize(QSize(16777215, 200));

        connect(m_pListFrames   , SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)) , this, SLOT(OnFrameChanged(QListWidgetItem*,QListWidgetItem*)));
    }

    //Frame ImageSet
    {
        m_pBoxFrameImageSets = new QComboBox;
        pLayoutEdition->addWidget(m_pBoxFrameImageSets);

        connect(m_pBoxFrameImageSets, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFrameImageSetChanged(int)));
    }

    //Frame SubImage
    {
        m_pBoxFrameSubImages = new QComboBox;
        pLayoutEdition->addWidget(m_pBoxFrameSubImages);

        connect(m_pBoxFrameSubImages, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnFrameSubImageChanged(QString)));
    }

    //Frame Textures
    {
        m_pBoxFrameTextures = new QComboBox;
        pLayoutEdition->addWidget(m_pBoxFrameTextures);

        std::vector<const gugu::ResourceInfo*> vecInfos;
        gugu::GetResources()->GetLoadedTextureInfos(vecInfos);

        m_pBoxFrameTextures->addItem("", QVariant::fromValue(QString("")));
        for (size_t i = 0; i < vecInfos.size(); ++i)
        {
            m_pBoxFrameTextures->addItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()), QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));
        }

        connect(m_pBoxFrameTextures, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFrameTextureChanged(int)));
    }

    //Frame Parameters
    {
        QHBoxLayout* pLayoutEditFrameDuration = new QHBoxLayout;
        pLayoutEdition->addLayout(pLayoutEditFrameDuration);

        QLabel* pTextFrameDuration = new QLabel;
        pTextFrameDuration->setText("Duration");
        pLayoutEditFrameDuration->addWidget(pTextFrameDuration);

        m_pEditFrameDuration = new QLineEdit;
        m_pEditFrameDuration->setFixedWidth(30);
        pLayoutEditFrameDuration->addWidget(m_pEditFrameDuration);

        pLayoutEditFrameDuration->addStretch();

        connect(m_pEditFrameDuration, SIGNAL(textEdited(QString)), this, SLOT(OnFrameDurationChanged(QString)));
    }

    //Bottom Layout
    pLayoutEdition->addStretch();

    //Right-side
    QVBoxLayout* pLayoutViewer = new QVBoxLayout();
    pLayoutTop->addLayout(pLayoutViewer);

    //Edition Tools
    {
        QHBoxLayout* pLayoutTools = new QHBoxLayout();
        pLayoutViewer->addLayout(pLayoutTools);

        QCheckBox* pCheckAutoPlay = new QCheckBox("Auto Play");
        pCheckAutoPlay->setChecked(m_bAutoPlay);
        pLayoutTools->addWidget(pCheckAutoPlay);

        pLayoutTools->addStretch();

        connect(pCheckAutoPlay, SIGNAL(stateChanged(int)), this, SLOT(OnCheckAutoPlay(int)));
    }

    //Render Canvas
    {
        m_pRenderWidget = new gugu::QSFMLCanvas(this, QPoint(0, 0), QSize(400, 400));
        pLayoutViewer->addWidget(m_pRenderWidget);

        /*QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        m_pRenderWidget->setSizePolicy(sizePolicy);*/
    }


    gugu::EngineConfig oConfig;
    oConfig.backgroundColor = sf::Color(128,128,128,255);
    //oConfig.ShowStats = true;
    gugu::Window* pWindow = new gugu::Window;
    pWindow->Init(m_pRenderWidget, oConfig);
    pWindow->SetRenderer(gugu::GetEngine()->GetDefaultRenderer());
    m_pRenderWidget->SetWindow(pWindow);
    gugu::GetEngine()->AddWindow(pWindow);

    m_pWindow = pWindow;

    //Background
    m_pSpriteAnimated = m_pWindow->GetUINode()->AddChild<gugu::ElementSpriteAnimated>();

    //Get Resources
    RefreshImageSets();
    RefreshAnimSets();
}

AnimationPanel::~AnimationPanel()
{
    qDebug("Release AnimationPanel");

    gugu::GetEngine()->RemoveWindow(m_pWindow);
    //gugu::SafeDelete(m_pWindow);
}

void AnimationPanel::PopulateMenu(QMenuBar* _pMenuBar)
{
    ToolWindow::PopulateMenu(_pMenuBar);

    QAction* pActionAnimSetNew = new QAction(this);
    pActionAnimSetNew->setText("New AnimSet");
    m_pMenu->addAction(pActionAnimSetNew);

    QObject::connect(pActionAnimSetNew, SIGNAL(triggered()), this, SLOT(OnNewAnimSet()));

    QAction* pActionAnimSetRename = new QAction(this);
    pActionAnimSetRename->setText("Rename Current");
    m_pMenu->addAction(pActionAnimSetRename);

    QObject::connect(pActionAnimSetRename, SIGNAL(triggered()), this, SLOT(OnRenameAnimSet()));

    QAction* pActionAnimSetSave = new QAction(this);
    pActionAnimSetSave->setText("Save Current");
    m_pMenu->addAction(pActionAnimSetSave);

    QObject::connect(pActionAnimSetSave, SIGNAL(triggered()), this, SLOT(OnSaveAnimSet()));

    m_pMenu->addSeparator();

    QAction* pActionAnimSetDelete = new QAction(this);
    pActionAnimSetDelete->setText("Delete Current");
    m_pMenu->addAction(pActionAnimSetDelete);

    QObject::connect(pActionAnimSetDelete, SIGNAL(triggered()), this, SLOT(OnDeleteAnimSet()));
}

void AnimationPanel::RefreshAnimSets()
{
    m_pListAnimSets->blockSignals(true);
    m_pListAnimSets->clear();

    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetLoadedAnimSetInfos(vecInfos);
    for (size_t i = 0; i < vecInfos.size(); ++i)
    {
        QListWidgetItem* pItem = new QListWidgetItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()));
        pItem->setToolTip(QString::fromStdString(vecInfos[i]->fileInfo.GetPathName()));
        pItem->setData(Qt::UserRole, QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));

        m_pListAnimSets->addItem(pItem);
    }

    m_pListAnimSets->blockSignals(false);
}

void AnimationPanel::OpenAnimSetFromID(const std::string& _strID, bool _bUpdateUI)
{
    QString qID = QString::fromStdString(_strID);
    QListWidgetItem* pItem = NULL;

    for (int i = 0; i < m_pListAnimSets->count(); ++i)
    {
        if (m_pListAnimSets->item(i)->data(Qt::UserRole).toString() == qID)
        {
            pItem = m_pListAnimSets->item(i);
            break;
        }
    }

    OpenAnimSetFromItem(pItem, _bUpdateUI);
}

void AnimationPanel::OpenAnimSetFromItem(QListWidgetItem* _pItem, bool _bUpdateUI)
{
    std::string strResourceID = "";

    if (_pItem)
    {
        strResourceID = _pItem->data(Qt::UserRole).toString().toStdString();
    }

    if (_bUpdateUI)
    {
        m_pListAnimSets->blockSignals(true);
        m_pListAnimSets->setCurrentItem(_pItem);
        m_pListAnimSets->blockSignals(false);
    }

    m_pBoxDefaultImageSets->blockSignals(true);
    m_pBoxDefaultImageSets->setCurrentIndex(0);
    m_pBoxDefaultImageSets->blockSignals(false);

    gugu::AnimSet* pAnimSet = gugu::GetResources()->GetAnimSet(strResourceID);

    m_pSpriteAnimated->ChangeAnimSet(pAnimSet);
    RefreshAnimations();
    RefreshFrames();

    if (pAnimSet)
    {
        //m_pSpriteAnimated->SetUnifiedOrigin(gugu::UDim2(0.5f, 0.f, 1.f, 0.f));
        m_pSpriteAnimated->SetUnifiedPosition(gugu::UDim2(0.5f, 0.f, 0.5f, 0.f));

        //m_pSpriteAnimated->ChangeAnimSet(_strName);

        //Refresh Global settings
        if (pAnimSet->GetImageSet())
        {
            QString qstrID = QString::fromStdString(pAnimSet->GetImageSet()->GetID());
            int iIndexDefault = m_pBoxDefaultImageSets->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);

            m_pBoxDefaultImageSets->setCurrentIndex(iIndexDefault);
        }

        //RefreshAnimations();

        if (m_pListAnimations->count() > 0)
            m_pListAnimations->setCurrentRow(0);
        //else
        //    RefreshFrames();
    }
}

void AnimationPanel::RefreshAnimations()
{
    m_pListAnimations->blockSignals(true);
    m_pListAnimations->clear();

    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        std::vector<gugu::Animation*> vecAnims;
        pAnimSet->GetAnimations(vecAnims);

        for (size_t i = 0; i < vecAnims.size(); ++i)
        {
            QListWidgetItem* pNewItem = new QListWidgetItem(QString::fromStdString(vecAnims[i]->GetName()));
            m_pListAnimations->addItem(pNewItem);
        }
    }

    m_pListAnimations->blockSignals(false);
}

void AnimationPanel::SelectAnimation(const std::string& _strName, bool _bUpdateUI)
{
    if (_bUpdateUI)
    {
        QList<QListWidgetItem*> listItems = m_pListAnimations->findItems(QString::fromStdString(_strName), Qt::MatchFixedString);
        if (!listItems.empty())
        {
            m_pListAnimations->blockSignals(true);
            m_pListAnimations->setCurrentItem(listItems[0]);
            m_pListAnimations->blockSignals(false);
        }
    }

    m_pSpriteAnimated->StopAnimation();

    m_pListFrames->blockSignals(true);
    m_pListFrames->clear();
    m_pListFrames->blockSignals(false);

    if (m_pSpriteAnimated->HasAnimation(_strName))
    {
        m_pSpriteAnimated->StartAnimation(_strName);
        m_pSpriteAnimated->SetAnimationPause(!m_bAutoPlay);

        RefreshFrames();
    }

    SelectFrame(0, true);
}

void AnimationPanel::RefreshFrames()
{
    m_pListFrames->blockSignals(true);
    m_pListFrames->clear();

    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        std::vector<gugu::AnimationFrame*> vecFrames;
        pAnimation->GetFrames(vecFrames);

        for (size_t i = 0; i < vecFrames.size(); ++i)
        {
            QListWidgetItem* pNewItem = new QListWidgetItem(QString("%1").arg(i));
            pNewItem->setData(Qt::UserRole, QVariant::fromValue(i));
            m_pListFrames->addItem(pNewItem);
        }
    }

    m_pListFrames->blockSignals(false);

    m_pCurrentFrame = NULL;
    RefreshFrame();
}

void AnimationPanel::SelectFrame(int _iFrameIndex, bool _bUpdateUI)
{
    m_pCurrentFrame = NULL;

    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        gugu::AnimationFrame* pFrame = pAnimation->GetFrame(_iFrameIndex);
        if (pFrame)
        {
            if (_bUpdateUI)
            {
                m_pListFrames->blockSignals(true);
                m_pListFrames->setCurrentRow(_iFrameIndex);
                m_pListFrames->blockSignals(false);
            }

            m_pCurrentFrame = pFrame;
        }
    }

    RefreshFrame();
}


void AnimationPanel::RefreshImageSets()
{
    m_pBoxDefaultImageSets->blockSignals(true);
    m_pBoxFrameImageSets->blockSignals(true);

    m_pBoxDefaultImageSets->clear();
    m_pBoxFrameImageSets->clear();

    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetLoadedImageSetInfos(vecInfos);

    m_pBoxDefaultImageSets->addItem("", QVariant::fromValue(QString("")));
    m_pBoxFrameImageSets->addItem("", QVariant::fromValue(QString("")));

    for (size_t i = 0; i < vecInfos.size(); ++i)
    {
        m_pBoxDefaultImageSets->addItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()), QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));
        m_pBoxFrameImageSets->addItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()), QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));
    }

    m_pBoxDefaultImageSets->blockSignals(false);
    m_pBoxFrameImageSets->blockSignals(false);

    //UI refresh
    int iIndexDefault = -1;

    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet && pAnimSet->GetImageSet())
    {
        QString qstrID = QString::fromStdString(pAnimSet->GetImageSet()->GetID());
        iIndexDefault = m_pBoxDefaultImageSets->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);
    }

    m_pBoxDefaultImageSets->setCurrentIndex(iIndexDefault);

    RefreshFrame();
}

void AnimationPanel::OnAnimSetChanged(QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious)
{
    if (_pCurrent)
        OpenAnimSetFromItem(_pCurrent, false);
}

void AnimationPanel::OnAnimationChanged(QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious)
{
    if (_pCurrent)
        SelectAnimation(_pCurrent->text().toStdString(), false);
}

void AnimationPanel::OnFrameChanged(QListWidgetItem* _pCurrent, QListWidgetItem* _pPrevious)
{
    if (_pCurrent)
    {
        int iFrameIndex = _pCurrent->data(Qt::UserRole).toInt();
        SelectFrame(iFrameIndex, false);
    }
}

void AnimationPanel::OnAddAnimation()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        //TODO: Refactoriser et centraliser
        std::string strAnimationName = "Animation_";
        int iNameIndex = 1;
        bool bRetry = true;
        while (bRetry)
        {
            std::string strTestName = strAnimationName;

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

            if (!pAnimSet->GetAnimation(strTestName))
            {
                strAnimationName = strTestName;
                bRetry = false;
            }
            else
            {
                ++iNameIndex;
                if (iNameIndex > 999)
                    bRetry = false;
            }
        }

        gugu::Animation* pAnimation = pAnimSet->AddAnimation(strAnimationName);
        if (pAnimation)
        {
            m_pListAnimations->blockSignals(true);
            QListWidgetItem* pNewItem = new QListWidgetItem(QString::fromStdString(pAnimation->GetName()));
            m_pListAnimations->addItem(pNewItem);
            m_pListAnimations->blockSignals(false);

            SelectAnimation(pAnimation->GetName(), true);

            OnRenameAnimation();
        }
    }
}

void AnimationPanel::OnRemoveAnimation()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        QListWidgetItem* pItem = m_pListAnimations->currentItem();
        int iRow = m_pListAnimations->currentRow();
        if (pItem)
        {
            gugu::Animation* pAnimation = pAnimSet->GetAnimation(pItem->text().toStdString());
            if (pAnimation)
            {
                pAnimSet->DeleteAnimation(pAnimation);
                //m_pListAnimations->takeItem(m_pListAnimations->row(pItem));
                delete pItem;   //TODO: replace with a refresh+select on the list, like for Frames
            }
        }

        if (iRow < m_pListAnimations->count())
        {
            m_pListAnimations->setCurrentRow(iRow);
        }
        else if (m_pListAnimations->count() > 0)
        {
            m_pListAnimations->setCurrentRow(iRow-1);
        }
        else
        {
            SelectAnimation("", false);
        }
    }
}

void AnimationPanel::OnRenameAnimation()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimSet && pAnimation)
    {
        std::vector<std::string> strAllAnimationNames;
        pAnimSet->GetAnimationNames(strAllAnimationNames);

        DialogRename oDialog(pAnimation->GetName(), strAllAnimationNames);

        int ret = oDialog.exec();
        if (ret == QDialog::Accepted)
        {
            pAnimation->SetName(oDialog.GetEditedName());
            RefreshAnimations();
            SelectAnimation(pAnimation->GetName(), true);
        }
    }
}

void AnimationPanel::OnAddFrame()
{
    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        gugu::AnimationFrame* pFrame = pAnimation->AddFrame();
        if (pFrame)
        {
            QString strDuration = m_pEditDefaultDuration->text();
            float fDuration = strDuration.toFloat();

            pFrame->SetDuration(fDuration);

            RefreshFrames();
            SelectFrame(pAnimation->GetFrameCount()-1, true);
        }
    }
}

void AnimationPanel::OnRemoveFrame()
{
    if (!m_pCurrentFrame)
        return;

    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        int iRow = m_pListFrames->currentRow();

        pAnimation->DeleteFrame(m_pCurrentFrame);
        m_pSpriteAnimated->RestartAnimation();
        m_pSpriteAnimated->SetAnimationPause(!m_bAutoPlay);

        RefreshFrames();

        if (iRow < m_pListFrames->count())
        {
            m_pListFrames->setCurrentRow(iRow);
        }
        else if (m_pListFrames->count() > 0)
        {
            m_pListFrames->setCurrentRow(iRow-1);
        }
    }
}

void AnimationPanel::OnDefaultImageSetChanged(int _iIndex)
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        std::string strID = m_pBoxDefaultImageSets->itemData(_iIndex).toString().toStdString();
        pAnimSet->SetImageSet(gugu::GetResources()->GetImageSet(strID));

        //No need to refresh if the frame is already edited
        if (!m_pCurrentFrame || !m_pCurrentFrame->GetSubImage())
        {
            m_pBoxFrameImageSets->blockSignals(true);
            m_pBoxFrameImageSets->setCurrentIndex(m_pBoxDefaultImageSets->currentIndex());
            m_pBoxFrameImageSets->blockSignals(false);

            RefreshSubImages();
        }
    }
}

void AnimationPanel::OnFrameImageSetChanged(int _iIndex)
{
    RefreshSubImages();
}

void AnimationPanel::OnFrameSubImageChanged(const QString& _strSubImage)
{
    if (m_pCurrentFrame)
    {
        gugu::ImageSet* pSetDefault = NULL;
        gugu::ImageSet* pSetFrame = NULL;
        std::string strNameSetDefault = m_pBoxDefaultImageSets->itemData(m_pBoxDefaultImageSets->currentIndex()).toString().toStdString();
        std::string strNameSetFrame = m_pBoxFrameImageSets->itemData(m_pBoxFrameImageSets->currentIndex()).toString().toStdString();

        pSetDefault = gugu::GetResources()->GetImageSet(strNameSetDefault);
        pSetFrame = gugu::GetResources()->GetImageSet(strNameSetFrame);

        gugu::ImageSet* pImageSet = (pSetFrame)? pSetFrame : pSetDefault;
        if (pImageSet)
        {
            m_pCurrentFrame->SetSubImage(pImageSet->GetSubImage(_strSubImage.toStdString()));
            m_pCurrentFrame->SetTexture(NULL);
        }

        RefreshFrame();
    }
}

void AnimationPanel::OnFrameTextureChanged(int _iIndex)
{
    if (m_pCurrentFrame)
    {
        std::string strID = m_pBoxFrameTextures->itemData(_iIndex).toString().toStdString();

        gugu::Texture* pTexture = gugu::GetResources()->GetTexture(strID);
        if (pTexture)
        {
            m_pCurrentFrame->SetSubImage(NULL);
            m_pCurrentFrame->SetTexture(pTexture);
        }

        RefreshFrame();
    }
}

void AnimationPanel::OnFrameDurationChanged(const QString& _strDuration)
{
    if (m_pCurrentFrame)
    {
        float fDuration = _strDuration.toFloat();
        m_pCurrentFrame->SetDuration(fDuration);
    }
}

void AnimationPanel::RefreshSubImages()
{
    m_pBoxFrameSubImages->blockSignals(true);

    m_pBoxFrameSubImages->clear();

    gugu::ImageSet* pSetDefault = NULL;
    gugu::ImageSet* pSetFrame = NULL;
    std::string strNameSetDefault = m_pBoxDefaultImageSets->itemData(m_pBoxDefaultImageSets->currentIndex()).toString().toStdString();
    std::string strNameSetFrame = m_pBoxFrameImageSets->itemData(m_pBoxFrameImageSets->currentIndex()).toString().toStdString();

    pSetDefault = gugu::GetResources()->GetImageSet(strNameSetDefault);
    pSetFrame = gugu::GetResources()->GetImageSet(strNameSetFrame);

    gugu::ImageSet* pImageSet = (pSetFrame)? pSetFrame : pSetDefault;
    if (pImageSet)
    {
        std::vector<gugu::SubImage*> vecSubImages;
        pImageSet->GetSubImages(vecSubImages);

        m_pBoxFrameSubImages->addItem("");
        for (size_t i = 0; i < vecSubImages.size(); ++i)
        {
            m_pBoxFrameSubImages->addItem(QString::fromStdString(vecSubImages[i]->GetName()));
        }
    }

    m_pBoxFrameSubImages->blockSignals(false);
}

void AnimationPanel::RefreshFrame()
{
    m_pBoxFrameImageSets->blockSignals(true);
    m_pBoxFrameImageSets->setCurrentIndex(m_pBoxDefaultImageSets->currentIndex());
    RefreshSubImages();
    m_pBoxFrameImageSets->blockSignals(false);

    m_pBoxFrameTextures->blockSignals(true);
    m_pBoxFrameTextures->setCurrentIndex(0);
    m_pBoxFrameTextures->blockSignals(false);

    m_pBoxFrameSubImages->blockSignals(true);
    m_pBoxFrameSubImages->setCurrentIndex(0);
    m_pBoxFrameSubImages->blockSignals(false);

    m_pEditFrameDuration->blockSignals(true);
    m_pEditFrameDuration->setText(QString(""));
    m_pEditFrameDuration->blockSignals(false);

    if (m_pCurrentFrame)
    {
        if (m_pCurrentFrame->GetTexture())
        {
            m_pBoxFrameTextures->blockSignals(true);
            QString qstrID = QString::fromStdString(m_pCurrentFrame->GetTexture()->GetID());
            int iIndex = m_pBoxFrameTextures->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);
            m_pBoxFrameTextures->setCurrentIndex(iIndex);
            m_pBoxFrameTextures->blockSignals(false);
        }
        else if (m_pCurrentFrame->GetSubImage())
        {
            m_pBoxFrameImageSets->blockSignals(true);
            QString qstrID = QString::fromStdString(m_pCurrentFrame->GetSubImage()->GetImageSet()->GetID());
            int iIndex = m_pBoxDefaultImageSets->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);
            m_pBoxFrameImageSets->setCurrentIndex(iIndex);
            RefreshSubImages();
            m_pBoxFrameImageSets->blockSignals(false);

            m_pBoxFrameSubImages->blockSignals(true);
            m_pBoxFrameSubImages->setCurrentIndex(m_pBoxFrameSubImages->findText(QString(m_pCurrentFrame->GetSubImage()->GetName().c_str())));
            m_pBoxFrameSubImages->blockSignals(false);
        }

        m_pEditFrameDuration->blockSignals(true);
        m_pEditFrameDuration->setText(QString("%1").arg(m_pCurrentFrame->GetDuration()));
        m_pEditFrameDuration->blockSignals(false);

        if (!m_bAutoPlay)
        {
            m_pSpriteAnimated->SetCurrentFrame(m_pCurrentFrame);
        }
    }
}

void AnimationPanel::OnNewAnimSet()
{
    //TODO: Refactoriser et centraliser
    std::string strResourceName = "NewAnimSet_";
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

        strTestName += ".animset.xml";
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

    gugu::AnimSet* pAnimSet = new gugu::AnimSet;

    bool bSuccess = gugu::GetResources()->AddResource(pAnimSet, oFileInfo);
    if (bSuccess)
    {
        RefreshAnimSets();
        OpenAnimSetFromID(pAnimSet->GetID(), true);

        OnRenameAnimSet();
    }
}

void AnimationPanel::OnRenameAnimSet()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        DialogRenameResource oDialog(pAnimSet);
        oDialog.exec();

        RefreshAnimSets();
        OpenAnimSetFromID(pAnimSet->GetID(), true);
    }
}

void AnimationPanel::OnSaveAnimSet()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        pAnimSet->SaveToFile();
    }
}

void AnimationPanel::OnDeleteAnimSet()
{
    gugu::AnimSet* pAnimSet = m_pSpriteAnimated->GetAnimSet();
    if (pAnimSet)
    {
        QString strName(pAnimSet->GetFileInfo().GetPathName().c_str());

        //TODO: centralize this (same code in ImageSet Editor)
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
            gugu::GetResources()->DeleteResource(pAnimSet);

            RefreshAnimSets();
            OpenAnimSetFromItem(NULL, true);
        }
    }
}

void AnimationPanel::GetFramesUsingSubImage(gugu::SubImage* _pSubImage, std::vector<gugu::AnimationFrame*>& _vecAnimFrames)
{
    if (!_pSubImage)
        return;

    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetLoadedAnimSetInfos(vecInfos);
    for (size_t i = 0; i < vecInfos.size(); ++i)
    {
        gugu::AnimSet* pAnimSet = gugu::GetResources()->GetAnimSet(vecInfos[i]->resourceID);
        if (!pAnimSet)
            continue;

        std::vector<gugu::Animation*> vecAnimations;
        pAnimSet->GetAnimations(vecAnimations);
        for (size_t j = 0; j < vecAnimations.size(); ++j)
        {
            gugu::Animation* pAnimation = vecAnimations[j];
            if (!pAnimation)
                continue;

            std::vector<gugu::AnimationFrame*> vecFrames;
            pAnimation->GetFrames(vecFrames);
            for (size_t k = 0; k < vecFrames.size(); ++k)
            {
                gugu::AnimationFrame* pFrame = vecFrames[k];
                if (!pFrame)
                    continue;

                if (pFrame->GetSubImage() == _pSubImage)
                {
                    _vecAnimFrames.push_back(pFrame);
                }
            }
        }
    }
}

void AnimationPanel::UpdateAnimationsImageSet(gugu::ImageSet* _pImageSetFrom, gugu::ImageSet* _pImageSetTo)
{
    if (!_pImageSetFrom)
        return;

    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetLoadedAnimSetInfos(vecInfos);
    for (size_t i = 0; i < vecInfos.size(); ++i)
    {
        gugu::AnimSet* pAnimSet = dynamic_cast<gugu::AnimSet*>(vecInfos[i]->resource);
        if (!pAnimSet)
            continue;

        bool bUpdated = false;

        if (pAnimSet->GetImageSet() == _pImageSetFrom)
        {
            pAnimSet->SetImageSet(_pImageSetTo);
            bUpdated = true;
        }

        std::vector<gugu::Animation*> vecAnimations;
        pAnimSet->GetAnimations(vecAnimations);
        for (size_t j = 0; j < vecAnimations.size(); ++j)
        {
            gugu::Animation* pAnimation = vecAnimations[j];
            if (!pAnimation)
                continue;

            std::vector<gugu::AnimationFrame*> vecFrames;
            pAnimation->GetFrames(vecFrames);
            for (size_t k = 0; k < vecFrames.size(); ++k)
            {
                gugu::AnimationFrame* pFrame = vecFrames[k];
                if (!pFrame)
                    continue;

                if (pFrame->GetSubImage() && pFrame->GetSubImage()->GetImageSet() == _pImageSetFrom)
                {
                    if (_pImageSetTo == NULL)
                    {
                        pFrame->SetSubImage(NULL);
                        bUpdated = true;
                    }
                }
            }
        }

        if (bUpdated)
            pAnimSet->SaveToFile();
    }
}

void AnimationPanel::PurgeSubImageInAnimSets(gugu::SubImage* _pSubImage)
{
    if (!_pSubImage)
        return;

    std::vector<gugu::AnimationFrame*> vecAnimFrames;
    GetFramesUsingSubImage(_pSubImage, vecAnimFrames);
    for (size_t i = 0; i < vecAnimFrames.size(); ++i)
    {
        vecAnimFrames[i]->SetSubImage(NULL);
    }

    RefreshFrame();     //This will also call RefreshSubImages()
}

void AnimationPanel::OnGenerateFromRange()
{
    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        gugu::ImageSet* pSetDefault = NULL;
        gugu::ImageSet* pSetFrame = NULL;
        std::string strNameSetDefault = m_pBoxDefaultImageSets->itemData(m_pBoxDefaultImageSets->currentIndex()).toString().toStdString();
        std::string strNameSetFrame = m_pBoxFrameImageSets->itemData(m_pBoxFrameImageSets->currentIndex()).toString().toStdString();

        pSetDefault = gugu::GetResources()->GetImageSet(strNameSetDefault);
        pSetFrame = gugu::GetResources()->GetImageSet(strNameSetFrame);

        gugu::ImageSet* pImageSet = (pSetFrame)? pSetFrame : pSetDefault;
        //if (pImageSet)
        {
            DialogGenerateAnimRange oDialog(pImageSet);

            int ret = oDialog.exec();
            if (ret == QDialog::Accepted)
            {
                gugu::ImageSet* pImageSet = oDialog.GetImageSet();
                gugu::SubImage* pFrom = oDialog.GetFrom();
                gugu::SubImage* pTo = oDialog.GetTo();

                if (pImageSet && pFrom && pTo)
                {
                    bool bParse = false;

                    QString strDuration = m_pEditDefaultDuration->text();
                    float fDuration = strDuration.toFloat();

                    std::vector<gugu::SubImage*> vecSubImages;
                    pImageSet->GetSubImages(vecSubImages);
                    for (size_t i = 0; i < vecSubImages.size(); ++i)
                    {
                        gugu::SubImage* pSubImage = vecSubImages[i];
                        if (pSubImage == pFrom)
                            bParse = true;

                        if (bParse)
                        {
                            gugu::AnimationFrame* pFrame = pAnimation->AddFrame();
                            if (pFrame)
                            {
                                pFrame->SetDuration(fDuration);
                                pFrame->SetSubImage(pSubImage);
                            }
                        }

                        if (pSubImage == pTo)
                            break;
                    }

                    RefreshFrames();
                    SelectFrame(0, true);
                }
            }
        }
    }
}

void AnimationPanel::OnGenerateFromFolder()
{
    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setDirectory(gugu::GetResources()->GetPathAssets().c_str());
        dialog.setViewMode(QFileDialog::List);
        //dialog.setOptions(QFileDialog::ShowDirsOnly);

        int iResult = dialog.exec();
        if (iResult == QDialog::Accepted)
        {
            //QString strRoot = QApplication::applicationDirPath();
            //QDir kRootDir(strRoot);

            QStringList fileNames = dialog.selectedFiles();
            if (!fileNames.empty())
            {
                QString strPathAbsolute = fileNames[0];

                QDir kRootDir(gugu::GetResources()->GetPathAssets().c_str());
                QString strPathRelative = kRootDir.relativeFilePath(strPathAbsolute);

                std::vector<const gugu::ResourceInfo*> vecInfos;
                gugu::GetResources()->GetResourceInfosFromPath(vecInfos, strPathRelative.toStdString(), gugu::EResourceType::Texture);

                QString strDuration = m_pEditDefaultDuration->text();
                float fDuration = strDuration.toFloat();

                for (size_t i = 0; i < vecInfos.size(); ++i)
                {
                    gugu::AnimationFrame* pFrame = pAnimation->AddFrame();
                    if (pFrame)
                    {
                        pFrame->SetDuration(fDuration);
                        pFrame->SetTexture(dynamic_cast<gugu::Texture*>(vecInfos[i]->resource));
                    }
                }

                RefreshFrames();
                SelectFrame(0, true);
            }
        }
    }
}

void AnimationPanel::OnApplyDefaultDuration()
{
    gugu::Animation* pAnimation = m_pSpriteAnimated->GetAnimation();
    if (pAnimation)
    {
        QString strDuration = m_pEditDefaultDuration->text();
        float fDuration = strDuration.toFloat();

        std::vector<gugu::AnimationFrame*> vecFrames;
        pAnimation->GetFrames(vecFrames);
        for (size_t i = 0; i < vecFrames.size(); ++i)
        {
            vecFrames[i]->SetDuration(fDuration);
        }

        RefreshFrame();
    }
}

void AnimationPanel::OnCheckAutoPlay(int _iState)
{
    m_bAutoPlay = (_iState == Qt::Checked);

    m_pSpriteAnimated->SetAnimationPause(!m_bAutoPlay);
}

