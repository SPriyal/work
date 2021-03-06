/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef MISInterface_H
#define MISInterface_H

#include <TProcessor.h>
#include "mis_types.h"



class MISInterfaceIf {
 public:
  virtual ~MISInterfaceIf() {}
  virtual void check(MISResponse& _return, const MISRequest& request) = 0;
  virtual void add2Q(const MISRequest& request, const std::string& reason) = 0;
  virtual int8_t ping() = 0;
  virtual void reload() = 0;
};

class MISInterfaceNull : virtual public MISInterfaceIf {
 public:
  virtual ~MISInterfaceNull() {}
  void check(MISResponse& /* _return */, const MISRequest& /* request */) {
    return;
  }
  void add2Q(const MISRequest& /* request */, const std::string& /* reason */) {
    return;
  }
  int8_t ping() {
    int8_t _return = 0;
    return _return;
  }
  void reload() {
    return;
  }
};

class MISInterface_check_args {
 public:

  MISInterface_check_args() {
  }

  virtual ~MISInterface_check_args() throw() {}

  MISRequest request;

  struct __isset {
    __isset() : request(false) {}
    bool request;
  } __isset;

  bool operator == (const MISInterface_check_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const MISInterface_check_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_check_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_check_pargs {
 public:


  virtual ~MISInterface_check_pargs() throw() {}

  const MISRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_check_result {
 public:

  MISInterface_check_result() {
  }

  virtual ~MISInterface_check_result() throw() {}

  MISResponse success;

  struct __isset {
    __isset() : success(false) {}
    bool success;
  } __isset;

  bool operator == (const MISInterface_check_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MISInterface_check_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_check_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_check_presult {
 public:


  virtual ~MISInterface_check_presult() throw() {}

  MISResponse* success;

  struct __isset {
    __isset() : success(false) {}
    bool success;
  } __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MISInterface_add2Q_args {
 public:

  MISInterface_add2Q_args() : reason("") {
  }

  virtual ~MISInterface_add2Q_args() throw() {}

  MISRequest request;
  std::string reason;

  struct __isset {
    __isset() : request(false), reason(false) {}
    bool request;
    bool reason;
  } __isset;

  bool operator == (const MISInterface_add2Q_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    if (!(reason == rhs.reason))
      return false;
    return true;
  }
  bool operator != (const MISInterface_add2Q_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_add2Q_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_add2Q_pargs {
 public:


  virtual ~MISInterface_add2Q_pargs() throw() {}

  const MISRequest* request;
  const std::string* reason;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_add2Q_result {
 public:

  MISInterface_add2Q_result() {
  }

  virtual ~MISInterface_add2Q_result() throw() {}


  bool operator == (const MISInterface_add2Q_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MISInterface_add2Q_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_add2Q_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_add2Q_presult {
 public:


  virtual ~MISInterface_add2Q_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MISInterface_ping_args {
 public:

  MISInterface_ping_args() {
  }

  virtual ~MISInterface_ping_args() throw() {}


  bool operator == (const MISInterface_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MISInterface_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_ping_pargs {
 public:


  virtual ~MISInterface_ping_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_ping_result {
 public:

  MISInterface_ping_result() : success(0) {
  }

  virtual ~MISInterface_ping_result() throw() {}

  int8_t success;

  struct __isset {
    __isset() : success(false) {}
    bool success;
  } __isset;

  bool operator == (const MISInterface_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MISInterface_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_ping_presult {
 public:


  virtual ~MISInterface_ping_presult() throw() {}

  int8_t* success;

  struct __isset {
    __isset() : success(false) {}
    bool success;
  } __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MISInterface_reload_args {
 public:

  MISInterface_reload_args() {
  }

  virtual ~MISInterface_reload_args() throw() {}


  bool operator == (const MISInterface_reload_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MISInterface_reload_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_reload_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_reload_pargs {
 public:


  virtual ~MISInterface_reload_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_reload_result {
 public:

  MISInterface_reload_result() {
  }

  virtual ~MISInterface_reload_result() throw() {}


  bool operator == (const MISInterface_reload_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MISInterface_reload_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MISInterface_reload_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class MISInterface_reload_presult {
 public:


  virtual ~MISInterface_reload_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MISInterfaceClient : virtual public MISInterfaceIf {
 public:
  MISInterfaceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  MISInterfaceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void check(MISResponse& _return, const MISRequest& request);
  void send_check(const MISRequest& request);
  void recv_check(MISResponse& _return);
  void add2Q(const MISRequest& request, const std::string& reason);
  void send_add2Q(const MISRequest& request, const std::string& reason);
  void recv_add2Q();
  int8_t ping();
  void send_ping();
  int8_t recv_ping();
  void reload();
  void send_reload();
  void recv_reload();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MISInterfaceProcessor : virtual public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<MISInterfaceIf> iface_;
  virtual bool process_fn(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid);
 private:
  std::map<std::string, void (MISInterfaceProcessor::*)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*)> processMap_;
  void process_check(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_add2Q(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
  void process_reload(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot);
 public:
  MISInterfaceProcessor(boost::shared_ptr<MISInterfaceIf> iface) :
    iface_(iface) {
    processMap_["check"] = &MISInterfaceProcessor::process_check;
    processMap_["add2Q"] = &MISInterfaceProcessor::process_add2Q;
    processMap_["ping"] = &MISInterfaceProcessor::process_ping;
    processMap_["reload"] = &MISInterfaceProcessor::process_reload;
  }

  virtual bool process(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot);
  virtual ~MISInterfaceProcessor() {}
};

class MISInterfaceMultiface : virtual public MISInterfaceIf {
 public:
  MISInterfaceMultiface(std::vector<boost::shared_ptr<MISInterfaceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MISInterfaceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<MISInterfaceIf> > ifaces_;
  MISInterfaceMultiface() {}
  void add(boost::shared_ptr<MISInterfaceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void check(MISResponse& _return, const MISRequest& request) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        ifaces_[i]->check(_return, request);
        return;
      } else {
        ifaces_[i]->check(_return, request);
      }
    }
  }

  void add2Q(const MISRequest& request, const std::string& reason) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      ifaces_[i]->add2Q(request, reason);
    }
  }

  int8_t ping() {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->ping();
      } else {
        ifaces_[i]->ping();
      }
    }
  }

  void reload() {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      ifaces_[i]->reload();
    }
  }

};



#endif
