#ifndef PROPERTY_H
#define PROPERTY_H

#include <QString>

namespace listEdit {

    class Property final {
    private:
        QString name_;
        QString description_;

    public:
        //Property() = delete;

        Property(QString&& name, QString&& description);

        Property(const Property& rhs);

        Property(Property&& rhs) noexcept;

        Property& operator=(const Property& rhs);

        Property& operator=(Property&& rhs) noexcept;

        QString getName();

        QString getDescription();
    };

}; // namespace listEdit


#endif // PROPERTY_H
