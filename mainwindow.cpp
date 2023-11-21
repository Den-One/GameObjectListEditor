#include "mainwindow.h"
#include "qforeach.h"
#include "ui_mainwindow.h"

#include <QGroupBox>
#include <QActionGroup>
#include <QScrollArea>
#include <QFrame>
#include <QVector>
#include <QFileDialog>

// We have no time, done better than perfect.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , editObjectForm_(new EditObjectForm())
    , creatorForm_(new FileCreatorForm())
    , editAreaLayout_(new QVBoxLayout())
    , typeAreaLayout_(new QVBoxLayout())
    , labelList_(new LabelList())
    , statusBarLabel_(new QLabel("has unsaved changes"))
{
    ui_->setupUi(this);
    ui_->labelEditArea->setAlignment(Qt::AlignCenter);
    ui_->scrollAreaForEdit->widget()->setLayout(editAreaLayout_);
    ui_->scrollAreaForTypes->widget()->setLayout(typeAreaLayout_);
    ui_->scrollAreaForTypes->widget()->layout()->setAlignment(Qt::AlignTop);
    ui_->scrollAreaForEdit->widget()->layout()->setAlignment(Qt::AlignTop);

    for (auto object : readGameObjects(QUrl(baseObjectsFileUrl_))) {
        addObjectToObjectArea(object);
    }

    connect(
        editObjectForm_->getSaveButton(), &QPushButton::clicked,
        this, &MainWindow::saveEditObjectForm
    );

    connect(
        creatorForm_->getCreateButton(), &QPushButton::clicked,
        this, &MainWindow::createFileList
    );

    editObjectForm_->addOnLayoutHidden(
        ui_->scrollAreaForEdit->widget()->layout()
    );

    labelList_->setLayout(ui_->scrollAreaForEdit->widget()->layout());

    setState(ApplicationState::START);
}

MainWindow::~MainWindow() {
    delete ui_;
    delete editObjectForm_;
    delete creatorForm_;
    delete editAreaLayout_;
    delete typeAreaLayout_;
    delete labelList_;
    delete statusBarLabel_;
}

void MainWindow::on_pushButtonCreateType_clicked() {
    setState(ApplicationState::CREATE_OBJECT);
}

void MainWindow::addTypeButtonClicked() {
    QPushButton* gameObjectButton
        = qobject_cast<QPushButton*>(QObject::sender());

    QVector<GameObject*> objects = readGameObjects(QUrl(baseObjectsFileUrl_));

    objects += runtimeGameObjects_;

    for (auto& object : objects) {
        if (object->getName() == gameObjectButton->text()) {
            labelList_->displayLine(object->getName());

            for (auto& property : object->getProperties()) {
                labelList_->displayLine(
                    property->getName() + " " + property->getDescription()
                );
            }

            labelList_->displayLine("");
            undoStack_.push(object);
        }
    }

    ui_->statusbar->addPermanentWidget(statusBarLabel_);
    doStack_.clear();

    setState(ApplicationState::CHANGE_LIST);
}

void MainWindow::on_actionOpen_List_triggered() {
    openFileToEdit_ = QFileDialog::getOpenFileName(
        this,
        tr("Open List - Game Object List Editor"),
        tr(""),
        tr("Text files (*.txt)")
    );

    if (openFileToEdit_.isEmpty()) {
        return;
    }

    if (!openFileToEdit_.isValid()) {
        throw std::runtime_error("Not valid file path");
    }

    setState(ApplicationState::VIEW_LIST);

    displayGameObjects(readGameObjects(openFileToEdit_));
}


void MainWindow::on_actionNew_List_triggered() {
    creatorForm_->show();
}

void MainWindow::on_actionSave_List_triggered() {
    if (state_ == ApplicationState::CHANGE_LIST) {
        auto list = undoStack_.toList();
        for (auto it = list.begin(), itend = list.end(); it != itend; ++it) {
            writeGameObject(openFileToEdit_, *it);
        }

        ui_->statusbar->removeWidget(statusBarLabel_);
        setState(ApplicationState::VIEW_LIST);
        displayGameObjects(readGameObjects(openFileToEdit_));
    }
}


void MainWindow::on_actionUndo_triggered() {
    if (undoStack_.empty()) {
        return;
    }

    doStack_.push(undoStack_.pop());

    labelList_->hideAll();

    displayGameObjects(readGameObjects(openFileToEdit_) + undoStack_);
}


void MainWindow::on_actionRedo_triggered() {
    if (doStack_.empty()) {
        return;
    }

    undoStack_.push(doStack_.pop());

    labelList_->hideAll();

    displayGameObjects(readGameObjects(openFileToEdit_) + undoStack_);
}


