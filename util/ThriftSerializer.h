#ifndef __THRIFT_SERIALIZER_H__
#define __THRIFT_SERIALIZER_H__

#include "protocol/TCompactProtocol.h"
#include "transport/TBufferTransports.h"

#include <boost/shared_ptr.hpp>

using namespace apache;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

template <typename T>
class ThriftSerializer {
private:
  boost::shared_ptr<TMemoryBuffer>   buffer_;

public:
  ThriftSerializer() : buffer_(new TMemoryBuffer()) {
  }

  void serialize(const T& object, uint8_t** buf, uint32_t* size) {
    buffer_->resetBuffer();
    TCompactProtocol protocol(buffer_);
    object.write(&protocol);

    buffer_->getBuffer(buf, size);
  }

  void deserialize(T& object, uint8_t* buf, uint32_t size) {
    buffer_->resetBuffer(buf, size);
    TCompactProtocol protocol(buffer_);
    
    object.read(&protocol);
  }
};

#endif
