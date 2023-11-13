#include "mainwindow.h"
#include "qforeach.h"
#include "ui_mainwindow.h"

#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QVector>
#include <QFileDialog>

// We have no time, done better than perfect.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , editObjectForm(new EditObjectForm())
    , creatorForm(new FileCreatorForm())
    , editAreaLayout(new QVBoxLayout())
    , typeAreaLayout(new QVBoxLayout())
    , labelList(new LabelList())
    , statusBarLabel(new QLabel("has unsaved changes"))
{
    ui->setupUi(this);
    ui->scrollAreaForEdit->widget()->setLayout(editAreaLayout);
    ui->scrollAreaForTypes->widget()->setLayout(typeAreaLayout);

    QString runtimeFileAddress = QDir::cleanPath(localPath.toString() + "\\" + runtimeSaveFileName);
    QFile file(runtimeFileAddress);

    file.open(QIODeviceBase::ReadOnly); // creates file if doesn't exists
    file.close();

    file.resize(0); // if already existed, cleans the content

    updateObjectTypesArea();

    connect(editObjectForm->getSaveButton(), &QPushButton::clicked, this, &MainWindow::saveEditObjectForm);
    connect(creatorForm->getCreateButton(), &QPushButton::clicked, this, &MainWindow::createFileList);

    setState(ApplicationState::START);
}

MainWindow::~MainWindow() {
    delete ui;
    delete editObjectForm;
    delete creatorForm;
    delete editAreaLayout;
    delete typeAreaLayout;
    delete labelList;
    delete statusBarLabel;
}

void MainWindow::on_pushButtonCreateType_clicked() {
    if (editAreaState == EditAreaState::EDIT_OBJECT_TYPE) {
        return;
    }
    else if (editAreaState == EditAreaState::NONE) {
        editObjectForm->displayElements(editAreaLayout);
        editAreaState = EditAreaState::EDIT_OBJECT_TYPE;
    }
    else if (editAreaState == EditAreaState::DISPAY_OBJECTS_LIST_FILE) {
        labelList->deleteAll(ui->scrollAreaForEdit->widget()->layout());
        editObjectForm->displayElements(editAreaLayout);
        editAreaState = EditAreaState::DISPAY_OBJECTS_LIST_FILE;
    }
}

void MainWindow::objectTypeButtonClicked() {
    QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());
    QString objectName = sender->text();

    QVector<GameObject*> objects = ObjectFileManager{}.readGameObjects(QUrl(":/BaseObjectTypes.txt"));
    objects += ObjectFileManager{}.readGameObjects(QUrl(QDir::cleanPath(localPath.toString() + "\\" + runtimeSaveFileName)));

    for (auto& object : objects) {
        if (object->getName() == objectName) {
            ui->scrollAreaForEdit->widget()->layout()->addWidget(new QLabel(object->getName()));
            for (auto& property : object->getProperties()) {
                ui->scrollAreaForEdit->widget()->layout()->addWidget(new QLabel(property->getName() + " " + property->getDescription()));
            }
            ui->scrollAreaForEdit->widget()->layout()->addWidget(new QLabel());
        }
    }

    ui->statusbar->addPermanentWidget(statusBarLabel);
}

void MainWindow::on_actionOpen_List_triggered() {
    QUrl url = QFileDialog::getOpenFileName(this, tr("Open List - Game Object List Editor"), tr(""), tr("Text files (*.txt)"));
    openFileToEdit = url;
    if (!url.isValid()) {
        throw std::runtime_error("Not valid file path");
    }

    if (url.isEmpty()) {
        return;
    }

    QVector<GameObject*> objects = ObjectFileManager{}.readGameObjects(url);

    if (editAreaState == EditAreaState::NONE) {
        editAreaState = EditAreaState::DISPAY_OBJECTS_LIST_FILE;
    }
    else if (editAreaState == EditAreaState::EDIT_OBJECT_TYPE) {
        editObjectForm->hideElements(editAreaLayout);
        editAreaState = EditAreaState::DISPAY_OBJECTS_LIST_FILE;
    }

    for (auto& object : objects) {
        labelList->displayLine(ui->scrollAreaForEdit->widget()->layout(), object->getName());
        for (auto& property : object->getProperties()) {
            labelList->displayLine(ui->scrollAreaForEdit->widget()->layout(), property->getName() + property->getDescription());
        }
        labelList->displayLine(ui->scrollAreaForEdit->widget()->layout(), "");
    }
}


