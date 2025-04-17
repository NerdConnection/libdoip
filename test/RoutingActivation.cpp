#include "gtest/gtest.h"
#include "DoIPServer.h"
#include "DoIPClient_h.h"

class RoutingActivation : public ::testing::Test {
    protected:
    DoIPServer* server;
    DoIPClient* client;
    void SetUp() override {
        std::cout << "----------------------------- SET UP -----------------------------" << std::endl;
        server = new DoIPServer();
        client = new DoIPClient();
    }

    void TearDown() override {
        std::cout << "----------------------------- TEAR DOWN -----------------------------" << std::endl;
        if (server) {
            delete server;
            server = nullptr;
        }
        if (client) {
            delete client;
            client = nullptr;
        }
    }
};

TEST_F(RoutingActivation, RoutingActivationTest) {
    std::cout << "--------------------- (1) 서버 TCP 소켓 생성 및 수신 대기 ---------------------" << std::endl;
    std::thread serverThread([&]() {
        server->setupTcpSocket();
        server->listenTcpConnection();
    });

    std::cout << "--------------------- (2) 클라이언트 TCP 소켓 생성 및 연결 시도 ---------------------" << std::endl;
    std::thread clientThread([&]() {
        client->startTcpConnection();
    });

    serverThread.join();
    clientThread.join();

    std::cout << "--------------------- (3) 클라이언트에서 서버로 Routing Activation Request 전송---------------------" << std::endl;
    client->sendRoutingActivationRequest();

    std::cout << "--------------------- (4) 서버에서 Routing Activation Request 확인 후 RoutingActivationResponse 전송---------------------" << std::endl;
    server->receiveTcpMessage();

    std::cout << "--------------------- (5) 클라이언트에서 RoutingActivationResponse 수신 ---------------------" << std::endl;
    client->receiveMessage();

    std::cout << "--------------------- (6) 서버, 클라이언트 TCP 소켓 닫기 ---------------------" << std::endl;
    server->closeSocket();
    client->closeTcpConnection();
}