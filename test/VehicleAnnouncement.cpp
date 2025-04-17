#include "gtest/gtest.h"
#include "DoIPServer.h"
#include "DoIPClient_h.h"

class VehicleAnnouncement : public ::testing::Test {
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

TEST_F(VehicleAnnouncement, VehicleAnnouncementTest) {
    std::cout << "--------------------- (1) 서버, 클라이언트 UDP 소켓 생성 및 설정---------------------" << std::endl;
    // 서버와 클라이언트의 UDP 소켓 설정
    server->setupUdpSocket();  
    client->startUdpConnection();  
    
    std::cout << "--------------------- (2) 초기 차량 정보 출력 ( 클라이언트 )---------------------" << std::endl;
    // 수신 전 차량 정보 표시
    client->displayVIResponseInformation();
    
    std::cout << "--------------------- (3) 차량 정보 3회 전송 (서버 -> 클라이언트) ---------------------" << std::endl;
    // 서버에서 차량 정보 전송
    server->sendVehicleAnnouncement();
    
    std::cout << "--------------------- (4) 차량 정보 수신 및 저장 ( 클라이언트 ) ---------------------" << std::endl;
    // 클라이언트에서 메시지 수신 및 파싱 그리고 저장
    client->receiveUdpMessage();

    std::cout << "--------------------- (5) 변경된 차량 정보 출력 ( 클라이언트 ) ---------------------" << std::endl;
    // 수신 후 차량 정보 출력
    client->displayVIResponseInformation();

    std::cout << "--------------------- (6) 서버, 클라이언트 UDP 소켓 닫기 ---------------------" << std::endl;
    // 서버, 클라이언트 UDP 소켓 닫기
    server->closeUdpSocket();
    client->closeUdpConnection();
}