////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ProtocolHelper.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "internal/Ai_Integrator.pb.h"

#include <QScopedArrayPointer>



using namespace ai::protocol;

ProtocolHelper::ProtocolHelper()
{

}

QList<ai::kernel::ExecutionModule> ProtocolHelper::executionModules(const ai::kernel::IntegratorOptions& integratorOptions)
{
	QList<ai::kernel::ExecutionModule> executionModuleList;

	for(int i = 0, size = integratorOptions.work_stations_size(); i < size; ++i) {
		const auto& workStation = integratorOptions.work_stations(i);

		for(int i = 0, size = workStation.module_groups_size(); i < size; ++i) {
			const auto& moduleGroup = workStation.module_groups(i);

			for(int i = 0, size = moduleGroup.execution_modules_size(); i < size; ++i) {
				const auto&  executionModule = moduleGroup.execution_modules(i);

				executionModuleList.append(executionModule);
			}
		}
	}

	return executionModuleList;
}

QList<ai::kernel::ExecutionModule> ProtocolHelper::executionModules(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName)
{
	QList<ai::kernel::ExecutionModule> executionModuleList;
	auto stdWorkStationName = workStationName.toStdString();

	for(int i = 0, size = integratorOptions.work_stations_size(); i < size; ++i) {
		const auto& workStation = integratorOptions.work_stations(i);

		if(workStation.name() != stdWorkStationName)
			continue;

		for(int i = 0, size = workStation.module_groups_size(); i < size; ++i) {
			const auto& moduleGroup = workStation.module_groups(i);

			for(int i = 0, size = moduleGroup.execution_modules_size(); i < size; ++i) {
				const auto&  executionModule = moduleGroup.execution_modules(i);

				executionModuleList.append(executionModule);
			}
		}
	}

	return executionModuleList;
}

QList<ai::kernel::ExecutionModule> ProtocolHelper::executionModules(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName, const QString& groupName)
{
	QList<ai::kernel::ExecutionModule> executionModuleList;
	auto stdWorkStationName = workStationName.toStdString();
	auto stdGroupName = groupName.toStdString();

	for(int i = 0, size = integratorOptions.work_stations_size(); i < size; ++i) {
		const auto& workStation = integratorOptions.work_stations(i);

		if(workStation.name() != stdWorkStationName)
			continue;

		for(int i = 0, size = workStation.module_groups_size(); i < size; ++i) {
			const auto& moduleGroup = workStation.module_groups(i);

			if(moduleGroup.name() != stdGroupName)
				continue;

			for(int i = 0, size = moduleGroup.execution_modules_size(); i < size; ++i) {
				executionModuleList << moduleGroup.execution_modules(i);
			}
		}
	}

	return executionModuleList;
}

QList<ai::kernel::ModuleGroup> ProtocolHelper::moduleGroups(const ai::kernel::IntegratorOptions& integratorOptions, const QString& workStationName)
{
	QList<ai::kernel::ModuleGroup> moduleGroupList;
	auto stdWorkStationName = workStationName.toStdString();

	for(int i = 0, size = integratorOptions.work_stations_size(); i < size; ++i) {
		const auto& workStation = integratorOptions.work_stations(i);

		if(workStation.name() != stdWorkStationName)
			continue;

		for(int i = 0, size = workStation.module_groups_size(); i < size; ++i) {
			moduleGroupList << workStation.module_groups(i);
		}
	}

	return moduleGroupList;
}

ai::proto::PostParameters ProtocolHelper::executionModuleParameter(const ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId)
{
	auto parameters = mutableExecutionModuleParameters(const_cast<ai::kernel::IntegratorOptions*>(integratorOptions), executionModuleId);

	if(parameters.isEmpty())
		return ai::proto::PostParameters();

	return *parameters[0];
}

QByteArray ProtocolHelper::serializeMessage(const google::protobuf::Message& message)
{
	QByteArray res;
	size_t size = message.ByteSize();

	QScopedArrayPointer<char> p(new char[size]);
	google::protobuf::io::ArrayOutputStream stream(static_cast<void*> (p.data()), size);
	message.SerializeToZeroCopyStream(&stream);
	res.append(p.data(), size);

	return res;
}

void ProtocolHelper::deserializeMessage(google::protobuf::MessageLite* destination, const void* source, size_t source_sz)
{
	google::protobuf::io::ArrayInputStream stream(source, source_sz);
	destination->ParseFromZeroCopyStream(&stream);
}

QList<ai::kernel::ExecutionModule*> ProtocolHelper::mutableExecutionModules(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId)
{
	QList<ai::kernel::ExecutionModule*> ems;

	for(int i = 0, size = integratorOptions->work_stations_size(); i < size; ++i) {
		auto* workStation = integratorOptions->mutable_work_stations(i);

		for(int i = 0, size = workStation->module_groups_size(); i < size; ++i) {
			auto* moduleGroup = workStation->mutable_module_groups(i);

			for(int i = 0, size = moduleGroup->execution_modules_size(); i < size; ++i) {
				auto* executionModule = moduleGroup->mutable_execution_modules(i);

				if(executionModule->id() == executionModuleId) {
					ems << executionModule;
				}
			}
		}
	}

	return ems;
}

QList<ai::kernel::ExecutionModule::Card*> ProtocolHelper::mutableCards(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId, const std::string& cardId)
{
	QList<ai::kernel::ExecutionModule::Card*> cards;

	auto ems = mutableExecutionModules(integratorOptions, executionModuleId);

	if(ems.isEmpty())
		return cards;

	for(auto* executionModule : ems) {
		for(int i = 0, size = executionModule->cards_size(); i < size; ++i) {
			auto* card = executionModule->mutable_cards(i);

			if(card->id() != cardId)
				continue;

			cards << card;
		}
	}

	return cards;
}

QList<ai::proto::PostParameters*> ProtocolHelper::mutableExecutionModuleParameters(ai::kernel::IntegratorOptions* integratorOptions, const std::string& executionModuleId)
{
	QList<ai::proto::PostParameters*> parameters;

	auto ems = mutableExecutionModules(integratorOptions, executionModuleId);

	if(ems.isEmpty())
		return parameters;

	for(auto* executionModule : ems) {
		parameters << executionModule->mutable_parameters();
	}

	return parameters;
}
