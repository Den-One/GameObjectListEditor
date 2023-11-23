#ifndef EDITOBJECTFORM_H
#define EDITOBJECTFORM_H

#include "gameobject.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLayout>

using listEdit::GameObject;

class EditObjectForm final : public QObject {
    Q_OBJECT

private:
    QLabel*    objectNameLabel_;
    QLineEdit* objectNameLineEdit_;

    QPushButton* saveChangesButton_;
    QPushButton* addPropertyButton_;

    QList<QLabel*> nameLabelList_;
    QList<QLineEdit*> lineEditList_;
    QList<QLabel*> descLabelList_;
    QList<QTextEdit*> descTextEditList_;

    QLayout* layout_;

    bool isWidgetsAdded = false;
    bool isDisplayed = true;

private slots:
    void addPropertyForm();

private:
    void clearPropertyForms();

public:
    EditObjectForm() = delete;

    EditObjectForm(QObject *parent = nullptr);

    ~EditObjectForm();

    void addOnLayoutHidden(QLayout* layout);

    void displayElements();

    void hideElements();

    void saveRuntimeObjectInfo(QVector<GameObject*>& object);

    QPushButton* getSaveButton() const;

    QString getObjectName() const;

    qsizetype countProperties() const;
};


#endif // EDITOBJECTFORM_H
