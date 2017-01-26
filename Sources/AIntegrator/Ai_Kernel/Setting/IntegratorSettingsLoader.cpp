////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Config.h"
#include "IntegratorSettingsLoader.h"
#include "Logger.h"
#include "internal/Ai_Integrator.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>



using namespace ai::setting;

/**
 * Читает настройки Интегратора из конфигурационной директории
 * @param integratorOptions
 */
void IntegratorSettingsLoader::load(ai::kernel::IntegratorOptions& integratorOptions)
{
	loadIntegratorSettings(integratorOptions);

	QDir dir(CONFIGURATION_DIRECTORY);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList list = dir.entryInfoList(filters);

	ai::log() << QObject::tr("Modules loaded from configuration files:");

	for(const auto& fileInfo : list) {
		if(fileInfo.fileName() == QString(AI_CONFIGURATION_FILE))
			continue;

		loadExecutionModuleSettings(integratorOptions, fileInfo.absoluteFilePath());
	}

	auto modulesList = ai::protocol::ProtocolHelper::executionModules(integratorOptions);

	for(const auto& em : modulesList) {
		if(em.cards_size() || em.has_parameters() || em.has_timing_parameters())
			continue;

		ai::error() << QObject::tr("Module '%1' has no properly configuration file!").arg(QString::fromStdString(em.id()));
	}
}

/**
 * Читает настройки из конфигурационного файла Интегратора
 * @param integratorOptions
 */
void IntegratorSettingsLoader::loadIntegratorSettings(ai::kernel::IntegratorOptions& integratorOptions)
{
	QFile file(QString("%1/%2").arg(CONFIGURATION_DIRECTORY).arg(AI_CONFIGURATION_FILE));

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		ai::error() << QObject::tr("Can not load Integrator settings from file %1").arg(file.fileName());
		return;
	}

	QXmlStreamReader xml(&file);

	while(!xml.atEnd() && !xml.hasError()) {
		xml.readNextStartElement();

		if(xml.tokenType() == QXmlStreamReader::StartDocument)
			continue;

		if(xml.tokenType() != QXmlStreamReader::StartElement)
			continue;

		// Читаем настройки Интегратора
		if(xml.name() == "a_integrator") {
			if(xml.isEndElement())
				continue;

			QString name = QObject::tr("A-Intagrator");

			if(xml.attributes().hasAttribute("name")) {
				name = xml.attributes().value("name").toString();
			}

			integratorOptions.set_name(QString("%1 - %2.%3.%4").arg(name).arg(MAJOR_VER).arg(MINOR_VER).arg(REVISION_VER).toStdString());

			integratorOptions.mutable_connection()->set_type(kernel::ConnectionOptions_ConnectionType_CT_TCP);
			auto* tcp = integratorOptions.mutable_connection()->mutable_tcp();
			tcp->set_ip_address("127.0.0.1");
			tcp->set_port(50200);

			if(xml.attributes().hasAttribute("ip_address")) {
				tcp->set_ip_address(xml.attributes().value("ip_address").toString().toStdString());
			}

			if(xml.attributes().hasAttribute("port")) {
				tcp->set_port(xml.attributes().value("port").toUInt());
			}

			continue;
		}

		// Читаем настройки АРМ
		if(xml.name() == "work_station") {
			if(xml.isEndElement())
				continue;

			auto* workStation = integratorOptions.add_work_stations();

			if(xml.attributes().hasAttribute("name")) {
				workStation->set_name(xml.attributes().value("name").toString().toStdString());
			}
			else {
				workStation->set_name(QObject::tr("ARM %1").arg(integratorOptions.work_stations_size()).toStdString());
			}

			addModuleGroups(xml, workStation);
		}
	}
}

/**
 * Вычитывает группы запускаемых модулей,входящие в АРМ, из конфигурационного файла Интегратора
 * @param xml
 * @param workStation
 */
void IntegratorSettingsLoader::addModuleGroups(QXmlStreamReader& xml, ai::kernel::WorkStation* workStation)
{
	while(!xml.atEnd() && !xml.hasError()) {
		xml.readNextStartElement();

		if(xml.name() == "work_station" && xml.isEndElement())
			break;

		if(xml.name() == "module_group") {
			if(xml.isEndElement())
				continue;

			auto* moduleGroup = workStation->add_module_groups();

			if(xml.attributes().hasAttribute("name")) {
				moduleGroup->set_name(xml.attributes().value("name").toString().toStdString());
			}

			addExecutionModules(xml, moduleGroup);
		}
	}
}

/**
 * Вычитывает запускаемые модули, входящие в группу, из конфигурационного файла Интегратора
 * @param xml
 * @param moduleGroup
 */
