////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "Query.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "Protocol/google/protobuf/descriptor.h"
#include "Transport/TcpServer.h"
#include "State/AppState.h"
#include <QDebug>



using namespace ai::query;

Query::Query(ai::common::Core* core)
    : QObject(core)
    , _core(core)
{
	Q_ASSERT(_core);

	startTimer(100);

	qRegisterMetaType<ai::kernel::AiMessage>("ai::kernel::AiMessage");

	connect( _core->server(), SIGNAL(recievedMessage(ai::kernel::AiMessage)), SLOT(onRecievedMessageFromHost(ai::kernel::AiMessage)), Qt::QueuedConnection );
}

/**
 * Enqueues commands from UI and from hosts
 * @param command
 */
void Query::addCommand(ai::command::ICommand::SPtr command)
{
	_outCommandQueue.enqueue(command);
}

/**
 * Enqueues incoming messages from host
 * @param message
 */
void Query::onRecievedMessageFromHost(const ai::kernel::AiMessage& message)
{
	_inMessageQueue.enqueue(message);
}

/**
 * Process queues of commands and messages
 */
void Query::timerEvent(QTimerEvent* /*event*/)
{
	// Обработаем очередь сообщений, полученных от хостов
	while(_inMessageQueue.count()) {
		auto message = _inMessageQueue.dequeue();
		processMessage(message);
	}

	// Обработаем очередь команд
	while(_outCommandQueue.count()) {
		auto cmd = _outCommandQueue.dequeue();
		cmd->execute(_core);

		if(cmd->isNeedResponse()) {
			_waitCommands.insert(cmd->id(), cmd);
		}
	}

	// Проверим очередь ожидающих команд на вшивость (таймауты)
	QList<quint32> timedOutCommandIds;

	for(auto cmd : _waitCommands) {
		if(cmd->isTimedOut()) {
			cmd->processTimeout();
			timedOutCommandIds << cmd->id();
		}
	}

	// Удалим вшивые команды из очереди
	for(auto id : timedOutCommandIds) {
		_waitCommands.remove(id);
	}
}

void Query::processMessage(const ai::kernel::AiMessage& message)
{
	switch (message.proto().type())
	{
	case proto::AiMessage_Type_HANDSHAKE_REQUEST:
		processHandshakeRequest(message);
		return;

	case proto::AiMessage_Type_SET_PARAMETERS_REQUEST:
		processSetParametersRequest(message);
		return;

	case proto::AiMessage_Type_SET_STATE_REQUEST:
		processSetStateRequest(message);
		return;

	case proto::AiMessage_Type_SET_WORK_MODE_RESPONSE:
	case proto::AiMessage_Type_CONTROL_ACTION_RESPONSE:
		if(_waitCommands.contains(message.proto().id())) {
			auto cmd = _waitCommands.take(message.proto().id());
			cmd->processResponse(message);
			return;
		}

	default:
		ai::logCommand() << "Recieved unsupported message type";
		break;
	}
}

