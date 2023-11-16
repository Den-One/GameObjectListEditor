#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcoreapplication.h"
#include "rwmanager.h"

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLayout>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class EditObjectForm;
class LabelList;
class FileCreatorForm;


enum class ApplicationState : unsigned char {
    START = 0, CREATE_OBJECT = 1, VIEW_LIST = 2, CHANGE_LIST = 3
};

struct ApplicationStateName {
    static const inline QString START = "Edit Area - Start Mode";
    static const inline QString CREATE = "Edit Area - Create Mode";
    static const inline QString VIEW_LIST = "Edit Area - View List Mode";
    static const inline QString CHANGE_LIST = "Edit Area - Edit List Mode";
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveEditObjectForm();

    void createFileList();

    void addTypeButtonClicked();

    void setState(ApplicationState newState);

private slots:
    void on_pushButtonCreateType_clicked();

    void on_actionOpen_List_triggered();

    void on_actionNew_List_triggered();

    void on_actionSave_List_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    void addObjectToObjectArea(GameObject* object);

    void displayGameObjects(QVector<GameObject*>&& objects);

private:
    Ui::MainWindow *ui;

    EditObjectForm  *editObjectForm;
    FileCreatorForm *creatorForm;

    QVBoxLayout *editAreaLayout;
    QVBoxLayout *typeAreaLayout;

    LabelList* labelList;
    QVector<QPushButton*> objectTypes;
    QList<QWidget*> editAreaWidgets_;

    QUrl localPath = QCoreApplication::applicationDirPath();
    QUrl openFileToEdit;

    QVector<GameObject*> runtimeGameObjects;

    QStack<GameObject*> doStack;
    QStack<GameObject*> undoStack;

    QLabel* statusBarLabel;

    ApplicationState state_;

    const QString baseObjectsFileUrl = ":/BaseObjectTypes.txt";
};

///////////////////////////////////////////////////////////////////////////////
class LabelList final {
private:
    QList<QLabel*> fileLines_;
    qsizetype size_ = 0;
    qsizetype capacity_ = 0;

    QLayout* layout_;

private:
    void allocateMemory() {
        qsizetype i = (capacity_ == 0) ? -1 : 0;
        for (; i < capacity_; ++i) {
            QLabel* label = new QLabel();
            label->setVisible(false);
            layout_->addWidget(label);
            fileLines_.append(label);
        }
        capacity_ = fileLines_.size();
    }

public:
    LabelList() {}

    void setLayout(QLayout* layout) {
        layout_ = layout;
    }

    void displayLine(const QString& line) {
        if (size_ == capacity_) {
            allocateMemory();
        }

        fileLines_.at(size_)->setText(line);
        fileLines_.at(size_)->setVisible(true);

        ++size_;
    }

    void hideAll() {
        for (auto& line : fileLines_) {
            line->setVisible(false);
        }

        size_ = 0;
    }

    void hideInRange(qsizetype beg, qsizetype end) {
        if (end > size_) {
            end = size_;
        }

        for (qsizetype i = beg; i < end; ++i) {
            fileLines_.at(i)->setVisible(false);
        }
    }

    qsizetype size() {
        return size_;
    }
};

///////////////////////////////////////////////////////////////////////////////

class EditObjectForm final {
private:
    QLabel*    objectNameLabel;
    QLineEdit* objectNameLineEdit;

    QLabel*    property1NameLabel;
    QLineEdit* property1LineEdit;
    QLabel*    property1DescLabel;
    QTextEdit* property1TextEdit;

    QLabel*    property2NameLabel;
    QLineEdit* property2LineEdit;
    QLabel*    property2DescLabel;
    QTextEdit* property2TextEdit;

    QPushButton* saveChangesButton;

    bool isDisplayed = true;

public:
    EditObjectForm() :
        objectNameLabel    (new QLabel("Object name:")),
        objectNameLineEdit (new QLineEdit()),
        property1NameLabel (new QLabel("Property #1 name:")),
        property1LineEdit  (new QLineEdit()),
        property1DescLabel (new QLabel("Property #1 description:")),
        property1TextEdit  (new QTextEdit()),
        property2NameLabel (new QLabel("Property #1 name:")),
        property2LineEdit  (new QLineEdit()),
        property2DescLabel (new QLabel("Property #1 description:")),
        property2TextEdit  (new QTextEdit()),
        saveChangesButton  (new QPushButton("Save or quit"))
    {
    }