void MainWindow::on_actionNew_List_triggered() {
    if (editAreaState == EditAreaState::NONE) {

        creatorForm->show();

        editAreaState = EditAreaState::DISPAY_OBJECTS_LIST_FILE; // ??
    }
}

void MainWindow::on_actionSave_List_triggered() {
    QFile file(openFileToEdit.path());

    if (file.open(QIODeviceBase::WriteOnly)) {
        for (int i = 0; i < ui->scrollAreaForEdit->widget()->layout()->count(); ++i) {
            QLabel* label = qobject_cast<QLabel*>(ui->scrollAreaForEdit->widget()->layout()->itemAt(i)->widget());
            file.write(label->text().toStdString().c_str());
        }
        file.close();
    }

    ui->statusbar->removeWidget(statusBarLabel);
}


void MainWindow::on_actionUndo_triggered() {

}


void MainWindow::on_actionRedo_triggered() {

}


void MainWindow::saveEditObjectForm() {
    editObjectForm->saveFormInfo(QDir::cleanPath(localPath.toString() + "\\" + runtimeSaveFileName));
    updateObjectTypesArea();
}


void MainWindow::updateObjectTypesArea() {
    for (auto typeButton : objectTypes) {
        ui->scrollAreaForTypes->widget()->layout()->removeWidget(typeButton);
        typeButton->deleteLater();
    }

    for (auto& button : objectTypes) {
        disconnect(button, &QPushButton::clicked, this, &MainWindow::objectTypeButtonClicked);
    }

    objectTypes.clear();
    objectTypes.shrink_to_fit();

    QVector<GameObject*> baseObjects = ObjectFileManager{}.readGameObjects(QUrl(":/BaseObjectTypes.txt"));

    for (auto& object : baseObjects) {
        objectTypes.push_back(new QPushButton(object->getName()));
        ui->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes.back());
    }

    QString saveFileFullPath = QDir::cleanPath(localPath.toString() + "\\" + runtimeSaveFileName);
    QVector<GameObject*> userObjects = ObjectFileManager{}.readGameObjects(QUrl(saveFileFullPath));

    for (auto& object : userObjects) {
        objectTypes.push_back(new QPushButton(object->getName()));
        ui->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes.back());
    }

    for (auto& button : objectTypes) {
        connect(button, &QPushButton::clicked, this, &MainWindow::objectTypeButtonClicked);
    }
}

void MainWindow::createFileList() {
    QString fileName = creatorForm->getFileName();
    creatorForm->hide();

    QUrl url = QFileDialog::getExistingDirectoryUrl(this, "Create List - Game Object List Editor");
    newFilePath = QDir::cleanPath(url.toString() + "\\" + fileName);
    openFileToEdit = QDir::cleanPath(url.toString() + "\\" + fileName);

    QFile newFile(newFilePath);
    newFile.open(QIODeviceBase::ReadOnly); // if doesn't exist, creates a new file
    newFile.close();

    // emit some signal to clear the edit Area
}

void MainWindow::setState(ApplicationState newState) {
    /*
     *          start   create  view    change
     * start            1       1       1
     * create   1
     * view             1               1
     * change                   1
     *
     */

    state_ = newState;
    switch (state_) {
    case ApplicationState::START: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(false);
        }

        ui->pushButtonCreateType->setEnabled(true);
        ui->menubar->actions()[1]->setEnabled(false);

        break;
    }
    case ApplicationState::CREATE_OBJECT: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(false);
        }

        ui->pushButtonCreateType->setEnabled(false);
        ui->menubar->actions()[1]->setEnabled(false);

        break;
    }
    case ApplicationState::VIEW_LIST: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(true);
        }

        ui->pushButtonCreateType->setEnabled(true);
        ui->menubar->actions()[1]->setEnabled(false);

        break;
    }
    case ApplicationState::CHANGE_LIST: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(true);
        }

        ui->pushButtonCreateType->setEnabled(false);
        ui->menubar->actions()[1]->setEnabled(true);

        break;
    }
    }
}
