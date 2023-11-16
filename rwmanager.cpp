#include "rwmanager.h"

#include <QFile>
#include <QDebug>

namespace listEdit {

    struct LineType {
        inline static const QString OBJECT_ = "OBJECT";
        inline static const QString PROPERTY_ = "PROPERTY";
    };


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


    void writeGameObject(const QUrl& url, GameObject* object) {
        QFile file(url.path());
        qDebug() << "writeGameObject:" << url.path() << Qt::endl;

        if (!file.exists()) {
            throw std::runtime_error("Not valid file's url while writing");
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            throw std::runtime_error("No such file to write");
        }
        else {
            QTextStream stream(&file);

            file.write(QString(
                           LineType::OBJECT_ +
                           " " +
                           object->getName() + "\n").toStdString().c_str()
            );

            for (auto& property : object->getProperties()) {
                file.write(QString(
                               LineType::PROPERTY_ +
                               " " +
                               property->getName() +
                               " " +
                               property->getDescription() +
                               "\n").toStdString().c_str()
                );
            }

            file.write("\n");
            file.close();
        }
    }


    void writeGameObjects(const QUrl& url,
                          const QVector<GameObject*>& objects)
    {
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
                file.write(QString(
                               LineType::OBJECT_ +
                               " " + object->getName()).toStdString().c_str()
                );

                for (auto& property : object->getProperties()) {
                    file.write(QString(LineType::PROPERTY_ +
                                       " " + property->getName() +
                                       " " + property->getDescription()
                                       ).toStdString().c_str()
                    );
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
                parse(stream.readLine(), objects);
            }

            file.close();
        }
        return objects;
    }

}; // namespace listEdit
