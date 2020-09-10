#include "tcptransmissionmodule.h"

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
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(serverNewConnection()));
}

void TCPTransmissionModule::startClient(QString host, quint16 port)
{
    if (tcpClient.isOpen())
        return;
    tcpClient.connectToHost(host, port);

    connect(&tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientSocketRead()));
    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpClientError(QAbstractSocket::SocketError)));

}

void TCPTransmissionModule::stopClient()
{
    tcpClient.disconnectFromHost();
    disconnect(&tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientSocketRead()));
    disconnect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(tcpClientError(QAbstractSocket::SocketError socketError)));
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
    QTcpSocket* clientSocket = tcpServer.nextPendingConnection();
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

void TCPTransmissionModule::tcpClientError(QAbstractSocket::SocketError)
{
    qInfo() << "Client:" << tcpClient.errorString();
}

void TCPTransmissionModule::tcpClientSocketRead()
{
    if (lastClientHeader.dataSize == 0) {
        if (tcpClient.bytesAvailable() < sizeof(TCPHeader))
            return;

        tcpClient.read((char*)&lastClientHeader, sizeof(TCPHeader));
    }

    if (tcpClient.bytesAvailable() < lastClientHeader.dataSize)
        return;

    QByteArray buf = tcpClient.read(lastClientHeader.dataSize);
    emit newClientData(lastClientHeader.signature, buf);
    lastClientHeader = TCPHeader();
}
