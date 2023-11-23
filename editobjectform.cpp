#include "editobjectform.h"


EditObjectForm::EditObjectForm(QObject* parent) :
    QObject             (parent),
    objectNameLabel_    (new QLabel("Object name:")),
    objectNameLineEdit_ (new QLineEdit()),
    saveChangesButton_  (new QPushButton("Save or quit")),
    addPropertyButton_  (new QPushButton("+1 Property"))
{
    connect(
        addPropertyButton_, &QPushButton::clicked,
        this, &EditObjectForm::addPropertyForm
    );
}

EditObjectForm::~EditObjectForm() {
    delete objectNameLabel_;
    delete objectNameLineEdit_;
    delete saveChangesButton_;
    delete addPropertyButton_;
}

void EditObjectForm::addOnLayoutHidden(QLayout* layout) {
    layout_ = layout;

    if (!isWidgetsAdded) {
        layout_->addWidget(objectNameLabel_);
        layout_->addWidget(objectNameLineEdit_);
        layout_->addWidget(saveChangesButton_);
        layout_->addWidget(addPropertyButton_);

        isWidgetsAdded = true;
    }

    hideElements();
}

void EditObjectForm::addPropertyForm() {
    nameLabelList_.append(new QLabel("Property name:"));
    lineEditList_.append(new QLineEdit());
    descLabelList_.append(new QLabel("Property description:"));
    descTextEditList_.append(new QTextEdit());

    layout_->addWidget(nameLabelList_.back());
    layout_->addWidget(lineEditList_.back());
    layout_->addWidget(descLabelList_.back());
    layout_->addWidget(descTextEditList_.back());
}

void EditObjectForm::clearPropertyForms() {
    for (qsizetype i = 0; i < lineEditList_.size(); ++i) {
        nameLabelList_[i]->deleteLater();
        lineEditList_[i]->deleteLater();
        descLabelList_[i]->deleteLater();
        descTextEditList_[i]->deleteLater();
    }

    nameLabelList_.clear();
    lineEditList_.clear();
    descLabelList_.clear();
    descTextEditList_.clear();
}

void EditObjectForm::displayElements() {
    if (!isDisplayed) {
        objectNameLabel_->setVisible(true);
        objectNameLineEdit_->setVisible(true);
        saveChangesButton_->setVisible(true);
        addPropertyButton_->setVisible(true);

        isDisplayed = true;
    }
}

void EditObjectForm::hideElements() {
    if (isDisplayed) {
        objectNameLabel_->setVisible(false);
        objectNameLineEdit_->setVisible(false);
        saveChangesButton_->setVisible(false);
        addPropertyButton_->setVisible(false);

        isDisplayed = false;
    }
}

void EditObjectForm::saveRuntimeObjectInfo(QVector<GameObject*>& object) {
    if (objectNameLineEdit_->text() != "" && lineEditList_.size() > 0 && lineEditList_[0]->text() != "") {
        objectNameLineEdit_->text().replace(" ", "");
        GameObject* newObject = new GameObject(objectNameLineEdit_->text());

        for (qsizetype i = 0; i < lineEditList_.size(); ++i) {
            if (lineEditList_[i]->text() != "" && descTextEditList_[i]->toPlainText() != "") {
                newObject->insertProperty(
                    lineEditList_[i]->text(), descTextEditList_[i]->toPlainText()
                );
            }
        }

        object.push_back(newObject);
    }

    clearPropertyForms();
}

QPushButton* EditObjectForm::getSaveButton() const {
    return saveChangesButton_;
}

QString EditObjectForm::getObjectName() const {
    return objectNameLineEdit_->text();
}

qsizetype EditObjectForm::countProperties() const {
    return lineEditList_.count();
}
