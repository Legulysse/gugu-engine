#ifndef DIALOGGENERATEANIMRANGE_H
#define DIALOGGENERATEANIMRANGE_H


#include <QtWidgets/QDialog>


class QLineEdit;
class QComboBox;

namespace gugu
{
    class ImageSet;
    class SubImage;
}

class DialogGenerateAnimRange : public QDialog
{
    Q_OBJECT

public:

    DialogGenerateAnimRange(gugu::ImageSet* _pImageSet);

    gugu::ImageSet* GetImageSet();
    gugu::SubImage* GetFrom();
    gugu::SubImage* GetTo();

private slots:

    void OnImageSetChanged  (int _iIndex);
    void OnButtonConfirm    ();

private:

    gugu::ImageSet* m_pImageSet;

    QComboBox* m_pBoxImageSet;
    QComboBox* m_pBoxSubImageFrom;
    QComboBox* m_pBoxSubImageTo;
};


#endif
