#include "editobjectform.h"


EditObjectForm::EditObjectForm() :
    objectNameLabel_    (new QLabel("Object name:")),
    objectNameLineEdit_ (new QLineEdit()),
    property1NameLabel_ (new QLabel("Property #1 name:")),
    property1LineEdit_  (new QLineEdit()),
    property1DescLabel_ (new QLabel("Property #1 description:")),
    property1TextEdit_  (new QTextEdit()),
    property2NameLabel_ (new QLabel("Property #1 name:")),
    property2LineEdit_  (new QLineEdit()),
    property2DescLabel_ (new QLabel("Property #1 description:")),
    property2TextEdit_  (new QTextEdit()),
    saveChangesButton_  (new QPushButton("Save or quit"))
{
}

EditObjectForm::~EditObjectForm() {
    delete objectNameLabel_;
    delete objectNameLineEdit_;
    delete property1NameLabel_;
    delete property1LineEdit_;
    delete property1DescLabel_;
    delete property1TextEdit_;
    delete property2NameLabel_;
    delete property2LineEdit_;
    delete property2DescLabel_;
    delete property2TextEdit_;
    delete saveChangesButton_;
}

void EditObjectForm::addOnLayoutHidden(QLayout* layout) {
    layout->addWidget(objectNameLabel_);
    layout->addWidget(objectNameLineEdit_);
    layout->addWidget(property1NameLabel_);
    layout->addWidget(property1LineEdit_);
    layout->addWidget(property1DescLabel_);
    layout->addWidget(property1TextEdit_);
    layout->addWidget(property2NameLabel_);
    layout->addWidget(property2LineEdit_);
    layout->addWidget(property2DescLabel_);
    layout->addWidget(property2TextEdit_);
    layout->addWidget(saveChangesButton_);

    hideElements();
}

void EditObjectForm::displayElements() {
    if (!isDisplayed) {
        objectNameLabel_->setVisible(true);
        objectNameLineEdit_->setVisible(true);
        property1NameLabel_->setVisible(true);
        property1LineEdit_->setVisible(true);
        property1DescLabel_->setVisible(true);
        property1TextEdit_->setVisible(true);
        property2NameLabel_->setVisible(true);
        property2LineEdit_->setVisible(true);
        property2DescLabel_->setVisible(true);
        property2TextEdit_->setVisible(true);
        saveChangesButton_->setVisible(true);

        objectNameLineEdit_->clear();
        property1LineEdit_->clear();
        property1TextEdit_->clear();
        property2LineEdit_->clear();
        property2TextEdit_->clear();

        isDisplayed = true;
    }
}

void EditObjectForm::hideElements() {
    if (isDisplayed) {
        objectNameLabel_->setVisible(false);
        objectNameLineEdit_->setVisible(false);
        property1NameLabel_->setVisible(false);
        property1LineEdit_->setVisible(false);
        property1DescLabel_->setVisible(false);
        property1TextEdit_->setVisible(false);
        property2NameLabel_->setVisible(false);
        property2LineEdit_->setVisible(false);
        property2DescLabel_->setVisible(false);
        property2TextEdit_->setVisible(false);
        saveChangesButton_->setVisible(false);

        isDisplayed = false;
    }
}

void EditObjectForm::saveRuntimeObjectInfo(QVector<GameObject*>& object) {
    if (objectNameLineEdit_->text() != "") {
        objectNameLineEdit_->text().replace(" ", "");
        GameObject* newObject = new GameObject(objectNameLineEdit_->text());
        if (property1LineEdit_->text() != "") {
            property1LineEdit_->text().replace(" ", "");
            newObject->insertProperty(
                property1LineEdit_->text(), property1TextEdit_->toPlainText()
                );
        }

        if (property2LineEdit_->text() != "") {
            property2LineEdit_->text().replace(" ", "");
            newObject->insertProperty(
                property2LineEdit_->text(), property2TextEdit_->toPlainText()
                );
        }

        object.push_back(newObject);
    }
}

QPushButton* EditObjectForm::getSaveButton() {
    return saveChangesButton_;
}
