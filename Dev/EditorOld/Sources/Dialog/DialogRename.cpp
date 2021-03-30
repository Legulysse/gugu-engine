#include "Dialog/DialogRename.h"

#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/Utility/System.h"

#include <QDir>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFormLayout>


DialogRename::DialogRename(const std::string& _strName, const std::vector<std::string>& _vecExcludedNames)
: QDialog()
{
    m_strEditedName = _strName;
    m_vecExcludedNames = _vecExcludedNames;

    gugu::StdVectorRemove(m_vecExcludedNames, m_strEditedName);     //Allow the given Name
    m_vecExcludedNames.push_back("");                               //Exclude an empty Name

    setWindowTitle("Rename");
    setWindowIcon(QIcon("Icons/pencil.png"));
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QVBoxLayout* pLayoutTop = new QVBoxLayout(this);
    setFixedWidth(300);

    //Rename Form
    {
        m_pEditName = new QLineEdit( this );
        m_pEditName->setText(m_strEditedName.c_str());

        QPushButton* pButtonValidate = new QPushButton( "Confirm", this );
        pButtonValidate->setDefault(true);
        pButtonValidate->setFixedWidth(120);

        QFormLayout* pFormLayout = new QFormLayout;
        pFormLayout->addRow("Name",      m_pEditName);
        pFormLayout->addRow("",          pButtonValidate);
        pFormLayout->setAlignment(pButtonValidate, Qt::AlignRight);
        pLayoutTop->addLayout(pFormLayout);

        m_pEditName->setFocus();
        m_pEditName->selectAll();

        connect(pButtonValidate, SIGNAL(clicked()), this, SLOT(OnButtonConfirm()));
    }
}

void DialogRename::OnButtonConfirm()
{
    std::string strName = m_pEditName->text().toStdString();
    if (!gugu::StdVectorContains(m_vecExcludedNames, strName))
    {
        m_strEditedName = strName;

        accept();
    }
}

std::string DialogRename::GetEditedName()
{
    return m_strEditedName;
}
