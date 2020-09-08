#include "tcptransmissionmodule.h"
#include <QTcpSocket>

TCPTransmissionModule::TCPTransmissionModule(QObject *parent) : QObject(parent)
{

}

void TCPTransmissionModule::startServer(quint16 port)
{
    if (tcpServer.isListening())
        return;
    if (!tcpServer.listen(QHostAddress::Any, port)) {
        qInfo() << "TCP server error:" << tcpServer.errorString();
        tcpServer.close();
        return;
    }

    serverClientSocket = nullptr;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(serverNewConnection()));
}

void TCPTransmissionModule::startClient()
{
    //---
}

void TCPTransmissionModule::serverSendData(TCPSignature signature, std::vector<uchar> &data)
{
    if (!serverClientSocket)
        return;

    TCPHeader header;
    header.signature = quint8(signature);
    header.dataSize = data.size() * sizeof(uchar);

    QByteArray arrBlock;
    arrBlock.append((const char*)&header, sizeof(header));
    arrBlock.append((const char*)data.data(), header.dataSize);
    serverClientSocket->write(arrBlock);
}

void TCPTransmissionModule::serverNewConnection()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    if (serverClientSocket) {
        clientSocket->disconnectFromHost();
        clientSocket->deleteLater();
        return;
    }
    else {
        connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(serverClientSocketDisconected()));
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(serverClientSocketRead()));
        serverClientSocket = clientSocket;
    }

    qInfo() << "TCP server new connection:" << clientSocket->localAddress().toString() << clientSocket->peerAddress().toString();
}

void TCPTransmissionModule::serverClientSocketDisconected()
{
    serverClientSocket->deleteLater();
    serverClientSocket = nullptr;
}

void TCPTransmissionModule::serverClientSocketRead()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    clientSocket->readAll();
//        QDataStream in(clientSocket);
//        in.setVersion(QDataStream::Qt_4_2);
//        for (;;) {
//            if (!m_nNextBlockSize) {
//                if (clientSocket->bytesAvailable() < sizeof(quint16)) {
//                    break;
//                }
//                in >> m_nNextBlockSize;
//            }

//            if (clientSocket->bytesAvailable() < m_nNextBlockSize) {
//                break;
//            }
//            QTime   time;
//            QString str;
//            in >> time >> str;

//            QString strMessage =
//                time.toString() + " " + "Client has sended - " + str;
//            m_ptxt->append(strMessage);

//            m_nNextBlockSize = 0;

//            sendToClient(clientSocket,
//                         "Server Response: Received \"" + str + "\""
//                        );
//        }
}
