////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TcpClient.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "host/Ai.pb.h"

#include <QDataStream>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>



TcpClient::TcpClient(const QString& moduleId, QObject* parent)
    : QObject(parent)
    , _tcpSocket(new QTcpSocket(this))
    , _expectedBlockSize(0)
    , _moduleId(moduleId)
    , _isConnected(false)
{
	connect( &_timer, SIGNAL(timeout()),         SLOT(tryToConnect()) );
	connect( _tcpSocket, SIGNAL(connected()),    SLOT(onConnected()) );
	connect( _tcpSocket, SIGNAL(disconnected()), SLOT(onDisconnected()) );
	connect( _tcpSocket, SIGNAL(readyRead()),    SLOT(onReadData()) );

	tryToConnect();
	_timer.start(5000);

	qsrand(QTime::currentTime().msec());
}

void TcpClient::sendHandShake()
{
	ai::proto::AiMessage message;
	message.set_type(ai::proto::AiMessage_Type_HANDSHAKE_REQUEST);

	auto* request = message.mutable_handshake_request();
	request->set_host_id(_moduleId.toStdString());
	request->mutable_host_version()->set_build_date_time(__DATE__); // Формат даты произвольный. Используется только для информирования оператора
	request->mutable_host_version()->set_protocol_version_current(ai::proto::SoftwareVersion::CURRENT);
	request->mutable_host_version()->set_protocol_version_min_supported(ai::proto::SoftwareVersion::MIN);
	request->mutable_host_version()->set_software_version("1.2.3");

	sendMessage(message);
}

void TcpClient::sendParameters()
{
	ai::proto::AiMessage message;
	message.set_type(ai::proto::AiMessage_Type_SET_PARAMETERS_REQUEST);

	auto* request = message.mutable_set_parameters_request();
	auto* group = request->mutable_parameters()->add_groups();
	group->set_name("Group1");

	{ auto* parameter = group->add_parameters();
		parameter->set_id("use_sources");
		parameter->set_type(ai::proto::Parameter::CHECK_BOX);
		parameter->set_name("CheckBox param");
		parameter->set_description("My cool parameter");
		parameter->mutable_check_box()->set_checked(qrand() % 2);
	}

	{ auto* parameter = group->add_parameters();
		parameter->set_id("GroupName");
		parameter->set_type(ai::proto::Parameter::LINE_EDIT);
		parameter->set_name("Line param");
		parameter->set_description("My cool text parameter");
		parameter->mutable_line_edit()->set_ui_type(ai::proto::LineEditParameter_UiType_STRING_VALUE);
		parameter->mutable_line_edit()->set_string_value("asasdasd");
		parameter->mutable_line_edit()->set_max_symbols(5);
	}

	{ auto* parameter = group->add_parameters();
		parameter->set_id("GroupName1");
		parameter->set_type(ai::proto::Parameter::LINE_EDIT);
		parameter->set_name("SInt");
		parameter->set_description("My cool text parameter");
		parameter->mutable_line_edit()->set_ui_type(ai::proto::LineEditParameter_UiType_SIGNED_INT);
		parameter->mutable_line_edit()->set_signed_int(-121);
	}

	{ auto* parameter = group->add_parameters();
		parameter->set_id("GroupName2");
		parameter->set_type(ai::proto::Parameter::LINE_EDIT);
		parameter->set_name("Double");
		parameter->set_description("My cool text parameter");
		parameter->mutable_line_edit()->set_ui_type(ai::proto::LineEditParameter_UiType_SIGNED_DOUBLE);
		parameter->mutable_line_edit()->set_signed_double(1.234);
	}


	{ auto* parameter = group->add_parameters();
		parameter->set_id("sources_list");
		parameter->set_type(ai::proto::Parameter::SELECT_LIST);
		parameter->set_name("Sources");
		parameter->set_description("List of sources");

		auto* selectList = parameter->mutable_select_list();
		selectList->set_ui_type(ai::proto::SelectListParameter_UiType_RADIO_BUTTONS);
		selectList->set_current_index(qrand() % 3);
		selectList->add_values("Source 1");
		selectList->add_values("Source 2");
		selectList->add_values("Source 3");
	}

	{ auto* parameter = group->add_parameters();
		parameter->set_id("GroupName4");
		parameter->set_type(ai::proto::Parameter::LINE_EDIT);
		parameter->set_name("SInt");
		parameter->set_description("My cool text parameter");
		parameter->mutable_line_edit()->set_ui_type(ai::proto::LineEditParameter_UiType_SIGNED_INT);
		parameter->mutable_line_edit()->set_signed_int(-121);
	}

	{ auto* parameter = group->add_parameters();
		parameter->set_id("Interval");
		parameter->set_type(ai::proto::Parameter::VALUE_FROM_INTERVAL);
		parameter->set_name("Intaerval1");
		parameter->set_description("Interval aasdd wsda");
		parameter->mutable_value_from_interval()->set_ui_type(ai::proto::ValueFromIntervalParameter_UiType_SPIN_BOX);
		parameter->mutable_value_from_interval()->set_min_value(0);
		parameter->mutable_value_from_interval()->set_max_value(1000);
		parameter->mutable_value_from_interval()->set_current_value(300);
		parameter->mutable_value_from_interval()->set_step(20);
	}

	sendMessage(message);
}

