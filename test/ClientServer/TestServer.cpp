#include "Server.h"
#include "ControllableInterface.h"
#include <maf/export/MafExport_global.h>
#include <maf/messaging/client-server/DefaultMessageTrait.h>
#include <maf/logging/Logger.h>

namespace maf {
namespace test {
using namespace messaging;

class TestServer : public ControllableDefault
{
    using MessageTrait  = messaging::DefaultMessageTrait;
    using ClientComp    = maf::test::ServerComponent<MessageTrait>;
    using Stub          = QueueingServiceStub<MessageTrait>;
public:
    TestServer();
    void init() override;
    void deinit() override;
    void start() override;
    maf::test::ServerComponent<MessageTrait> server;
};


TestServer::TestServer()
{
    logging::Logger::debug("Test server created!");
}

void TestServer::init()
{
    logging::Logger::debug("Test server initializing....");

    server.setName("Server Component ");
    logging::Logger::debug("Test server initialized successfully");
}

void TestServer::deinit()
{
    server.stopTest();
    logging::Logger::debug("Test server deinitialized successfully");
}

void TestServer::start()
{
    server.startTest(
        "app_internal",
        {"com.github.sepcon", 0}
        );
}


} //test
} //maf

MAF_PLUGIN(maf::test::TestServer, "TestServer", "1");