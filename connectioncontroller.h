#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <QObject>
#include <QMetaObject>
#include <QString>

class ConnectionController : public QObject {
    Q_OBJECT
public:
    ConnectionController(QObject *parent = nullptr) : QObject(parent) {}
    template<typename Sender, typename Receiver, typename Signal, typename Slot>
    bool createConnection(Sender *sender, Signal signal, Receiver *receiver, Slot slot) {
        QObject *obj1, *obj2;
        if ((obj1 = dynamic_cast<QObject*>(sender)) != nullptr && (obj2 = dynamic_cast<QObject*>(receiver)) != nullptr) {
            QMetaObject::Connection connection = QObject::connect(sender, signal, receiver, slot);
            connections.append(Connection(obj1, obj2, connection));
            return true;
        }
        return false;
    }
    template<typename Sender, typename Receiver, typename Signal, typename Slot>
    bool createConnection(QString name ,Sender *sender, Signal signal, Receiver *receiver, Slot slot) {
        QObject *obj1, *obj2;
        if ((obj1 = dynamic_cast<QObject*>(sender)) != nullptr && (obj2 = dynamic_cast<QObject*>(receiver)) != nullptr) {
            QMetaObject::Connection connection = QObject::connect(sender, signal, receiver, slot);
            connections.append(Connection(name, obj1, obj2, connection));
            return true;
        }
        return false;
    }
    void disconnectOnObject(QObject *obj) {
        auto it = connections.begin();
        while (it != connections.end()) {
            if (obj == it->sender || obj == it->receiver) {
                disconnect(it->connection);
                it = connections.erase(it);
            } else ++it;
        }
    }
    void disconnectOnName(const QString name) {
        if (name != "") {
            auto it = connections.begin();
            while (it != connections.end()) {
                if (it->connectionName == name) {
                    disconnect(it->connection);
                    it = connections.erase(it);
                } else ++it;
            }
        }
    }
private:
    struct Connection {
        Connection(QObject *senderObj, QObject *receiverObj, QMetaObject::Connection thisConnection)
            : sender(senderObj)
            , receiver(receiverObj)
            , connection(thisConnection) {}
        Connection(const QString &name, QObject *senderObj, QObject *receiverObj, QMetaObject::Connection thisConnection)
            : connectionName(name)
            , sender(senderObj)
            , receiver(receiverObj)
            , connection(thisConnection) {}
        QString connectionName = "";
        QObject *sender;
        QObject *receiver;
        QMetaObject::Connection connection;
    };
    QList<Connection> connections;
};

#endif // CONNECTIONCONTROLLER_H
