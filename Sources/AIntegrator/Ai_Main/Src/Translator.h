////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_TRANSLATOR_H
#define AI_MAIN_TRANSLATOR_H

#include <QObject>



class QTranslator;

namespace ai {
namespace main {

/**
 * Вспомогательный класс, который используется при смене языка для обновления строк на UI
 * При смене языка генерируется сигнал languageChanged() который обновляет проперти tr
 * В QML при создании строчки, которая должна быть переводимой, нужно добавить проперти tr данного объекта
 */
class Translator : public QObject
{
	Q_OBJECT
	 Q_PROPERTY(QString tr READ getEmptyString NOTIFY languageChanged)

signals:
	void languageChanged();

public:
	explicit Translator(QObject* parent = nullptr);

	QString getEmptyString();
	Q_INVOKABLE void selectLanguage(quint32 languageId = 0);

private:
	void loadSettings();
	void saveSettings(const QString& lang);

private:
	QTranslator* _translatorRu;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_TRANSLATOR_H
