////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_SETTING_INTEGRATORSETTINGSLOADER_H
#define AI_SETTING_INTEGRATORSETTINGSLOADER_H

#include "Kernel_Export.h"
#include "internal/Ai_ExecutionModule.pb.h"



class QXmlStreamReader;

namespace ai {

namespace kernel {
class ModuleGroup;
class WorkStation;
class IntegratorOptions;
}

namespace setting {

/**
 * Класс, отвечающий за загрузку параметров Игтегратора из файлов настроек
 */
class Kernel_EXPORT IntegratorSettingsLoader
{
public:
	IntegratorSettingsLoader() = delete;

public:
	static void load(kernel::IntegratorOptions& integratorOptions);

private:
	static void loadIntegratorSettings(kernel::IntegratorOptions& integratorOptions);
	static void addModuleGroups(QXmlStreamReader& xml, ai::kernel::WorkStation* workStation);
	static void addExecutionModules(QXmlStreamReader& xml, ai::kernel::ModuleGroup* moduleGroup);

	static void loadExecutionModuleSettings(kernel::IntegratorOptions& integratorOptions, const QString& filePath);

private:
	static ai::proto::StateType cardTypeFromString(const QString& cardTypeString);
	static ai::kernel::ExecutionModule_CardBgColor cardBgColorFromString(const QString& cardBgString);
};

} // namespace setting
} // namespace ai

#endif // AI_SETTING_INTEGRATORSETTINGSLOADER_H
