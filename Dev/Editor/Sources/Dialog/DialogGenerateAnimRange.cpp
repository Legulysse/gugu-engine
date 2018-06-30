#include "Dialog/DialogGenerateAnimRange.h"

#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Utility/System.h"

#include <QDir>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>


DialogGenerateAnimRange::DialogGenerateAnimRange(gugu::ImageSet* _pImageSet)
: QDialog()
{
    m_pImageSet = NULL;

    setWindowTitle("Generate Animation");
    setWindowIcon(QIcon("Icons/pencil.png"));
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QVBoxLayout* pLayoutTop = new QVBoxLayout(this);
    setFixedWidth(300);

    {
        m_pBoxImageSet = new QComboBox;
        m_pBoxSubImageFrom = new QComboBox;
        m_pBoxSubImageTo = new QComboBox;

        QPushButton* pButtonValidate = new QPushButton( "Confirm", this );
        pButtonValidate->setDefault(true);
        pButtonValidate->setFixedWidth(120);

        QFormLayout* pFormLayout = new QFormLayout;
        pFormLayout->addRow("ImageSet", m_pBoxImageSet);
        pFormLayout->addRow("From",     m_pBoxSubImageFrom);
        pFormLayout->addRow("To",       m_pBoxSubImageTo);
        pFormLayout->addRow("",         pButtonValidate);
        pFormLayout->setAlignment(pButtonValidate, Qt::AlignRight);
        pLayoutTop->addLayout(pFormLayout);

        connect(pButtonValidate, SIGNAL(clicked()), this, SLOT(OnButtonConfirm()));

        {
            std::vector<const gugu::ResourceInfo*> vecInfos;
            gugu::GetResources()->GetLoadedImageSetInfos(vecInfos);

            m_pBoxImageSet->addItem("", QVariant::fromValue(QString("")));
            for (size_t i = 0; i < vecInfos.size(); ++i)
            {
                m_pBoxImageSet->addItem(QString::fromStdString(vecInfos[i]->fileInfo.GetPrettyName()), QVariant::fromValue(QString::fromStdString(vecInfos[i]->resourceID)));
            }

            connect(m_pBoxImageSet, SIGNAL(currentIndexChanged(int)), this, SLOT(OnImageSetChanged(int)));

            if (_pImageSet)
            {
                QString qstrID = QString::fromStdString(_pImageSet->GetID());
                int iIndexDefault = m_pBoxImageSet->findData(QVariant::fromValue(qstrID), Qt::UserRole, Qt::MatchFixedString);
                m_pBoxImageSet->setCurrentIndex(iIndexDefault);
            }
        }

    }
}

void DialogGenerateAnimRange::OnImageSetChanged(int _iIndex)
{
    m_pBoxSubImageFrom->clear();
    m_pBoxSubImageTo->clear();

    std::string strID = m_pBoxImageSet->itemData(_iIndex).toString().toStdString();
    m_pImageSet = gugu::GetResources()->GetImageSet(strID);

    if (m_pImageSet)
    {
        std::vector<gugu::SubImage*> vecSubImages;
        m_pImageSet->GetSubImages(vecSubImages);
        for (size_t i = 0; i < vecSubImages.size(); ++i)
        {
            m_pBoxSubImageFrom->addItem(QString::fromStdString(vecSubImages[i]->GetName()));
            m_pBoxSubImageTo->addItem(QString::fromStdString(vecSubImages[i]->GetName()));
        }
    }
}

void DialogGenerateAnimRange::OnButtonConfirm()
{
    accept();
}

gugu::ImageSet* DialogGenerateAnimRange::GetImageSet()
{
    return m_pImageSet;
}

gugu::SubImage* DialogGenerateAnimRange::GetFrom()
{
    if (m_pImageSet)
    {
        std::string strName = m_pBoxSubImageFrom->currentText().toStdString();
        return m_pImageSet->GetSubImage(strName);
    }
    return NULL;
}

gugu::SubImage* DialogGenerateAnimRange::GetTo()
{
    if (m_pImageSet)
    {
        std::string strName = m_pBoxSubImageTo->currentText().toStdString();
        return m_pImageSet->GetSubImage(strName);
    }
    return NULL;
}


