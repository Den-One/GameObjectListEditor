#include "labellist.h"

#include <QLayout>


LabelList::LabelList() {}


void LabelList::allocateMemory() {
    qsizetype i = (capacity_ == 0) ? -1 : 0;
    for (; i < capacity_; ++i) {
        QLabel* label = new QLabel();
        label->setVisible(false);
        layout_->addWidget(label);
        fileLines_.append(label);
    }
    capacity_ = fileLines_.size();
}


void LabelList::setLayout(QLayout* layout) {
    layout_ = layout;
}


void LabelList::displayLine(const QString& line) {
    if (size_ == capacity_) {
        allocateMemory();
    }

    fileLines_.at(size_)->setText(line);
    fileLines_.at(size_)->setVisible(true);

    ++size_;
}


void LabelList::hideAll() {
    for (auto& line : fileLines_) {
        line->setVisible(false);
    }

    size_ = 0;
}


void LabelList::hideInRange(qsizetype beg, qsizetype end) {
    if (end > size_) {
        end = size_;
    }

    for (qsizetype i = beg; i < end; ++i) {
        fileLines_.at(i)->setVisible(false);
    }
}


qsizetype LabelList::size() {
    return size_;
}
