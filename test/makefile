CXX = g++
CXXFLAGS = -g -I ../libdoipcommon/include/ -I ../libdoipclient/include/ -I ../libdoipserver/include/
LDFLAGS = -L /usr/lib -lgtest -lgtest_main -lpthread
OBJS = ../build/*.o

TEST_NAMES = VehicleAnnouncement RoutingActivation RequestVehicleInfomation DiagnosticMessage

TARGETS = $(addsuffix TestRun, $(TEST_NAMES))

all: $(TARGETS)

%TestRun: %.cpp $(OBJS)
	$(CXX) -o $@ $< $(OBJS) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGETS)