void IntegratorSettingsLoader::addExecutionModules(QXmlStreamReader& xml, ai::kernel::ModuleGroup* moduleGroup)
{
	while(!xml.atEnd() && !xml.hasError()) {
		xml.readNextStartElement();

		if(xml.name() == "module_group" && xml.isEndElement())
			break;

		if(xml.name() == "execution_module") {
			if(xml.isEndElement())
				continue;

			auto* executionModule = moduleGroup->add_execution_modules();
			executionModule->set_id(xml.attributes().value("id").toString().toLower().toStdString());

			if(xml.attributes().hasAttribute("name")) {
				executionModule->set_name(xml.attributes().value("name").toString().toStdString());
			}
		}
	}
}

/**
 * Вычитывает настройки запускаемого модуля из файла
 * @param integratorOptions
 * @param filePath
 */
void IntegratorSettingsLoader::loadExecutionModuleSettings(ai::kernel::IntegratorOptions& integratorOptions, const QString& filePath)
{
	QFile file(filePath);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		ai::error() << QObject::tr("Can not load Integrator settings");
		return;
	}

	QXmlStreamReader xml(&file);
	kernel::ExecutionModule executionModule;

	while(!xml.atEnd() && !xml.hasError()) {
		xml.readNextStartElement();

		if(xml.tokenType() == QXmlStreamReader::StartDocument)
			continue;

		if(xml.tokenType() != QXmlStreamReader::StartElement)
			continue;

		// Читаем настройки Интегратора
		if(xml.name() == "execution_module") {
			if(xml.isEndElement())
				break;

			if(!xml.attributes().hasAttribute("id")) {
				ai::error() << QObject::tr("File %1 has no id field. Execution module was ignored");
				break;
			}

			executionModule.set_id(xml.attributes().value("id").toString().toStdString());

			if(!executionModule.has_name() && xml.attributes().hasAttribute("name")) {
				executionModule.set_name(xml.attributes().value("name").toString().toStdString());
			}
			else {
				executionModule.set_name(QObject::tr("Post %1").arg(QString::fromStdString(executionModule.id())).toStdString());
			}
			continue;
		}

		if(xml.name() == "timing_parameters") {
			if(xml.isEndElement())
				continue;

			auto* timingParameters = executionModule.mutable_timing_parameters();

			// Установим значения тайминга по-умолчнию
			kernel::ExecutionModule::TimingParameters defaultTimingParameters;
			timingParameters->set_module_launch_delay_sec(defaultTimingParameters.module_launch_delay_sec());
			timingParameters->set_status_update_sec(defaultTimingParameters.status_update_sec());
			timingParameters->set_launch_interval_sec(defaultTimingParameters.launch_interval_sec());

			// Вычитаем настройки тайминга из файла
			if(xml.attributes().hasAttribute("module_launch_delay_sec")) {
				timingParameters->set_module_launch_delay_sec(xml.attributes().value("module_launch_delay_sec").toUInt());
			}

			if(xml.attributes().hasAttribute("status_update_sec")) {
				timingParameters->set_status_update_sec(xml.attributes().value("status_update_sec").toUInt());
			}
			if(xml.attributes().hasAttribute("launch_interval_sec")) {
				timingParameters->set_launch_interval_sec(xml.attributes().value("launch_interval_sec").toUInt());
			}

			continue;
		}

		// Список процессов для запуска модуля
		if(xml.name() == "launch_list") {
			if(xml.isEndElement())
				continue;

			while(!xml.atEnd() && !xml.hasError()) {
				xml.readNextStartElement();

				if(xml.name() == "launch_list" && xml.isEndElement())
					break;

				if(xml.name() == "launch_parameters") {
					if(xml.isEndElement())
						continue;

					auto* launchParameters = executionModule.add_launch_paramerers();
					launchParameters->set_path_to_executable(xml.attributes().value("path_to_executable").toString().toStdString());
					launchParameters->set_command_options(xml.attributes().value("command_options").toString().toStdString());
					launchParameters->set_work_directory(xml.attributes().value("work_directory").toString().toStdString());

					continue;
				}
			}

			continue;
		}

		// Список карточек поста
		if(xml.name() == "card_list") {
			if(xml.isEndElement())
				continue;

			while(!xml.atEnd() && !xml.hasError()) {
				xml.readNextStartElement();

				if(xml.name() == "card_list" && xml.isEndElement())
					break;

				if(xml.name() == "card") {
					if(xml.isEndElement())
						continue;

					auto* card = executionModule.add_cards();

					if(xml.attributes().hasAttribute("id")) {
						card->set_id(xml.attributes().value("id").toString().toStdString());
					} else {
						ai::error() << QObject::tr("Card of execution module '%1' don't have id").arg(executionModule.name().c_str());
					}

					card->set_type(cardTypeFromString(xml.attributes().value("type").toString()));
					card->set_bg_color(cardBgColorFromString(xml.attributes().value("background_color").toString()));

					continue;
				}
			}

			continue;
		}

		// Список режимов работы
		if(xml.name() == "work_mode_list") {
			if(xml.isEndElement())
				continue;

			while(!xml.atEnd() && !xml.hasError()) {
				xml.readNextStartElement();

				if(xml.name() == "work_mode_list" && xml.isEndElement())
					break;

				if(xml.name() == "work_mode") {
					if(xml.isEndElement())
						continue;

					auto* workMode = executionModule.add_work_modes();

					if(xml.attributes().hasAttribute("id")) {
						workMode->set_id(xml.attributes().value("id").toString().toStdString());
					} else {
						ai::error() << QObject::tr("Work mode of execution module '%1' don't have id").arg(executionModule.name().c_str());
					}

					workMode->set_name(xml.attributes().value("name").toString().toStdString());

					continue;
				}
			}

			continue;
		}

		// Список статус-иконок
		if(xml.name() == "status_file_list") {
			if(xml.isEndElement())
				continue;

			while(!xml.atEnd() && !xml.hasError()) {
				xml.readNextStartElement();

				if(xml.name() == "status_file_list" && xml.isEndElement())
					break;

				if(xml.name() == "status_file") {
					if(xml.isEndElement())
						continue;

					auto* statusFile = executionModule.add_status_files();

					QString id;
					if(xml.attributes().hasAttribute("id")) {
						id = xml.attributes().value("id").toString();
						statusFile->set_id(id.toStdString());
					} else {
						ai::error() << QObject::tr("Status file of execution module '%1' don't have id").arg(executionModule.name().c_str());
					}

					auto filePath = xml.attributes().value("path").toString();
					statusFile->set_path(filePath.toStdString());

					QFileInfo fi(filePath);
					if(!fi.exists()) {
						ai::warn() << QObject::tr("Status icon file %1 id %2 is not exists!")
						               .arg(filePath)
						               .arg(id);
					}

					continue;
				}
			}

			continue;
		}

	}

	if(!executionModule.has_id()) {
		ai::error() << QObject::tr("File %1 has no information about execution module and was ignored");
	}

	// Найдем запускаемый модуль в уже загруженных настройках интегратора по его id
	for(int i = 0, size = integratorOptions.work_stations_size(); i < size; ++i) {
		auto* workStation = integratorOptions.mutable_work_stations(i);

		for(int i = 0, size = workStation->module_groups_size(); i < size; ++i) {
			auto* moduleGroup = workStation->mutable_module_groups(i);

			for(int i = 0, size = moduleGroup->execution_modules_size(); i < size; ++i) {
				auto* moduleFromIntegrator = moduleGroup->mutable_execution_modules(i);

				// Нашли, копируем
				if(executionModule.id() == moduleFromIntegrator->id()) {
					std::string name;
					if(moduleFromIntegrator->has_name()) {
						name = moduleFromIntegrator->name();
					}

					moduleFromIntegrator->CopyFrom(executionModule);

					if(name.size()) {
						moduleFromIntegrator->set_name(name);
					}

					ai::log() << QObject::tr(" - ID: %1, name: '%2'")
					             .arg(QString::fromStdString(moduleFromIntegrator->id()))
					             .arg(QString::fromStdString(moduleFromIntegrator->name()));
				}
			}
		}
	}
}

