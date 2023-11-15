#include "gameobject.h"

GameObject::GameObject(QString&& name) : name_ {name} {}


void GameObject::insertProperty(QString&& name, QString&& description) {
    try {
        properties_.push_back(
            new Property(std::move(name), std::move(description))
        );
    }
    catch (std::bad_alloc& ec) {
        puts(ec.what());
    }
}


void GameObject::removeProperty(const QString& name) {
    properties_.removeIf([&name](auto elem) {
        return elem->getName() == name;
    });
}


void GameObject::removeAll() {
    return properties_.clear();
}


QList<Property*> GameObject::getProperties() {
    return properties_.toList();
}


QString GameObject::getName() {
    return name_;
}