void MainWindow::saveEditObjectForm() {
    qsizetype sizeBefore = runtimeGameObjects_.size();
    editObjectForm_->saveRuntimeObjectInfo(runtimeGameObjects_);
    qsizetype sizeAfter = runtimeGameObjects_.size();

    if (sizeBefore != sizeAfter) {
        addObjectToObjectArea(runtimeGameObjects_.back());
    }

    setState(ApplicationState::START);
}

void MainWindow::addObjectToObjectArea(GameObject* object) {
    objectTypes_.push_back(new QPushButton(object->getName()));
    ui_->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes_.back());

    connect(
        objectTypes_.back(), &QPushButton::clicked,
        this, &MainWindow::addTypeButtonClicked
    );
}

void MainWindow::displayGameObjects(QVector<GameObject*>&& objects) {
    for (auto object : objects) {
        labelList_->displayLine(object->getName());
        for (auto property : object->getProperties()) {
            labelList_->displayLine(
                property->getName() + property->getDescription()
            );
        }
        labelList_->displayLine("");
    }
}

void MainWindow::createFileList() {
    QString fileName = creatorForm_->getFileName();
    creatorForm_->hide();

    QString dir = QFileDialog::getExistingDirectory(
        this, "Create List - Game Object List Editor"
    );

    openFileToEdit_ = QDir::cleanPath(dir + "\\" + fileName);

    QFile file(openFileToEdit_.path());
    file.open(QIODevice::WriteOnly);
    file.close();

    labelList_->hideAll();

    setState(ApplicationState::CHANGE_LIST);
}

void MainWindow::setState(ApplicationState newState) {
    state_ = newState;
    switch (state_) {
    case ApplicationState::START: {

        foreach (auto& button, objectTypes_) {
            button->setEnabled(false);
        }

        ui_->pushButtonCreateType->setEnabled(true);
        ui_->menubar->actions().at(0)->setEnabled(true);
        ui_->menubar->actions().at(1)->setEnabled(false);
        ui_->labelEditArea->setText(ApplicationStateName::START);

        QPalette palette = ui_->scrollAreaForEdit->widget()->palette();
        palette.setColor(QPalette::Window, QColorConstants::Svg::whitesmoke);
        ui_->scrollAreaForEdit->widget()->setPalette(palette);

        editObjectForm_->hideElements();
        labelList_->hideAll();

        undoStack_.clear();
        doStack_.clear();

        break;
    }
    case ApplicationState::CREATE_OBJECT: {

        foreach (auto& button, objectTypes_) {
            button->setEnabled(false);
        }

        ui_->pushButtonCreateType->setEnabled(false);
        ui_->menubar->actions().at(0)->setEnabled(false);
        ui_->menubar->actions().at(1)->setEnabled(false);
        ui_->labelEditArea->setText(ApplicationStateName::CREATE);

        QPalette palette = ui_->scrollAreaForEdit->widget()->palette();
        palette.setColor(QPalette::Window, QColorConstants::Svg::whitesmoke);
        ui_->scrollAreaForEdit->widget()->setPalette(palette);

        editObjectForm_->displayElements();
        labelList_->hideAll();

        undoStack_.clear();
        doStack_.clear();

        break;
    }
    case ApplicationState::VIEW_LIST: {

        foreach (auto& button, objectTypes_) {
            button->setEnabled(true);
        }

        ui_->pushButtonCreateType->setEnabled(true);
        ui_->menubar->actions().at(0)->setEnabled(true);
        ui_->menubar->actions().at(1)->setEnabled(false);
        ui_->labelEditArea->setText(ApplicationStateName::VIEW_LIST);

        QPalette palette = ui_->scrollAreaForEdit->widget()->palette();
        palette.setColor(QPalette::Window, QColorConstants::Svg::whitesmoke);
        ui_->scrollAreaForEdit->widget()->setPalette(palette);

        editObjectForm_->hideElements();
        labelList_->hideAll();

        undoStack_.clear();
        doStack_.clear();

        break;
    }
    case ApplicationState::CHANGE_LIST: {

        foreach (auto& button, objectTypes_) {
            button->setEnabled(true);
        }

        ui_->pushButtonCreateType->setEnabled(false);
        ui_->menubar->actions().at(0)->setEnabled(true);
        ui_->menubar->actions().at(1)->setEnabled(true);
        ui_->labelEditArea->setText(ApplicationStateName::CHANGE_LIST);

        QPalette palette = ui_->scrollAreaForEdit->widget()->palette();
        palette.setColor(QPalette::Window, Qt::white);
        ui_->scrollAreaForEdit->widget()->setPalette(palette);

        editObjectForm_->hideElements();

        break;
    }
    }
}
