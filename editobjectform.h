#ifndef EDITOBJECTFORM_H
#define EDITOBJECTFORM_H

#include "gameobject.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLayout>

using listEdit::GameObject;

class EditObjectForm final {
private:
    QLabel*    objectNameLabel_;
    QLineEdit* objectNameLineEdit_;

    QLabel*    property1NameLabel_;
    QLineEdit* property1LineEdit_;
    QLabel*    property1DescLabel_;
    QTextEdit* property1TextEdit_;

    QLabel*    property2NameLabel_;
    QLineEdit* property2LineEdit_;
    QLabel*    property2DescLabel_;
    QTextEdit* property2TextEdit_;

    QPushButton* saveChangesButton_;

    bool isDisplayed = true;

public:
    EditObjectForm();

    ~EditObjectForm();

    void addOnLayoutHidden(QLayout* layout);

    void displayElements();

    void hideElements();

    void saveRuntimeObjectInfo(QVector<GameObject*>& object);

    QPushButton* getSaveButton();
};


#endif // EDITOBJECTFORM_H
