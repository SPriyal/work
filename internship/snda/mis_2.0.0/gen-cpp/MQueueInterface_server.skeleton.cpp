// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "MQueueInterface.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class MQueueInterfaceHandler : virtual public MQueueInterfaceIf {
 public:
  MQueueInterfaceHandler() {
    // Your initialization goes here
  }

  int32_t getCurMsgNum() {
    // Your implementation goes here
    printf("getCurMsgNum\n");
  }

  void quit() {
    // Your implementation goes here
    printf("quit\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<MQueueInterfaceHandler> handler(new MQueueInterfaceHandler());
  shared_ptr<TProcessor> processor(new MQueueInterfaceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

