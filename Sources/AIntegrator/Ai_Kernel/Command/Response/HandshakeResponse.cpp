////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "HandshakeResponse.h"
#include "internal/Ai_Primitives.pb.h"



namespace ai {
namespace command {

HandshakeResponse::HandshakeResponse(const kernel::AiMessage& response)
    : Command(QString::fromStdString(response.host_id()))
{
}

void HandshakeResponse::execute()
{

}

} // namespace command
} // namespace ai
