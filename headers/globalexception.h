#ifndef GLOBALEXCEPTION_H
#define GLOBALEXCEPTION_H

#include <QException>

class GlobalException : public QException
{
public:
    GlobalException(QString const& message) :
        message(message)
    {}

    virtual ~GlobalException()
    {

    }

    void raise() const { throw *this; }
    GlobalException *clone() const { return new GlobalException(*this); }

    QString getMessage() const;
private:
    QString message;
};

#endif // GLOBALEXCEPTION_H
