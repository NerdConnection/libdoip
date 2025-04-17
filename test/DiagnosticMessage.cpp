#include "gtest/gtest.h"
#include "DoIPServer.h"
#include "DoIPClient_h.h"

class DiagnosticMessage : public ::testing::Test {
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


TEST_F(DiagnosticMessage, DiagnosticMessageTest) {
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


    std::cout << "--------------------- (6) 클라이언트에서 서버로 DiagnosticMessage request 전송---------------------" << std::endl;
    unsigned char targetAddress[] = {0x02, 0x03};
    unsigned char userData[] = {0x22, 0xF1, 0x90};
    client->sendDiagnosticMessage(targetAddress, userData, 3); // 진단 메시지 전송


    std::cout << "--------------------- (7) 서버에서 Diagnostic 관련 콜백함수 설정, DiagnosticMessage request 수신 ---------------------" << std::endl;
    server->setCallback(
        [this](unsigned char* target_address, unsigned char* cb_message, int cb_message_length) {
            server->receiveDiagnosticPayload(target_address, cb_message, cb_message_length);
        },
        [this](unsigned char* target) {
            // ECU에 진단 요청을 전송 가능한지 사전 확인하는 추상화 레이어
            // 현재는 무조건 true (원래는 CAN 통신 삽입)
            bool EcuFlag = true;
            if (EcuFlag) {
                // 클라이언트로 Positive ack 보내기
                server->sendDiagnosticAck(true, 0x00);
                return true;
            } else {
                // 클라이언트로 Negative ack 보내기
                server->sendDiagnosticAck(false, 0x01);
                return false;
            }
        },
        []() {
            std::cout << "Connection closed" << std::endl;
        }
    );
    
    server->receiveTcpMessage();


    std::cout << "--------------------- (8) 클라이언트에서 DiagnosticMessage 수신 ---------------------" << std::endl;
    client->receiveMessage();
}