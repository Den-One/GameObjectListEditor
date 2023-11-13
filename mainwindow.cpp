#include "mainwindow.h"
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
{
    ui->setupUi(this);
    ui->scrollAreaForEdit->widget()->setLayout(editAreaLayout);
    ui->scrollAreaForTypes->widget()->setLayout(typeAreaLayout);

    updateObjectTypesArea();

    connect(editObjectForm->getSaveButton(), &QPushButton::clicked, this, &MainWindow::saveEditObjectForm);
    connect(creatorForm->getCreateButton(), &QPushButton::clicked, this, &MainWindow::createFileList);
}

MainWindow::~MainWindow() {
    delete ui;
    delete editObjectForm;
    delete creatorForm;
    delete editAreaLayout;
    delete typeAreaLayout;
    delete labelList;
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

void MainWindow::on_actionOpen_List_triggered() {
    QUrl url = QFileDialog::getOpenFileName(this, tr("Open List - Game Object List Editor"), tr(""), tr("Text files (*.txt)"));

    if (url.isEmpty()) {
        return;
    }

    QVector<GameObject*> objects = readGameObjects(url);

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

}


void MainWindow::on_actionUndo_triggered() {

}


void MainWindow::on_actionRedo_triggered() {

}


void MainWindow::saveEditObjectForm() {
    editObjectForm->saveFormInfo();
    updateObjectTypesArea();

    auto vec = readGameObjects(QUrl(":/RuntimeObjectTypes.txt"));
    for (auto v : vec) {
        qDebug() << v << "\n";
    }
}


void MainWindow::updateObjectTypesArea() {
    for (auto& typeButton : objectTypes) {
        ui->scrollAreaForTypes->widget()->layout()->removeWidget(typeButton);
        typeButton->deleteLater();
    }

    objectTypes.clear();
    objectTypes.shrink_to_fit();

    QVector<GameObject*> baseObjects = readGameObjects(QUrl(":/BaseObjectTypes.txt"));

    for (auto& object : baseObjects) {
        objectTypes.push_back(new QPushButton(object->getName()));
        ui->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes.back());
    }

    QVector<GameObject*> userObjects = readGameObjects(QUrl(":/RuntimeObjectTypes.txt"));

    for (auto& object : userObjects) {
        objectTypes.push_back(new QPushButton(object->getName()));
        ui->scrollAreaForTypes->widget()->layout()->addWidget(objectTypes.back());
    }
}

QVector<GameObject*> MainWindow::readGameObjects(const QUrl& fileUrl) {
    QVector<GameObject*> objects;
    QFile file(fileUrl.path());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("No such file to read");
    }
    else {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            LineParser{}.parse(stream.readLine(), objects);
        }

        file.close();
    }
    return objects;
}

void MainWindow::writeGameObjects(const QUrl& url, const QVector<GameObject*>& objects) {
    QFile file(url.fileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("No such file to write");
    }
    else {
        QTextStream stream(&file);
        for (auto& object : objects) {
            stream << LineType::OBJECT_ << " " << object->getName() << "\n";
            for (auto& property : object->getProperties()) {
                stream << LineType::PROPERTY_ << " "
                       << property->getName()
                       << property->getDescription() << "\n";
            }
        }
    }
}

void MainWindow::createFileList() {
    QString fileName = creatorForm->getFileName();
    creatorForm->hide();
    QUrl url = QFileDialog::getExistingDirectoryUrl();
}
