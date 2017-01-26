#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "code/protobuf_packer.h"

void protobuf_deserialize(::google::protobuf::MessageLite & destination, const void * source, size_t source_sz)
{
	::google::protobuf::io::ArrayInputStream stream(source, source_sz);
	destination.ParseFromZeroCopyStream(&stream);
}

void protobuf_deserialize(::google::protobuf::MessageLite & destination, const std::vector<unsigned char> & source)
{
	protobuf_deserialize(destination, &source[0], source.size());
}

void protobuf_serialize(const ::google::protobuf::MessageLite & source, void ** destination, size_t * destination_sz)
{
	*destination_sz = source.ByteSize();
	*destination = malloc(*destination_sz);
	::google::protobuf::io::ArrayOutputStream stream(*destination, *destination_sz);
	source.SerializeToZeroCopyStream(&stream);
}

void protobuf_serialize(const ::google::protobuf::MessageLite & source, std::vector<unsigned char> & destination)
{
	void *p;
	size_t sz;
	protobuf_serialize(source, &p, &sz);
	destination.resize(sz);
	if (sz)
	{
		destination.assign((const unsigned char*)p, (const unsigned char*)p+sz);
		protobuf_serializefree(p);
	}
}

void protobuf_serializefree(void *p)
{
	free(p);
}
