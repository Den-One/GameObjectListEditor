#ifndef FILECREATORFORM_H
#define FILECREATORFORM_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>


class FileCreatorForm final {
private:
    QWidget* windowFrame_;
    QVBoxLayout* layout_;
    QLabel* label_;
    QLineEdit* lineEdit_;
    QPushButton* createButton_;

public:
    FileCreatorForm();

    ~FileCreatorForm();

    void show();

    void hide();

    QPushButton* getCreateButton();

    QString getFileName();
};


#endif // FILECREATORFORM_H
