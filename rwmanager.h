#ifndef RWMANAGER_H
#define RWMANAGER_H

#include "gameobject.h"

#include <QUrl>

namespace gole {};

void writeGameObject(const QUrl& url, GameObject* object);
void writeGameObjects(const QUrl& url, const QVector<GameObject*>& objects);
QVector<GameObject*> readGameObjects(const QUrl& fileUrl);

#endif // RWMANAGER_H
