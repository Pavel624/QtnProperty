#include "DoubleSpinBox.h"

QtnDoubleSpinBox::QtnDoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
	, percentSuffix(false)
{

}

bool QtnDoubleSpinBox::getHavePercentSuffix() const
{
	return percentSuffix;
}

void QtnDoubleSpinBox::setHavePercentSuffix(bool value)
{
	if (percentSuffix != value)
	{
		percentSuffix = value;
		update();
	}
}

QValidator::State QtnDoubleSpinBox::validate(QString &input, int &pos) const
{
	if (percentSuffix)
	{
		auto value = getNumberText(input);

		auto result = QDoubleSpinBox::validate(value, pos);

		if (QValidator::Acceptable == result)
		{
			value.append(QLocale().percent());
			input = value;
		}

		return result;
	}

	return QDoubleSpinBox::validate(input, pos);
}

double QtnDoubleSpinBox::valueFromText(const QString &text) const
{
	return QDoubleSpinBox::valueFromText(getNumberText(text));
}

QString QtnDoubleSpinBox::textFromValue(double val) const
{
	auto result = valueToText(val, locale(), decimals(), isGroupSeparatorShown());

	if (percentSuffix)
		result.append(locale().percent());

	return result;
}

QString QtnDoubleSpinBox::valueToText(double value, const QLocale &locale, int decimals, bool groupSeparatorShown)
{
	auto result = locale.toString(value, 'f', decimals);

	auto groupSeparator = locale.groupSeparator();
	if (!groupSeparatorShown)
		result.remove(groupSeparator);

	auto decimalPoint = locale.decimalPoint();
	auto zeroDigit = locale.zeroDigit();
	int i = result.indexOf(decimalPoint);
	if (i >= 0)
	{
		auto begin = result.constData();
		auto data = &begin[result.length() - 1];
		auto decBegin = &begin[i];

		while (data >= decBegin && (	*data == zeroDigit
									||	*data == decimalPoint
									||	*data == groupSeparator))
		{
			data--;
		}

		result.resize(data + 1 - begin);
	}

	return result;
}

QString QtnDoubleSpinBox::getNumberText(const QString &source) const
{
	auto result = source;
	if (percentSuffix)
	{
		int new_len = result.length() - 1;
		if (new_len > 0)
		{
			auto c = result.at(new_len);
			if (c == '%' || c == QLocale().percent())
				result.resize(new_len);
		}
	}

	return result;
}