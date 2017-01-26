////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_PROTOCOL_PROTOCOLHELPER_H
#define AI_PROTOCOL_PROTOCOLHELPER_H

#include "Kernel_Export.h"
#include "internal/Ai_ExecutionModule.pb.h"

#include <QList>



namespace ai {

namespace kernel {
class IntegratorOptions;
class ModuleGroup;
}

namespace protocol {

class Kernel_EXPORT ProtocolHelper
{
public:
	ProtocolHelper();

public:
	static QByteArray serializeMessage(const google::protobuf::Message& message);
	static void deserializeMessage(google::protobuf::MessageLite* destination, const void* source, size_t source_sz);

public: // const
	static QList<kernel::ExecutionModule> executionModules(const ai::kernel::IntegratorOptions& integratorOptions);
	static QList<kernel::ExecutionModule> executionModules(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName);
	static QList<kernel::ExecutionModule> executionModules(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName, const QString& groupName);
	static QList<ai::kernel::ModuleGroup> moduleGroups(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName);
	static ai::proto::PostParameters executionModuleParameter(const ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId);

public: // mutable
	static QList<kernel::ExecutionModule*> mutableExecutionModules(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId);
	static QList<kernel::ExecutionModule::Card*> mutableCards(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId, const std::string& cardId);
	static QList<ai::proto::PostParameters*> mutableExecutionModuleParameters(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId);
};

} // namespace protocol
} // namespace ai

#endif // AI_PROTOCOL_PROTOCOLHELPER_H
