#ifndef DIALOGRENAMERESSOURCE_H
#define DIALOGRENAMERESSOURCE_H


#include <QtWidgets/QDialog>


class QLineEdit;

namespace gugu {
    class Resource;
}

class DialogRenameResource : public QDialog
{
    Q_OBJECT

public:

    DialogRenameResource(gugu::Resource* _pResource);

private slots:

    void OnButtonConfirm();

private:

    gugu::Resource* m_pResource;

    QLineEdit* m_pEditFolder;
    QLineEdit* m_pEditName;
    QLineEdit* m_pEditExtension;
};


#endif
