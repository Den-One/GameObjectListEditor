#pragma once

#include "qcoreapplication.h"
#ifndef TEMPORARY_H
#define TEMPORARY_H

namespace gole {};

#include <QStringList>
#include <QString>
#include <QUrl>
#include <QList>
#include <stdexcept>
#include <QSet>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QTextStream>

class Property final {
private:
    QString name_;
    QString description_;

public:
    //Property() = delete;

    Property(QString&& name, QString&& description) :
        name_{name}, description_{description}
    {
    }

    Property(const Property& rhs) :
        name_ {rhs.name_}, description_ {rhs.description_}
    {
    }

    Property(Property&& rhs) noexcept :
        name_ {std::move(rhs.name_)},
        description_ {std::move(rhs.description_)}
    {
    }

    Property& operator=(const Property& rhs) {
        if (this != &rhs) {
            std::copy(
                rhs.name_.begin(), rhs.name_.end(), std::back_inserter(name_)
            );

            std::copy(
                rhs.description_.begin(),
                rhs.description_.end(),
                std::back_inserter(description_)
            );
        }
        return *this;
    }

    Property& operator=(Property&& rhs) noexcept {
        if (this != &rhs) {
            name_ = std::move(rhs.name_);
            description_ = std::move(rhs.description_);
        }
        return *this;
    }

    QString getName() {
        return name_;
    }

    QString getDescription() {
        return description_;
    }
};

class GameObject final {
private:
    QString name_;
    QSet<Property*> properties_;

public:
    GameObject() = delete;

    GameObject(QString&& name) : name_ {name}
    {
    }

    void insertProperty(QString&& name, QString&& description) {
        try {
            properties_.insert(
                new Property(std::move(name), std::move(description))
            );
        }
        catch (std::bad_alloc& ec) {
            puts(ec.what());
        }
    }

    void removeProperty(const QString& name) {
        properties_.removeIf([&name](auto elem) {
            return elem->getName() == name;
        });
    }

    void removeAll() {
        return properties_.clear();
    }

    QList<Property*> getProperties() {
        return properties_.values();
    }

    QString getName() {
        return name_;
    }
};


struct LineType {
    inline static const QString OBJECT_ = "OBJECT";
    inline static const QString PROPERTY_ = "PROPERTY";
};

class LineParser final {
public:
    LineParser() {}

    void parse(const QString& fileLine, QVector<GameObject*>& objects) {
        if (fileLine == "") {
            return;
        }

        QStringList list = fileLine.split(" ");
        QString& lineType = list[0];
        QString& entityName = list[1];
        lineType.replace(" ", "");
        entityName.replace(" ", "");

        if (lineType == LineType::OBJECT_) {
            objects.push_back(new GameObject(std::move(entityName)));
        }
        else if (lineType == LineType::PROPERTY_) {
            QString description = fileLine.split(entityName)[1];
            objects.back()->insertProperty(
                std::move(entityName), std::move(description)
                );
        }
    }
};

class ObjectFileManager {
public:
    void writeGameObject(const QUrl& url, GameObject* object) {
        QFile file(url.path());
        qDebug() << "writeGameObject:" << url.path() << Qt::endl;

        if (!file.exists()) {
            throw std::runtime_error("Not valid file's url while writing");
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            throw std::runtime_error("No such file to write"); // !!!
        }
        else {
            QTextStream stream(&file);

            file.write(QString(LineType::OBJECT_ + " " + object->getName() + "\n").toStdString().c_str());
            for (auto& property : object->getProperties()) {
                file.write(QString(LineType::PROPERTY_ + " " + property->getName() + " " + property->getDescription() + "\n").toStdString().c_str());
            }
            file.write("\n");
            file.close();
        }
    }

    void writeGameObjects(const QUrl& url, const QVector<GameObject*>& objects) {
        QFile file(url.path());

        if (!file.exists()) {
            throw std::runtime_error("Not valid file's url while writing");
        }

        if (!file.open(QIODevice::Append)) {
            throw std::runtime_error("No such file to write");
        }
        else {
            QTextStream stream(&file);
            for (auto& object : objects) {
                file.write(QString(LineType::OBJECT_ + " " + object->getName()).toStdString().c_str());
                for (auto& property : object->getProperties()) {
                    file.write(QString(LineType::PROPERTY_ + " " + property->getName() + " " + property->getDescription()).toStdString().c_str());
                }
                file.write("");
            }
            file.close();
        }
    }

    QVector<GameObject*> readGameObjects(const QUrl& fileUrl) {
        QVector<GameObject*> objects;
        QFile file(fileUrl.path());

        if (!file.exists()) {
            throw std::runtime_error("Not valid file's url while reading");
        }

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
};

#endif // TEMPORARY_H
