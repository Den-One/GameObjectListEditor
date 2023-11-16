#ifndef LABELLIST_H
#define LABELLIST_H

#include <QLabel>
#include <QList>

class LabelList final {
private:
    QList<QLabel*> fileLines_;
    qsizetype size_ = 0;
    qsizetype capacity_ = 0;

    QLayout* layout_;

private:
    void allocateMemory();

public:
    LabelList();

    void setLayout(QLayout* layout);

    void displayLine(const QString& line);

    void hideAll();

    void hideInRange(qsizetype beg, qsizetype end);

    qsizetype size();
};


#endif // LABELLIST_H
