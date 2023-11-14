#include "property.h"

Property::Property(QString&& name, QString&& description) :
    name_{name}, description_{description}
{
}

Property::Property(const Property& rhs) :
    name_ {rhs.name_}, description_ {rhs.description_}
{
}

Property::Property(Property&& rhs) noexcept :
    name_ {std::move(rhs.name_)},
    description_ {std::move(rhs.description_)}
{
}

Property& Property::operator=(const Property& rhs) {
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

Property& Property::operator=(Property&& rhs) noexcept {
    if (this != &rhs) {
        name_ = std::move(rhs.name_);
        description_ = std::move(rhs.description_);
    }
    return *this;
}

QString Property::getName() {
    return name_;
}

QString Property::getDescription() {
    return description_;
}