ai::proto::StateType IntegratorSettingsLoader::cardTypeFromString(const QString& cardTypeString)
{
	if(cardTypeString.toLower() == "simple")
		return ai::proto::ST_SIMPLE;

	return ai::proto::ST_UNKNOWN;
}

ai::kernel::ExecutionModule_CardBgColor IntegratorSettingsLoader::cardBgColorFromString(const QString& cardBgString)
{
	auto bgColor = cardBgString.toLower();

	if(bgColor == "red")
		return ai::kernel::ExecutionModule_CardBgColor_BG_RED;

	if(bgColor == "green")
		return ai::kernel::ExecutionModule_CardBgColor_BG_GREEN;

	if(bgColor == "blue")
		return ai::kernel::ExecutionModule_CardBgColor_BG_BLUE;

	if(bgColor == "lightblue")
		return ai::kernel::ExecutionModule_CardBgColor_BG_LIGHTBLUE;

	if(bgColor == "yellow")
		return ai::kernel::ExecutionModule_CardBgColor_BG_YELLOW;

	if(bgColor == "purple")
		return ai::kernel::ExecutionModule_CardBgColor_BG_PURPLE;

	if(bgColor == "cyan")
		return ai::kernel::ExecutionModule_CardBgColor_BG_CYAN;

	if(bgColor == "orange")
		return ai::kernel::ExecutionModule_CardBgColor_BG_ORANGE;

	if(bgColor == "teal")
		return ai::kernel::ExecutionModule_CardBgColor_BG_TEAL;

	if(bgColor == "indigo")
		return ai::kernel::ExecutionModule_CardBgColor_BG_INDIGO;

	if(bgColor == "grey")
		return ai::kernel::ExecutionModule_CardBgColor_BG_GREY;

	if(bgColor == "bluegrey")
		return ai::kernel::ExecutionModule_CardBgColor_BG_BLUEGREY;

	return ai::kernel::ExecutionModule_CardBgColor_BG_UNKNOWN;
}
