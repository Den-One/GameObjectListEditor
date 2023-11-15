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

    for (auto object : readGameObjects(QUrl(baseObjectsFileUrl))) {
        addObjectToObjectArea(object);
    }

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

void MainWindow::addTypeButtonClicked() {
    QPushButton* gameObjectButton
        = qobject_cast<QPushButton*>(QObject::sender());

    QVector<GameObject*> objects = readGameObjects(QUrl(baseObjectsFileUrl));

    objects += runtimeGameObjects;

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
    doStack.clear();

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

    setState(ApplicationState::VIEW_LIST);

    displayGameObjects(readGameObjects(openFileToEdit));
}


void MainWindow::on_actionNew_List_triggered() {
    creatorForm->show();
}

void MainWindow::on_actionSave_List_triggered() {
    auto list = undoStack.toList();
    for (auto it = list.begin(), itend = list.end(); it != itend; ++it) {
        writeGameObject(openFileToEdit, *it);
    }

    ui->statusbar->removeWidget(statusBarLabel);

    setState(ApplicationState::VIEW_LIST);

    displayGameObjects(readGameObjects(openFileToEdit));
}


void MainWindow::on_actionUndo_triggered() {
    if (undoStack.empty()) {
        return;
    }

    doStack.push(undoStack.pop());

    labelList->hideAll();

    displayGameObjects(readGameObjects(openFileToEdit) + undoStack);
}


void MainWindow::on_actionRedo_triggered() {
    if (doStack.empty()) {
        return;
    }

    undoStack.push(doStack.pop());

    labelList->hideAll();

    displayGameObjects(readGameObjects(openFileToEdit) + undoStack);
}


void MainWindow::saveEditObjectForm() {
    editObjectForm->saveRuntimeObjectInfo(runtimeGameObjects);
    addObjectToObjectArea(runtimeGameObjects.back());
    setState(ApplicationState::START);
}

void MainWindow::addObjectToObjectArea(GameObject* object) {
    objectTypes.push_back(new QPushButton(object->getName()));
    ui->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes.back());
    connect(objectTypes.back(), &QPushButton::clicked, this, &MainWindow::addTypeButtonClicked);
}

void MainWindow::displayGameObjects(QVector<GameObject*>&& objects) {
    for (auto object : objects) {
        labelList->displayLine(object->getName());
        for (auto property : object->getProperties()) {
            labelList->displayLine(property->getName() + property->getDescription());
        }
        labelList->displayLine("");
    }
}

void MainWindow::createFileList() {
    QString fileName = creatorForm->getFileName();
    creatorForm->hide();

    QString dir = QFileDialog::getExistingDirectory(this, "Create List - Game Object List Editor");
    openFileToEdit = QDir::cleanPath(dir + "\\" + fileName);

    QFile file(openFileToEdit.path());
    file.open(QIODevice::WriteOnly);
    file.close();

    labelList->hideAll();

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

        undoStack.clear();
        doStack.clear();

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

        undoStack.clear();
        doStack.clear();

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

        undoStack.clear();
        doStack.clear();

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
