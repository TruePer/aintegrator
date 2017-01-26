////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CardModel.h"
#include "internal/Ai_Integrator.pb.h"
#include "CardState/SimpleCard.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::main;

CardModel::CardModel(QObject* parent)
    : QAbstractListModel(parent)
    , _appState(nullptr)
{
}

QString CardModel::cardId(const std::string& executionModuleId, const std::string& cardId)
{
	return QString("%1-%2").arg(QString::fromStdString(executionModuleId)).arg(QString::fromStdString(cardId));
}

int CardModel::rowCount(const QModelIndex& /*index*/) const
{
	return _cards.size();
}

QVariant CardModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();

	auto row = index.row();

	if(row >= _cardIds.count())
		return QVariant();

	auto id = _cardIds[row];

	switch (role) {
	case Card:
		return QVariant::fromValue(_cards[id]);
	}

	return QVariant();
}

QHash<int, QByteArray> CardModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Card] = "_card";
	return roles;
}

void CardModel::swap(int firstIndex, int secondIndex)
{
	if(firstIndex < 0 || secondIndex < 0)
		return;

	if(firstIndex == secondIndex)
		return;

	if(firstIndex < _cards.count() && secondIndex < _cards.count()) {
		if(firstIndex > secondIndex) {
			beginMoveRows(QModelIndex(), firstIndex, firstIndex, QModelIndex(), secondIndex);
		}
		else {
			beginMoveRows(QModelIndex(), firstIndex, firstIndex, QModelIndex(), secondIndex + 1);
		}
		_cardIds.swap(firstIndex, secondIndex);
		endMoveRows();
	}
}

void CardModel::update(const ai::state::AppState* state)
{
	_appState = state;

	auto emList = ai::protocol::ProtocolHelper::executionModules(*_appState->integratorOptions(), _workStation, _moduleGroup);

	for(auto& em : emList) {
		for(int i = 0, count = em.cards_size(); i < count; ++i) {
			const auto& card = em.cards(i);
			auto id = cardId(em.id(), card.id());

			if(!_cards.contains(id)) {
				beginInsertRows(QModelIndex(), _cardIds.count(), _cardIds.count());

				auto* card = new SimpleCard(this);
				_cards.insert(id, card);
				_cardIds << id;

				connect( card, SIGNAL(showModuleParameters(QString,QString)), SIGNAL(showModuleParameters(QString,QString)) );

				endInsertRows();
			}

			auto* cardInfo = _cards[id];
			cardInfo->setCard(em, card);
		}
	}
}
