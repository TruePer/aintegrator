////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_CARDMODEL_H
#define AI_MAIN_CARDMODEL_H

#include "Common/IObserver.h"
#include <QAbstractListModel>



namespace ai {

namespace state {
class AppState;
}

namespace main {

class SimpleCard;

/**
 * Модель для представления карточек запускаемых модлей на QML
 */
class CardModel : public QAbstractListModel, public common::IObserver
{
	Q_OBJECT
	Q_PROPERTY(QString workStation MEMBER _workStation)
	Q_PROPERTY(QString moduleGroup MEMBER _moduleGroup)

signals:
	void showModuleParameters(const QString& moduleId, const QString& moduleName); // Signal uses in QML
	void sendCommand(const ai::command::ICommand::SPtr command);

public:
	enum Roles {
		Card = Qt::UserRole + 1
	};

public:
	CardModel(QObject* parent = nullptr);

public:
	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

public:
	Q_INVOKABLE void swap(int firstIndex, int secondIndex);

private slots:
	void update(const ai::state::AppState* state);

private:
	static QString cardId(const std::string& executionModuleId, const std::string& cardId);

private:
	QHash<QString, SimpleCard*>    _cards;
	QList<QString>                 _cardIds;
	const ai::state::AppState*     _appState;
	QString                        _workStation;
	QString                        _moduleGroup;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_CARDMODEL_H
