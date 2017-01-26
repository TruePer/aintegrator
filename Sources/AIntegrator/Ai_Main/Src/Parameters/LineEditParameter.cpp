////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LineEditParameter.h"
#include "host/Ai_PostParameters.pb.h"

#include <limits>
#include <QIntValidator>



using namespace ai::main;

LineEditParameter::LineEditParameter(QObject* parent)
    : BaseParameter(parent)
    , _type(Unknown)
    , _maxSymbols(100)
    , _validator(nullptr)
    , _lineEdit(nullptr)
{
}

void LineEditParameter::update(ai::proto::Parameter* parameter)
{
	if(!parameter->has_line_edit())
		return;

	_lineEdit = parameter->mutable_line_edit();

	if(_lineEdit->has_max_symbols()) {
		_maxSymbols = _lineEdit->max_symbols();
	}

	auto type = static_cast<ParameterType>(_lineEdit->ui_type());

	if(_type != type) {
		_type = type;

		delete _validator;
		_validator = nullptr;

		switch(_type) {
		case SignedInt:
			_validator = new QRegExpValidator(QRegExp("[+-]?\\d+"), this);
			break;

		case UnsignedInt:
			_validator = new QRegExpValidator(QRegExp("\\d+"), this);
			break;

		case SignedDouble:
			_validator = new QRegExpValidator(QRegExp("[+-]?\\d*\\.?\\d+"), this);
			break;

		case StringValue: {
			auto regExp = QRegExp(QString(".{0,%1}").arg(_maxSymbols));
			_validator = new QRegExpValidator(regExp, this);
			break;
		}
		}

		emit changed();
	}

	if(_type == SignedInt && _lineEdit->has_signed_int()) {
		setValue(QString::number(_lineEdit->signed_int()));
	}

	if(_type == UnsignedInt && _lineEdit->has_unsigned_int()) {
		setValue(QString::number(_lineEdit->unsigned_int()));
	}

	if(_type == SignedDouble && _lineEdit->has_signed_double()) {
		setValue(QString::number(_lineEdit->signed_double()));
	}

	if(_type == StringValue && _lineEdit->has_string_value()) {
		setValue(QString::fromStdString(_lineEdit->string_value()));
	}
}

void LineEditParameter::setValue(const QString& value)
{
	if(_lineEdit) {
		if(_type == SignedInt) {
			_lineEdit->set_signed_int(value.toInt());
		}

		if(_type == UnsignedInt) {
			_lineEdit->set_unsigned_int(value.toUInt());
		}

		if(_type == SignedDouble) {
			_lineEdit->set_signed_double(value.toDouble());
		}

		if(_type == StringValue) {
			_lineEdit->set_string_value(value.toStdString());
		}
	}

	if(_value != value) {
		_value = value;
		emit changed();
	}
}
