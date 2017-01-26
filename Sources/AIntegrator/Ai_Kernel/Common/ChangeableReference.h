////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CHANGEABLEREFERENCE_H
#define CHANGEABLEREFERENCE_H

#include "Kernel_Export.h"

#include <QtGlobal>



namespace ai {
namespace common {

/**
 * @brief Шаблон для создания ссылочных экземпляров, по которому можно судить, был ли изменен
 * объект со времени последнего обновления
 * Создает обертку для объекта
 * Такой объект добавляется в обертку посредством setRef().
 * Изменеился ли объект, проверяется оператором bool()
 * Для получения объекта из обертки используется ref()
 * Если объект составной (класс, структура), то для доступа к элементам можно пользоваться
 * [ operator-> () ] или же [ ref(). ]
 * Если используются неконстантные версии operator->() или ref(), тосчитается, что объект подвергся модификации
 */
template<typename ReferenceType>
class Kernel_EXPORT ChangeableReference
{
public:
	ChangeableReference()
		: _wasChanged(true)
	{
	}

	ChangeableReference(const ReferenceType& param)
		: _wasChanged(true)
		, _ref(param)
	{
	}

	inline operator bool() const
	{
		return _wasChanged;
	}

	void setUnchanged()
	{
		_wasChanged = false;
	}

	inline ReferenceType& ref()
	{
		_wasChanged = true;
		return _ref;
	}

	inline const ReferenceType& ref() const
	{
		return _ref;
	}

	inline ReferenceType* operator->()
	{
		_wasChanged = true;
		return &_ref;
	}

	inline const ReferenceType* operator->() const
	{
		return &_ref;
	}

	inline void setRef(const ReferenceType& reference)
	{
		_wasChanged = true;
		_ref = reference;
	}

private:
	bool _wasChanged;
	ReferenceType _ref;
};

}
}

#endif // CHANGEABLEREFERENCE_H
