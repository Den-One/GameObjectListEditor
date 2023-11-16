#ifndef RWMANAGER_H
#define RWMANAGER_H

#include "gameobject.h"

#include <QUrl>

namespace listEdit {

    void writeGameObject(const QUrl& url, GameObject* object);

    void writeGameObjects(const QUrl& url,
                          const QVector<GameObject*>& objects);

    QVector<GameObject*> readGameObjects(const QUrl& fileUrl);

}; // namespace listEdit

#endif // RWMANAGER_H
