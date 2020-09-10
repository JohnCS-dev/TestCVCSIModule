#ifndef TCPTRANSMISSIONMODULE_H
#define TCPTRANSMISSIONMODULE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

enum TCPSignature {
    sCompressedFrame = 1
};

#pragma pack(push,1)
struct TCPHeader {
    quint8 signature;
    quint32 dataSize;
    TCPHeader() : signature(0), dataSize(0) {}
};
#pragma pack(pop)

class TCPTransmissionModule : public QObject
{
    Q_OBJECT
public:
    explicit TCPTransmissionModule(QObject *parent = nullptr);
    void startServer(quint16 port);
    void startClient(QString host, quint16 port);
    void stopClient();

signals:
    void newClientData(quint8,QByteArray); // recived from server by client

public slots:
    void serverSendData(TCPSignature signature, std::vector<uchar> &data);
private:
    QTcpServer tcpServer;
    QTcpSocket* serverClientSocket;
    QTcpSocket tcpClient;
    TCPHeader lastClientHeader;

private slots:
    // server slots
    void serverNewConnection();
    void serverClientSocketDisconected();
    void serverClientSocketRead();

    // client slots
    void tcpClientError(QAbstractSocket::SocketError);
    void tcpClientSocketRead();
};

#endif // TCPTRANSMISSIONMODULE_H