void Query::processHandshakeRequest(const ai::kernel::AiMessage& request)
{
//	/////////////////////////////////
//	ai::logCommand() << QString("[%1] HANDSHAKE_REQUEST  - cur %2, min %3, build %4, version %5")
//	                    .arg(QString::fromStdString(request.host_id()), 12, ' ')
//	                    .arg(request.proto().handshake_request().host_version().protocol_version_current())
//	                    .arg(request.proto().handshake_request().host_version().protocol_version_min_supported())
//	                    .arg(QString::fromStdString(request.proto().handshake_request().host_version().build_date_time()))
//	                    .arg(QString::fromStdString(request.proto().handshake_request().host_version().software_version()))
//	                    ;
//	/////////////////////////////////

	ai::kernel::AiMessage message;
	message.set_host_id(request.host_id());
	message.mutable_proto()->set_type(proto::AiMessage::HANDSHAKE_RESPONSE);
	message.mutable_proto()->set_id(request.proto().id());

	auto* response = message.mutable_proto()->mutable_handshake_response();

	// Заполним номера версий
	auto* version = response->mutable_integrator_version();
	version->set_protocol_version_current(proto::SoftwareVersion_ProtocolCurrentVersion_CURRENT);
	version->set_protocol_version_min_supported(proto::SoftwareVersion_ProtocolSupportedVersion_MIN);
	version->set_software_version(QString("%1.%2.%4").arg(MAJOR_VER).arg(MINOR_VER).arg(REVISION_VER).toStdString());
	version->set_build_date_time(QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy")).toString("dd.MM.yyyy").toStdString());

	const auto& hostVer = request.proto().handshake_request().host_version();

	// Заполним код ответа
	auto* report = response->mutable_report();
	report->set_code(proto::DONE);

	if(!request.proto().has_handshake_request()) {
		report->set_code(proto::ERROR);
		report->set_description(tr("Request doesn't contains field handshake_request").toStdString());
	}
	else if(hostVer.protocol_version_current() < version->protocol_version_min_supported()) {
		report->set_code(proto::ERR_UNSUPPORTED_PROTOCOL_VERSION);
		report->set_description(tr("Unsupported protocol versions detected. Host current version: %1, Integrator min supported version: %2")
		                        .arg(hostVer.protocol_version_current())
		                        .arg(version->protocol_version_min_supported())
		                        .toStdString());
	}
	else if(hostVer.protocol_version_min_supported() > version->protocol_version_current()) {
		report->set_code(proto::ERR_UNSUPPORTED_PROTOCOL_VERSION);
		report->set_description(tr("Unsupported protocol versions detected. Integrator current version: %1, Host min supported version: %2")
		                        .arg(version->protocol_version_current())
		                        .arg(hostVer.protocol_version_min_supported())
		                        .toStdString());
	}
	else if(hostVer.protocol_version_current() != version->protocol_version_current()) {
		report->set_code(proto::ERR_PROTOCOL_VERSION_MISMATCH);
		report->set_description(tr("Mismatch versions of the protocol. Integrator current version: %1, Host current version: %2")
		                        .arg(version->protocol_version_current())
		                        .arg(hostVer.protocol_version_current())
		                        .toStdString());
	}

	if(report->code() != proto::DONE) {
		ai::error() << tr("Module '%1': %2")
		               .arg(QString::fromStdString(request.host_id()))
		               .arg(QString::fromStdString(report->description()));
	}

	_core->server()->sendMessage(message);

//	/////////////////////////////////
//	ai::logCommand() << QString("[%1] HANDSHAKE_RESPONSE - cur %2, min %3, build %4, version %5")
//	                    .arg(QString::fromStdString(message.host_id()), 12, ' ')
//	                    .arg(version->protocol_version_current())
//	                    .arg(version->protocol_version_min_supported())
//	                    .arg(QString::fromStdString(version->build_date_time()))
//	                    .arg(QString::fromStdString(version->software_version()))
//	                    ;
//	/////////////////////////////////
}

void Query::processSetParametersRequest(const ai::kernel::AiMessage& request)
{
	kernel::AiMessage message;
	message.set_host_id(request.host_id());
	message.mutable_proto()->set_type(proto::AiMessage::SET_PARAMETERS_RESPONSE);
	message.mutable_proto()->set_id(request.proto().id());

	auto* response = message.mutable_proto()->mutable_set_parameters_response();
	response->mutable_report()->set_code(proto::DONE);

	auto ems = ai::protocol::ProtocolHelper::mutableExecutionModules(_core->state()->integratorOptions(), request.host_id());

	for(auto* em : ems) {
		if(!em) {
			auto errorString = QString("Module id %1 is not found")
			        .arg(QString::fromStdString(request.host_id()));

			ai::logCommand() << errorString;

			response->mutable_report()->set_code(proto::ERR_PARAMETERS_NOT_APPLIED);
			response->mutable_report()->set_description(errorString.toStdString());
		}
		else if(!request.proto().has_set_parameters_request()) {
			auto errorString = QString("Proto message don't have set_parameters_request field")
			        .arg(QString::fromStdString(request.host_id()));

			ai::logCommand() << errorString;

			response->mutable_report()->set_code(proto::ERR_PARAMETERS_NOT_APPLIED);
			response->mutable_report()->set_description(errorString.toStdString());
		}
		else {
			em->mutable_parameters()->CopyFrom(request.proto().set_parameters_request().parameters());
		}
	}

	_core->server()->sendMessage(message);
}

void Query::processSetStateRequest(const ai::kernel::AiMessage& request)
{
	const auto& stateRequest = request.proto().set_state_request();

//	/////////////////////////////////
//	ai::logCommand() << QString("[%1] SET_STATE_REQUEST - %2")
//	                    .arg(request.host_id().c_str(), 12, ' ')
//	                    .arg(ai::proto::StateType_descriptor()->FindValueByNumber(stateRequest.post_state().type())->name().c_str())
//	                    ;
//	/////////////////////////////////

	kernel::AiMessage message;
	message.set_host_id(request.host_id());
	message.mutable_proto()->set_type(proto::AiMessage::SET_STATE_RESPONSE);
	message.mutable_proto()->set_id(request.proto().id());

	auto* response = message.mutable_proto()->mutable_set_state_response();
	response->mutable_report()->set_code(proto::DONE);

	auto cards = ai::protocol::ProtocolHelper::mutableCards(_core->state()->integratorOptions(), request.host_id(), stateRequest.post_state().card_id());

	for(auto* card : cards) {
		if(card) {
			if(card->type() == stateRequest.post_state().type()) {
				card->mutable_state()->CopyFrom(stateRequest.post_state());
				card->set_last_update_state(QDateTime::currentDateTime().toTime_t());
			}
			else {
				auto errorString = QString("Card '%1' is not equal state type")
				        .arg(QString::fromStdString(stateRequest.post_state().card_id()));

				ai::logCommand() << errorString;
				response->mutable_report()->set_code(proto::ERR_STATE_NOT_APPLIED);
				response->mutable_report()->set_description(errorString.toStdString());
			}

			if(card->has_state() && card->state().has_simple_state()) {
				const auto& simpleState = card->state().simple_state();

				if(simpleState.has_current_mode_id() && !simpleState.has_mode_status()) {
					auto errorString = QString("Card '%1' has current_mode_id but don't have mode_status field!")
					        .arg(QString::fromStdString(stateRequest.post_state().card_id()));

					ai::logCommand() << errorString;
					response->mutable_report()->set_code(proto::ERR_STATE_NOT_APPLIED);
					response->mutable_report()->set_description(errorString.toStdString());
				}
			}
		}
		else {
			auto errorString = QString("Can not find card id '%1' from execution module '%2'")
			        .arg(QString::fromStdString(stateRequest.post_state().card_id()))
			        .arg(QString::fromStdString(request.host_id()));

			ai::logCommand() << errorString;
			response->mutable_report()->set_code(proto::ERR_STATE_NOT_APPLIED);
			response->mutable_report()->set_description(errorString.toStdString());
		}
	}

	_core->server()->sendMessage(message);
}

