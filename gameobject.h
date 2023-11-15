#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "property.h"

#include <QList>

class GameObject final {
private:
    QString name_;
    QVector<Property*> properties_; // it should be set, looks like it's unordered

public:
    GameObject() = delete;

    GameObject(QString&& name);

    void insertProperty(QString&& name, QString&& description);

    void removeProperty(const QString& name);

    void removeAll();

    QList<Property*> getProperties();

    QString getName();
};


#endif // GAMEOBJECT_H