    ~EditObjectForm() {
        delete objectNameLabel;
        delete objectNameLineEdit;
        delete property1NameLabel;
        delete property1LineEdit;
        delete property1DescLabel;
        delete property1TextEdit;
        delete property2NameLabel;
        delete property2LineEdit;
        delete property2DescLabel;
        delete property2TextEdit;
        delete saveChangesButton;
    }

    void addOnLayoutHidden(QLayout* layout) {
        layout->addWidget(objectNameLabel);
        layout->addWidget(objectNameLineEdit);
        layout->addWidget(property1NameLabel);
        layout->addWidget(property1LineEdit);
        layout->addWidget(property1DescLabel);
        layout->addWidget(property1TextEdit);
        layout->addWidget(property2NameLabel);
        layout->addWidget(property2LineEdit);
        layout->addWidget(property2DescLabel);
        layout->addWidget(property2TextEdit);
        layout->addWidget(saveChangesButton);

        hideElements();
    }

    void displayElements() {
        if (!isDisplayed) {
            objectNameLabel->setVisible(true);
            objectNameLineEdit->setVisible(true);
            property1NameLabel->setVisible(true);
            property1LineEdit->setVisible(true);
            property1DescLabel->setVisible(true);
            property1TextEdit->setVisible(true);
            property2NameLabel->setVisible(true);
            property2LineEdit->setVisible(true);
            property2DescLabel->setVisible(true);
            property2TextEdit->setVisible(true);
            saveChangesButton->setVisible(true);

            objectNameLineEdit->clear();
            property1LineEdit->clear();
            property1TextEdit->clear();
            property2LineEdit->clear();
            property2TextEdit->clear();

            isDisplayed = true;
        }
    }

    void hideElements() {
        if (isDisplayed) {
            objectNameLabel->setVisible(false);
            objectNameLineEdit->setVisible(false);
            property1NameLabel->setVisible(false);
            property1LineEdit->setVisible(false);
            property1DescLabel->setVisible(false);
            property1TextEdit->setVisible(false);
            property2NameLabel->setVisible(false);
            property2LineEdit->setVisible(false);
            property2DescLabel->setVisible(false);
            property2TextEdit->setVisible(false);
            saveChangesButton->setVisible(false);

            isDisplayed = false;
        }
    }

    void saveRuntimeObjectInfo(QVector<GameObject*>& object) {
        if (objectNameLineEdit->text() != "") {
            objectNameLineEdit->text().replace(" ", "");
            GameObject* newObject = new GameObject(objectNameLineEdit->text());
            if (property1LineEdit->text() != "") {
                property1LineEdit->text().replace(" ", "");
                newObject->insertProperty(
                    property1LineEdit->text(), property1TextEdit->toPlainText()
                );
            }

            if (property2LineEdit->text() != "") {
                property2LineEdit->text().replace(" ", "");
                newObject->insertProperty(
                    property2LineEdit->text(), property2TextEdit->toPlainText()
                    );
            }

            object.push_back(newObject);
        }
    }

    QPushButton* getSaveButton() {
        return saveChangesButton;
    }
};

///////////////////////////////////////////////////////////////////////////////

class FileCreatorForm final {
private:
    QWidget* windowFrame_;
    QVBoxLayout* layout_;
    QLabel* label_;
    QLineEdit* lineEdit_;
    QPushButton* createButton_;

public:
    FileCreatorForm() :
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

    ~FileCreatorForm() {
        delete windowFrame_;
        delete layout_;
        delete label_;
        delete lineEdit_;
        delete createButton_;
    }

    void show() {
        lineEdit_->clear();
        lineEdit_->setText(".txt");
        lineEdit_->setCursorPosition(0);
        windowFrame_->show();
    }

    void hide() {
        windowFrame_->hide();
    }

    QPushButton* getCreateButton() {
        return createButton_;
    }

    QString getFileName() {
        return lineEdit_->text();
    }
};
///////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
