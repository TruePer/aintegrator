#ifndef protobuf_packerH
#define protobuf_packerH

#include <vector>
#include <google/protobuf/message_lite.h>

void LIBPROTOBUF_EXPORT protobuf_deserialize(::google::protobuf::MessageLite & destination, const void * source, size_t source_sz);
void LIBPROTOBUF_EXPORT protobuf_serialize(const ::google::protobuf::MessageLite & source, void ** destination, size_t * destination_sz);
void LIBPROTOBUF_EXPORT protobuf_serializefree(void *p);

#endif
