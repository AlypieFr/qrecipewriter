#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <QException>

class FileNotFoundException : public QException
{
public:
    FileNotFoundException(QString const& message) :
        message(message)
    {}

    virtual ~FileNotFoundException()
    {

    }

    void raise() const { throw *this; }
    FileNotFoundException *clone() const { return new FileNotFoundException(*this); }

    QString getMessage() const;
private:
    QString message;
};

#endif // FILENOTFOUNDEXCEPTION_H
