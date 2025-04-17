#include "gtest/gtest.h"
#include "DoIPServer.h"
#include "DoIPClient_h.h"

class requestVehicleInformation : public ::testing::Test {
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

TEST_F(requestVehicleInformation, requestVehicleInformationTest) {
    std::cout << "--------------------- (1) 서버, 클라이언트 UDP 소켓 생성 및 설정 ---------------------" << std::endl;
    server->setupUdpSocket();  
    client->startUdpConnection();  
    
    std::cout << "--------------------- (2) 초기 차량 정보 출력 ( 클라이언트 ) ---------------------" << std::endl;
    client->displayVIResponseInformation();

    std::cout << "--------------------- (3) 서버에서 차량 정보 전송 ( 클라이언트는 수신 못함 ) ---------------------" << std::endl;
    server->sendVehicleAnnouncement();

    std::cout << "--------------------- (4) 클라이언트에서 Vehicle Identification Request 전송 (Broadcast) ---------------------" << std::endl;
    client->sendVehicleIdentificationRequest("255.255.255.255");

    std::cout << "--------------------- (5) 서버가 클라이언트 요청 수신 후 차량 정보 요청을 확인 후 응답 전송 ---------------------" << std::endl;
    server->receiveUdpMessage();
    
    std::cout << "--------------------- (6) 클라이언트가 응답 수신 후 파싱 및 저장 ---------------------" << std::endl;
    client->receiveUdpMessage();
    
    std::cout << "--------------------- (7) 변경된 차량 정보 출력 ( 클라이언트 ) ---------------------" << std::endl;
    client->displayVIResponseInformation();

    std::cout << "--------------------- (8) 서버, 클라이언트 UDP 소켓 닫기 ---------------------" << std::endl;
    server->closeUdpSocket();
    client->closeUdpConnection();
}