void TcpClient::sendStatus()
{
	ai::proto::AiMessage message;
	message.set_type(ai::proto::AiMessage_Type_SET_STATE_REQUEST);

	auto* requst = message.mutable_set_state_request();
	auto* state  = requst->mutable_post_state();

	state->set_card_id("1");
	state->set_type(ai::proto::ST_SIMPLE);

	auto* simpleState = state->mutable_simple_state();

	if(_moduleId == "3gpp-gui") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Исправно");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_RHOMBUS);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("БД Спрут отключена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_RED);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DATABASE);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("БД ЧТП и РЭО подключена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DATABASE);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Пеленгатор отключенин").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_RED);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DF);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Координаты актуалны").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_WEB);
		}

		simpleState->add_text_statuses(tr("Стандарты: GSM, UMTS").toStdString());
		simpleState->add_text_statuses(tr("Обнаружено БС: 27").toStdString());
		simpleState->add_text_statuses(tr("Зарегистировано АТ: 2/49").toStdString());

		simpleState->set_current_mode_id("work");
		simpleState->set_mode_status(ai::proto::PM_IDLE);
	}
	else if(_moduleId == "rfpost") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Готов, БД не подключена");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_YELLOW);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_LOCATION);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Подключены клиенты (2)").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_RHOMBUS);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("БД отключена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_RED);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DATABASE);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Координаты актуальны для расчетов").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_WEB);
		}

		simpleState->add_text_statuses(tr("Подключено клиентов: 2").toStdString());
		simpleState->add_text_statuses(tr("Источников в работе: 7").toStdString());
	}
	else if(_moduleId == "barion") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Готов к работе");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_LOCATION);

		simpleState->set_current_mode_id("table");
		simpleState->set_mode_status(ai::proto::PM_RUN);
	}
	else if(_moduleId == "2dnpu-m") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Есть сообщения о неисправностях");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_YELLOW);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_WARNING);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Связь стабильна").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_WIRELESS);
		}

		simpleState->add_text_statuses(tr("Подключено БЛА: 2").toStdString());
	}
	else if(_moduleId == "dmpost") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Исправен");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_OPERATOR);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Подключены клиенты (2)").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DATABASE);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Служба запущена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_COMPUTER);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Служба запущена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_COMPUTER);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Служба запущена").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_COMPUTER);
		}

		simpleState->add_text_statuses(tr("Доступные АРМ: Р, Ц, Л").toStdString());
	}
	else if(_moduleId == "cell-net-uavvideo") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Исправен");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_CIRCLE);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Связь с бортом есть").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_WIRELESS);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Связи с поворотным устройством нет").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_RED);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DF);
		}
	}
	else if(_moduleId == "cell-net-gpsemulator") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Работоспособен с ограничениями");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_YELLOW);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_CIRCLE);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Связь с бортом есть").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_WIRELESS);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Маршрут отправлен").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_RADAR);
		}
	}
	else if(_moduleId == "aeromosaic") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Программа работает");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_CIRCLE);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Вычисления не запущены").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GRAY);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_HARDWARE);
		}
	}
	else if(_moduleId == "fortis") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Готов к работе");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_CIRCLE);

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("Пост подключен").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_LINK);
		}

		{ auto* additionalStatus = simpleState->add_additional_statuses();
			additionalStatus->set_type(ai::proto::SIT_STANDARD);
			additionalStatus->set_description(tr("БД подключен").toStdString());
			additionalStatus->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
			additionalStatus->mutable_standard_icon()->set_type(ai::proto::IT_DATABASE);
		}

		simpleState->add_text_statuses(tr("Подключено постов: 2 из 3").toStdString());
	}
	else if(_moduleId == "afterwork") {
		simpleState->mutable_main_status()->set_type(ai::proto::SIT_STANDARD);
		simpleState->mutable_main_status()->set_description("Готов к работе");
		simpleState->mutable_main_status()->mutable_standard_icon()->set_color(ai::proto::IC_GREEN);
		simpleState->mutable_main_status()->mutable_standard_icon()->set_type(ai::proto::IT_CIRCLE);
	}

	sendMessage(message);
}

