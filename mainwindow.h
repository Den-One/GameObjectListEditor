#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rwmanager.h"
#include "labellist.h"
#include "filecreatorform.h"
#include "editobjectform.h"

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

using listEdit::Property;
using listEdit::GameObject;
using listEdit::readGameObjects;
using listEdit::writeGameObject;

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
    Ui::MainWindow* ui_;

    EditObjectForm  *editObjectForm_;
    FileCreatorForm *creatorForm_;

    QVBoxLayout *editAreaLayout_;
    QVBoxLayout *typeAreaLayout_;

    LabelList* labelList_;
    QVector<QPushButton*> objectTypes_;
    QList<QWidget*> editAreaWidgets_;

    QUrl openFileToEdit_;

    QVector<GameObject*> runtimeGameObjects_;

    QStack<GameObject*> doStack_;
    QStack<GameObject*> undoStack_;

    QLabel* statusBarLabel_;

    ApplicationState state_;

    const QString baseObjectsFileUrl_ = ":/BaseObjectTypes.txt";
};


#endif // MAINWINDOW_H
