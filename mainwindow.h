#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "temporary.h"

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
class EditObjectForm;
class LabelList;
class FileCreatorForm;

enum class EditAreaState : unsigned char {
    NONE                     = 0,
    EDIT_OBJECT_TYPE         = 1,
    DISPAY_OBJECTS_LIST_FILE = 2,
    SIZE                     = 3
};
///////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveEditObjectForm();
    void createFileList();

private slots:
    void on_pushButtonCreateType_clicked();

    void on_actionOpen_List_triggered();

    void on_actionNew_List_triggered();

    void on_actionSave_List_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    void updateObjectTypesArea();
    QVector<GameObject*> readGameObjects(const QUrl& fileUrl);
    void writeGameObjects(const QUrl& url, const QVector<GameObject*>& objects);

private:
    Ui::MainWindow *ui;

    EditObjectForm  *editObjectForm;
    FileCreatorForm *creatorForm;

    QVBoxLayout *editAreaLayout;
    QVBoxLayout *typeAreaLayout;

    EditAreaState editAreaState = EditAreaState::NONE;

    LabelList* labelList;

    QVector<QPushButton*> objectTypes;
};

///////////////////////////////////////////////////////////////////////////////
class LabelList final {
private:
    QList<QLabel*> fileLines;

public:
    LabelList() {}

    void displayLine(QLayout* layout, const QString& line) {
        QLabel* label = new QLabel(line);
        layout->addWidget(label);
        fileLines.append(label);
    }

    void deleteAll(QLayout* layout) {
        for (auto& line : fileLines) {
            layout->removeWidget(line);
            fileLines.removeOne(line);
            line->deleteLater();
        }
    }
};

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
        saveChangesButton  (new QPushButton("Save"))
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

    void displayElements(QLayout* layout) {
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
    }

    void hideElements(QLayout* layout) {
        layout->removeWidget(objectNameLabel);
        layout->removeWidget(objectNameLineEdit);
        layout->removeWidget(property1NameLabel);
        layout->removeWidget(property1LineEdit);
        layout->removeWidget(property1DescLabel);
        layout->removeWidget(property1TextEdit);
        layout->removeWidget(property2NameLabel);
        layout->removeWidget(property2LineEdit);
        layout->removeWidget(property2DescLabel);
        layout->removeWidget(property2TextEdit);
        layout->removeWidget(saveChangesButton);

        objectNameLabel->deleteLater();
        objectNameLineEdit->deleteLater();
        property1NameLabel->deleteLater();
        property1LineEdit->deleteLater();
        property1DescLabel->deleteLater();
        property1TextEdit->deleteLater();
        property2NameLabel->deleteLater();
        property2LineEdit->deleteLater();
        property2DescLabel->deleteLater();
        property2TextEdit->deleteLater();
        saveChangesButton->deleteLater();
    }

    void saveFormInfo() {
        if (objectNameLineEdit->text() != "") {
            GameObject* obj = new GameObject(objectNameLineEdit->text());
            if (property1LineEdit->text() != "") {
                obj->insertProperty(property1LineEdit->text(), property1TextEdit->toPlainText());
            }

            if (property2LineEdit->text() != "") {
                obj->insertProperty(property2LineEdit->text(), property2TextEdit->toPlainText());
            }

            ObjectFileManager{}.writeGameObject(QUrl(":/RuntimeObjectTypes.txt"), obj);
        }
    }

    QPushButton* getSaveButton() {
        return saveChangesButton;
    }
};

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
        windowFrame_->setFixedSize(400, 100);
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
