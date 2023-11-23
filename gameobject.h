#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "property.h"

#include <QList>

namespace listEdit {

    class GameObject final {
    private:
        QString name_;
        QVector<Property*> properties_;

    public:
        GameObject() = delete;

        GameObject(QString&& name);

        void insertProperty(QString&& name, QString&& description);

        void removeProperty(const QString& name);

        void removeAll();

        QList<Property*> getProperties();

        QString getName() const;
    };

}; // namespace listEdit

#endif // GAMEOBJECT_H
