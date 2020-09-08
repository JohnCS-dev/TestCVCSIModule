#ifndef TCPTRANSMISSIONMODULE_H
#define TCPTRANSMISSIONMODULE_H

#include <QObject>
#include <QTcpServer>

enum TCPSignature {
    sCompressedFrame = 1
};

#pragma pack(push,1)
struct TCPHeader {
    quint8 signature;
    quint32 dataSize;
};
#pragma pack(pop)

class TCPTransmissionModule : public QObject
{
    Q_OBJECT
public:
    explicit TCPTransmissionModule(QObject *parent = nullptr);
    void startServer(quint16 port);
    void startClient();

signals:

public slots:
    void serverSendData(TCPSignature signature, std::vector<uchar> &data);
private:
    QTcpServer tcpServer;
    QTcpSocket* serverClientSocket;

private slots:
    void serverNewConnection();
    void serverClientSocketDisconected();
    void serverClientSocketRead();
};

#endif // TCPTRANSMISSIONMODULE_H
