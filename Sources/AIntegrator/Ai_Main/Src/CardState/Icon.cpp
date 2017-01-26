////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Icon.h"
#include "Logger.h"
#include "internal/Ai_ExecutionModule.pb.h"

#include <QDebug>



using namespace ai::main;

StatusIcon::StatusIcon(QObject* parent)
    : QObject(parent)
    , _isValid(false)
    , _isStandardIcon(true)
{
}

void StatusIcon::update(const ai::kernel::ExecutionModule& em, const ai::proto::StatusIcon& icon)
{
	setValid(true);

	_customIconFiles.clear();

	for(int i = 0, size = em.status_files_size(); i < size; ++i) {
		const auto& statusFile = em.status_files(i);
		_customIconFiles.insert(QString::fromStdString(statusFile.id()), QString::fromStdString(statusFile.path()));
	}

	// Обновим описание
	auto descr = QString::fromStdString(icon.description());

	if(_description != descr) {
		_description = descr;
		emit descriptionChanged();
	}

	// Проверим тип иконки
	bool isStandardIcon = icon.type() == ai::proto::SIT_STANDARD;

	if(_isStandardIcon != isStandardIcon) {
		_isStandardIcon = isStandardIcon;
		emit isStandardIconChanged();
	}

	if(isStandardIcon && icon.has_standard_icon()) {
		const auto& standardIcon = icon.standard_icon();

		// Зададим простую иконку
		QString standardIconName;

		switch (standardIcon.type()) {
		case ai::proto::IT_CIRCLE:     standardIconName = "image/brightness_1";                     break;
		case ai::proto::IT_RING:       standardIconName = "toggle/radio_button_unchecked";          break;
		case ai::proto::IT_RHOMBUS:    standardIconName = "maps/beenhere";                          break;
		case ai::proto::IT_SQUARE:     standardIconName = "toggle/check_box_outline_blank";         break;
		case ai::proto::IT_TRIANGLE:   standardIconName = "action/change_history";                  break;

		case ai::proto::IT_BACKWARD:   standardIconName = "av/fast_rewind";                         break;
		case ai::proto::IT_FORWARD:    standardIconName = "av/fast_forward";                        break;
		case ai::proto::IT_PLAY:       standardIconName = "av/play_arrow";                          break;
		case ai::proto::IT_PAUSE:      standardIconName = "av/pause";                               break;
		case ai::proto::IT_STOP:       standardIconName = "av/stop";                                break;

		case ai::proto::IT_GROUP:      standardIconName = "social/people";                          break;
		case ai::proto::IT_OPERATOR:   standardIconName = "social/person";                          break;
		case ai::proto::IT_USER:       standardIconName = "communication/contacts";                 break;

		case ai::proto::IT_CLOUD:      standardIconName = "file/cloud";                             break;
		case ai::proto::IT_COMPUTER:   standardIconName = "hardware/computer";                      break;
		case ai::proto::IT_DATABASE:   standardIconName = "device/storage";                         break;
		case ai::proto::IT_DIRECTORY:  standardIconName = "file/folder";                            break;
		case ai::proto::IT_FILE:       standardIconName = "action/description";                     break;
		case ai::proto::IT_LINK:       standardIconName = "content/link";                           break;
		case ai::proto::IT_MEMORYCARD: standardIconName = "notification/sd_card";                   break;
		case ai::proto::IT_MONITOR:    standardIconName = "hardware/desktop_windows";               break;
		case ai::proto::IT_PRINTER:    standardIconName = "action/print";                           break;
		case ai::proto::IT_WEB:        standardIconName = "social/public";                          break;

		case ai::proto::IT_MICROPHONE: standardIconName = "av/mic";                                 break;
		case ai::proto::IT_SPEAKER:    standardIconName = "hardware/speaker";                       break;
		case ai::proto::IT_CAMERA:     standardIconName = "image/photo_camera";                     break;

		case ai::proto::IT_DF:         standardIconName = "action/explore";                         break;
		case ai::proto::IT_HARDWARE:   standardIconName = "hardware/memory";                        break;
		case ai::proto::IT_LOCATION:   standardIconName = "maps/place";                             break;
		case ai::proto::IT_MAP:        standardIconName = "maps/map";                               break;
		case ai::proto::IT_NETWORK:    standardIconName = "social/share";                           break;
		case ai::proto::IT_RADAR:      standardIconName = "action/settings_input_antenna";          break;
		case ai::proto::IT_WIRELESS:   standardIconName = "notification/wifi";                      break;
		case ai::proto::IT_SPECTRUM:   standardIconName = "av/equalizer";                           break;
		case ai::proto::IT_CALL:       standardIconName = "communication/call";                     break;
		case ai::proto::IT_CALL_END:   standardIconName = "communication/call_end";                 break;
		case ai::proto::IT_SMS:        standardIconName = "communication/message";                  break;

		case ai::proto::IT_INFO:       standardIconName = "action/info";                            break;
		case ai::proto::IT_WARNING:    standardIconName = "alert/warning";                          break;
		case ai::proto::IT_ERROR:      standardIconName = "alert/error";                            break;
		case ai::proto::IT_REPORT:     standardIconName = "content/report";                         break;
		case ai::proto::IT_FLAG:       standardIconName = "content/flag";                           break;
		case ai::proto::IT_BLOCK:      standardIconName = "content/block";                          break;
		}

		if(_standardIcon != standardIconName) {
			_standardIcon = standardIconName;
			emit standardIconChanged();
		}

		// Зададим цвет простой иконки
		QColor color;

		switch (standardIcon.color()) {
		case ai::proto::IC_BLUE:
			color = "blue";
			break;

		case ai::proto::IC_GRAY:
			color = "gray";
			break;

		case ai::proto::IC_GREEN:
			color = "green";
			break;

		case ai::proto::IC_PURPLE:
			color = "purple";
			break;

		case ai::proto::IC_RED:
			color = "#d75752";
			break;

		case ai::proto::IC_YELLOW:
			color = "yellow";
			break;
		}

		if(_color != color) {
			_color = color;
			emit colorChanged();
		}
	}

	if(!isStandardIcon && icon.has_custom_icon()) {
		auto iconId = QString::fromStdString(icon.custom_icon().custom_file_id());
		setCustomIcon(_customIconFiles.value(iconId, QString()));
	}
}

void StatusIcon::setValid(bool valid)
{
	if(_isValid != valid) {
		_isValid = valid;
		emit isValidChanged();
	}
}

void StatusIcon::setCustomIcon(const QString& path)
{
	if(_customIconPath != path) {
		_customIconPath = path;
		emit customIconPathChanged();
	}
}
