#include "filecreatorform.h"


FileCreatorForm::FileCreatorForm() :
    windowFrame_(new QWidget()),
    layout_(new QVBoxLayout()),
    label_(new QLabel("File name:")),
    lineEdit_(new QLineEdit()),
    createButton_(new QPushButton("Create"))
{
    windowFrame_->setWindowTitle("New List - Game Object List Editor");
    windowFrame_->setFixedSize(355, 100);
    windowFrame_->setLayout(layout_);
    windowFrame_->layout()->addWidget(label_);
    windowFrame_->layout()->addWidget(lineEdit_);
    windowFrame_->layout()->addWidget(createButton_);
}


FileCreatorForm::~FileCreatorForm() {
    delete windowFrame_;
    delete layout_;
    delete label_;
    delete lineEdit_;
    delete createButton_;
}


void FileCreatorForm::show() {
    lineEdit_->clear();
    lineEdit_->setText(".txt");
    lineEdit_->setCursorPosition(0);
    windowFrame_->show();
}


void FileCreatorForm::hide() {
    windowFrame_->hide();
}


QPushButton* FileCreatorForm::getCreateButton() {
    return createButton_;
}


QString FileCreatorForm::getFileName() {
    return lineEdit_->text();
}
