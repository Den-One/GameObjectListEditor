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

    editObjectForm->addOnLayoutHidden(ui->scrollAreaForEdit->widget()->layout());
    labelList->setLayout(ui->scrollAreaForEdit->widget()->layout());

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
    setState(ApplicationState::CREATE_OBJECT);
}

void MainWindow::objectTypeButtonClicked() {
    QPushButton* gameObjectButton
        = qobject_cast<QPushButton*>(QObject::sender());

    QVector<GameObject*> objects
        = ObjectFileManager{}.readGameObjects(QUrl(":/BaseObjectTypes.txt"));

    objects += ObjectFileManager{}.readGameObjects(
        QUrl(QDir::cleanPath(localPath.toString() +"\\"+ runtimeSaveFileName))
    );

    for (auto& object : objects) {
        if (object->getName() == gameObjectButton->text()) {
            labelList->displayLine(object->getName());

            for (auto& property : object->getProperties()) {
                labelList->displayLine(
                    property->getName() + " " + property->getDescription()
                );
            }

            labelList->displayLine("");
            undoStack.push(object);
        }
    }

    ui->statusbar->addPermanentWidget(statusBarLabel);

    setState(ApplicationState::CHANGE_LIST);
}

void MainWindow::on_actionOpen_List_triggered() {
    openFileToEdit = QFileDialog::getOpenFileName(
        this,
        tr("Open List - Game Object List Editor"),
        tr(""),
        tr("Text files (*.txt)")
    );

    if (openFileToEdit.isEmpty()) {
        return;
    }

    if (!openFileToEdit.isValid()) {
        throw std::runtime_error("Not valid file path");
    }

    QVector<GameObject*> objects =
        ObjectFileManager{}.readGameObjects(openFileToEdit);

    setState(ApplicationState::VIEW_LIST);

    for (auto& object : objects) {
        labelList->displayLine(object->getName());
        for (auto& property : object->getProperties()) {
            labelList->displayLine(property->getName() + property->getDescription());
        }
        labelList->displayLine("");
    }
}


void MainWindow::on_actionNew_List_triggered() {
    creatorForm->show();
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
    auto gameObject = undoStack.pop();
    qsizetype end = labelList->size();
    // 2 - additional object name line and empty line
    qsizetype beg = end - gameObject->getProperties().size() - 2;
    labelList->hideInRange(beg, end);

    doStack.push(gameObject);
}


void MainWindow::on_actionRedo_triggered() {

}


void MainWindow::saveEditObjectForm() {
    editObjectForm->saveFormInfo(QDir::cleanPath(localPath.toString() + "\\" + runtimeSaveFileName));
    updateObjectTypesArea();

    setState(ApplicationState::START);
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

    setState(ApplicationState::CHANGE_LIST);
}

void MainWindow::setState(ApplicationState newState) {
    state_ = newState;
    switch (state_) {
    case ApplicationState::START: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(false);
        }

        ui->pushButtonCreateType->setEnabled(true);
        ui->menubar->actions()[0]->setEnabled(true);
        ui->menubar->actions()[1]->setEnabled(false);

        editObjectForm->hideElements();
        labelList->hideAll();

        break;
    }
    case ApplicationState::CREATE_OBJECT: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(false);
        }

        ui->pushButtonCreateType->setEnabled(false);
        ui->menubar->actions()[0]->setEnabled(false);
        ui->menubar->actions()[1]->setEnabled(false);

        editObjectForm->displayElements();
        labelList->hideAll();

        break;
    }
    case ApplicationState::VIEW_LIST: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(true);
        }

        ui->pushButtonCreateType->setEnabled(true);
        ui->menubar->actions()[0]->setEnabled(true);
        ui->menubar->actions()[1]->setEnabled(false);

        editObjectForm->hideElements();
        labelList->hideAll();

        break;
    }
    case ApplicationState::CHANGE_LIST: {

        foreach (auto& button, objectTypes) {
            button->setEnabled(true);
        }

        ui->pushButtonCreateType->setEnabled(false);
        ui->menubar->actions()[0]->setEnabled(true);
        ui->menubar->actions()[1]->setEnabled(true);

        editObjectForm->hideElements();

        break;
    }
    }
}
