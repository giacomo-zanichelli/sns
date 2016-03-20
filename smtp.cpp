/*
Copyright (c) 2013 Raivis Strogonovs

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "smtp.h"

Smtp::Smtp()
{    
    is_ready = false;
}

Smtp::~Smtp()
{
}

void Smtp::setParams(       const QString &user, const QString &pass,
                            const QString &host, int port)
{
    this->user = user;
    this->pass = pass;

    this->host = host;
    this->port = port;
    is_ready = true;    
}

bool Smtp::sendMessage(const QVector<QString>& recipients, const QString& sub, const QString& cont)
{
    using namespace Poco::Net;
    using namespace Poco;
    using namespace std;

    bool success = false;

    if(is_ready) {
        std::vector<MailRecipient> to;
        string from = user.toStdString();
        string subject = MailMessage::encodeWord(sub.toStdString(), "UTF-8");
        string content = cont.toStdString();

        message.setSender(from);
        foreach(QString recipient, recipients) {
            to.push_back( MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipient.toStdString()));
        }
        message.setRecipients(to);
        message.setSubject(subject);
        message.setContentType("text/plain; charset=UTF-8");
        message.setContent(content, MailMessage::ENCODING_8BIT);

        qDebug() << "here ready";

        try {
            initializeSSL();
            SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
            Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
            SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

            Poco::UInt16 port_const = 587;
            SecureSMTPClientSession session(host.toStdString(), port_const);

            try {
                // TLS begins with an unsecured connection
                session.login();
                // Upgrades to secured connection once the information is sent
                if (session.startTLS()) {
                    session.login(SMTPClientSession::AUTH_LOGIN, user.toStdString(), pass.toStdString());
                    session.sendMessage(message);
                    cout << "Message successfully sent" << endl;
                    success = true;
                }
                session.close();
                uninitializeSSL();
            } catch (SMTPException &e) {
                //cerr << e.displayText() << endl;
                session.close();
                uninitializeSSL();
            }
        }
        catch (NetException &e) {
            //cerr << e.displayText() << endl;
        }
    }
    return success;
}