void TcpClient::sendData(const QByteArray& data)
{
	quint32 size = data.size();
	QByteArray sendData = QByteArray((const char *)&size, sizeof(size));
	sendData.append(data);

	_tcpSocket->write(sendData);
}

void TcpClient::sendMessage(ai::proto::AiMessage& message)
{
	static quint32 counter = 0;

	if(!message.has_id()) {
		message.set_id(++counter);
	}

	if(message.IsInitialized()) {
		sendData(ai::protocol::ProtocolHelper::serializeMessage(message));
	}
	else {
		qDebug() << "Message initialization error";
		qDebug() << message.InitializationErrorString().c_str();
	}
}

void TcpClient::processMessage(ai::proto::AiMessage& message)
{
	switch (message.type()) {
	case ai::proto::AiMessage_Type_SET_WORK_MODE_REQUEST: {
		ai::proto::AiMessage response;
		response.set_type(ai::proto::AiMessage_Type_SET_WORK_MODE_RESPONSE);
		response.set_id(message.id());

		response.mutable_set_work_mode_response()->set_work_mode_id(message.set_work_mode_request().work_mode_id());

		auto* report = response.mutable_set_work_mode_response()->mutable_report();
		report->set_code(ai::proto::DONE);

		sendMessage(response);

		break;
	}
	case ai::proto::AiMessage_Type_CONTROL_ACTION_REQUEST: {

		ai::proto::AiMessage response;
		response.set_type(ai::proto::AiMessage_Type_CONTROL_ACTION_RESPONSE);
		response.set_id(message.id());

		auto* report = response.mutable_control_action_response()->mutable_report();
		report->set_code(ai::proto::DONE);

		sendMessage(response);

		if(message.has_control_action_request()) {
			auto type = message.control_action_request().action().type();

			if(type == ai::proto::ControlAction_Type_CA_KILL_PROGRAM) {
				exit(0);
			}

			if(type == ai::proto::ControlAction_Type_CA_RAISE_WINDOW) {
				if(parent()) {
					auto* window = qobject_cast<QMainWindow*>(parent());
					if(window) {
						auto state = window->windowState();
						window->showFullScreen();
						window->showNormal();
						window->setWindowState(state);
						window->raise();  // for MacOS
						window->activateWindow(); // for Windows
						window->setFocus();
					}
				}
			}
		}

		break;
	}
	default:
		break;
	}
}

void TcpClient::onReadData()
{
	QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());

	if(!clientSocket)
		return;

	while(true) {
		if(!_expectedBlockSize) {
			if(clientSocket->bytesAvailable() < sizeof(_expectedBlockSize)) {
				break; // Данных нет, или их недостаточно даже для определения размера блока. Ждем следущую порцию
			}

			// Читаем размер блока
			QDataStream in(clientSocket);
			in.setByteOrder(QDataStream::LittleEndian);
			in >> _expectedBlockSize;
		}

		// Проверим, все ли данные пришли от клиента
		if(clientSocket->bytesAvailable() < _expectedBlockSize) {
			break; // Ждем следущую порцию данных.
		}

		// Читаем данные из сокета
		QScopedArrayPointer<char> data(new char[_expectedBlockSize]);
		int countRead = clientSocket->read(data.data(), _expectedBlockSize);

		// Проверяем, все ли считалось (да, это параноя, но все же...)
		if(countRead != _expectedBlockSize) {
			qCritical() << tr("Expected data size in not equal to readed data size");
			break;
		}

		// Десериализуем полученное сообщение
		ai::proto::AiMessage message;
		ai::protocol::ProtocolHelper::deserializeMessage(&message, data.data(), _expectedBlockSize);

		processMessage(message);

		_expectedBlockSize = 0;
		emit messageRecieved(message.DebugString().c_str());
	}
}

void TcpClient::tryToConnect()
{
	_tcpSocket->connectToHost("127.0.0.1", 50200);
}

void TcpClient::onConnected()
{
	_timer.stop();
	_isConnected = true;

	sendHandShake();
	sendParameters();
	sendStatus();

}

void TcpClient::onDisconnected()
{
	_timer.start();
	_isConnected = false;
}
