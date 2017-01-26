////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_ICON_H
#define AI_MAIN_ICON_H

#include "host/Ai_Icons.pb.h"

#include <QColor>
#include <QHash>
#include <QObject>



namespace ai {

namespace kernel {
class ExecutionModule;
}

namespace main {

class StatusIcon : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool    isValid        MEMBER _isValid          NOTIFY isValidChanged)
	Q_PROPERTY(QString description    MEMBER _description      NOTIFY descriptionChanged)
	Q_PROPERTY(bool    isStandardIcon MEMBER _isStandardIcon   NOTIFY isStandardIconChanged)
	Q_PROPERTY(QString standardIcon   MEMBER _standardIcon     NOTIFY standardIconChanged)
	Q_PROPERTY(QString customIconPath MEMBER _customIconPath   NOTIFY customIconPathChanged)
	Q_PROPERTY(QColor  color          MEMBER _color            NOTIFY colorChanged)

signals:
	void isValidChanged();
	void descriptionChanged();
	void isStandardIconChanged();
	void standardIconChanged();
	void customIconPathChanged();
	void colorChanged();

public:
	explicit StatusIcon(QObject* parent = nullptr);
	void update(const ai::kernel::ExecutionModule& em, const proto::StatusIcon& icon);
	void setValid(bool valid);
	void setCustomIcon(const QString& path);

private:
	bool                    _isValid;
	QString                 _description;
	bool                    _isStandardIcon;
	QString                 _standardIcon;
	QColor                  _color;
	QHash<QString, QString> _customIconFiles;
	QString                 _customIconPath;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_ICON_H
