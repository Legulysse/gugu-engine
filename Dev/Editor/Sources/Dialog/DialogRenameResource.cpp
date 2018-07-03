#include "Dialog/DialogRenameResource.h"

#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Resources/Resource.h"

#include <QDir>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFormLayout>


DialogRenameResource::DialogRenameResource(gugu::Resource* _pResource)
: QDialog()
{
    m_pResource = _pResource;

    setWindowTitle("Rename");
    setWindowIcon(QIcon("Icons/pencil.png"));
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    //setSizeGripEnabled(true);

    QVBoxLayout* pLayoutTop = new QVBoxLayout(this);
    setFixedWidth(400);

    //Rename Form
    {
        m_pEditFolder = new QLineEdit( this );
        m_pEditFolder->setText(m_pResource->GetFileInfo().GetPath().c_str());

        m_pEditName = new QLineEdit( this );
        m_pEditName->setText(m_pResource->GetFileInfo().GetPrettyName().c_str());

        m_pEditExtension = new QLineEdit( this );
        m_pEditExtension->setText(m_pResource->GetFileInfo().GetExtension().c_str());

        QPushButton* pButtonValidate = new QPushButton( "Confirm", this );
        pButtonValidate->setDefault(true);
        pButtonValidate->setFixedWidth(120);

        QFormLayout* pFormLayout = new QFormLayout;
        pFormLayout->addRow("Folder",       m_pEditFolder);
        pFormLayout->addRow("Name",         m_pEditName);
        pFormLayout->addRow("Extension",    m_pEditExtension);
        pFormLayout->addRow("",             pButtonValidate);
        pFormLayout->setAlignment(pButtonValidate, Qt::AlignRight);
        pLayoutTop->addLayout(pFormLayout);

        connect(pButtonValidate, SIGNAL(clicked()), this, SLOT(OnButtonConfirm()));

        m_pEditName->setFocus();
        m_pEditName->selectAll();
    }
}

void DialogRenameResource::OnButtonConfirm()
{
    std::string strPath = m_pEditFolder->text().toStdString();
    std::string strName = m_pEditName->text().toStdString();
    std::string strExtension = m_pEditExtension->text().toStdString();
    gugu::FileInfo oNewFileInfo(strPath, strName +"."+ strExtension);

    QDir dir(oNewFileInfo.GetPath().c_str());
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (gugu::GetResources()->MoveResource(m_pResource, oNewFileInfo))
    {
        accept();
    }
}
