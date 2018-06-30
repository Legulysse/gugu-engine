#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H


#include <QtWidgets/QDialog>


class QLineEdit;

class DialogRename : public QDialog
{
    Q_OBJECT

public:

    DialogRename(const std::string& _strName, const std::vector<std::string>& _vecExcludedNames);

    std::string GetEditedName();

private slots:

    void OnButtonConfirm();

private:

    std::string m_strEditedName;
    std::vector<std::string> m_vecExcludedNames;

    QLineEdit* m_pEditName;
};


#endif
