#include "variant.hpp"
#include <string.h>

namespace VM
{

bool Variable::hasValue() const
{
	if (reference_) {
		if (referenceIndeces_[3] == 0) {
			return reference_->hasValue();
		} else if (referenceIndeces_[3] == 1) {
			return reference_->hasValue(referenceIndeces_[0]);
		} else if (referenceIndeces_[3] == 2) {
			return reference_->hasValue(referenceIndeces_[0], referenceIndeces_[1]);
		} else {
			return reference_->hasValue(referenceIndeces_[0], referenceIndeces_[1], referenceIndeces_[2]);
		}
	} else {
		return value_.isValid();
	}
}


bool Variable::hasValue(int indeces[4]) const
{
	if (indeces[3] == 1) {
		return hasValue(indeces[0]);
	} else if (indeces[3] == 2) {
		return hasValue(indeces[0], indeces[1]);
	} else if (indeces[3] == 3) {
		return hasValue(indeces[0], indeces[1], indeces[2]);
	} else {
		return hasValue();
	}
}

AnyValue Variable::value() const
{
	if (reference_) {
		if (referenceIndeces_[3] == 0) {
			return reference_->value();
		} else if (referenceIndeces_[3] == 1) {
			return reference_->value(referenceIndeces_[0]);
		} else if (referenceIndeces_[3] == 2) {
			return reference_->value(referenceIndeces_[0], referenceIndeces_[1]);
		} else if (referenceIndeces_[3] == 3) {
			return reference_->value(referenceIndeces_[0], referenceIndeces_[1], referenceIndeces_[2]);
		}
	} else {
		if (!value_.isValid() && !ignoreUndefinedError) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Нет значения у величины"));
		}
		return value_;
	}
	return value_;
}



void Variable::setValue(const AnyValue &v)
{
	if (reference_) {
		if (referenceIndeces_[3] == 0) {
			reference_->setValue(v);
		} else if (referenceIndeces_[3] == 1) {
			reference_->setValue(referenceIndeces_[0], v);
		} else if (referenceIndeces_[3] == 2) {
			reference_->setValue(referenceIndeces_[0], referenceIndeces_[1], v);
		} else if (referenceIndeces_[3] == 3) {
			reference_->setValue(referenceIndeces_[0], referenceIndeces_[1], referenceIndeces_[2], v);
		}

	} else {
		value_ = v;
	}

}




String Variable::toString() const
{
	String result;
	switch (baseType_) {
	case VT_bool:
		if (value().toBool()) {
			result = Kumir::Core::fromUtf8("да");
		} else {
			result = Kumir::Core::fromUtf8("нет");
		}
		break;
	case VT_real:
		result = Kumir::Converter::sprintfReal(value().toReal(), '.', false, 0, -1, 0);
		break;
	case VT_int:
		result = Kumir::Converter::sprintfInt(value().toInt(), 10, 0, 0);
		break;
	case VT_char:
		result.push_back(value().toChar());
		break;
	case VT_string:
		result = value().toString();
		break;
	default:
		break;
	}
	return result;
}

String Variable::toString(int indeces[4]) const
{
	String result;
	switch (baseType_) {
	case VT_bool:
		if (value(indeces).toBool()) {
			result = Kumir::Core::fromUtf8("да");
		} else {
			result = Kumir::Core::fromUtf8("нет");
		}
		break;
	case VT_real:
		result = Kumir::Converter::sprintfReal(value(indeces).toReal(), '.', false, 0, -1, 0);
		break;
	case VT_int:
		result = Kumir::Converter::sprintfInt(value(indeces).toInt(), 10, 0, 0);
		break;
	case VT_char:
		result.push_back(value(indeces).toChar());
		break;
	case VT_string:
		result = value(indeces).toString();
		break;
	default:
		break;
	}
	return result;
}

AnyValue Variable::value(int indeces[4]) const
{
	if (indeces[3] == 1) {
		return value(indeces[0]);
	} else if (indeces[3] == 2) {
		return value(indeces[0], indeces[1]);
	} else if (indeces[3] == 3) {
		return value(indeces[0], indeces[1], indeces[2]);
	} else {
		return value();
	}
}

void Variable::setValue(int indeces[4], const AnyValue &value)
{
	if (indeces[3] == 1) {
		setValue(indeces[0], value);
	} else if (indeces[3] == 2) {
		setValue(indeces[0], indeces[1], value);
	} else if (indeces[3] == 3) {
		setValue(indeces[0], indeces[1], indeces[2], value);
	} else {
		setValue(value);
	}
}


size_t Variable::linearIndex(int a) const
{
	return a - bounds_[0];
}

size_t Variable::linearIndex(int a, int b) const
{
	int size0 = bounds_[3] - bounds_[2] + 1;
	int offset0 = (a - bounds_[0]) * size0;
	int result = offset0 + b - bounds_[2];
	return result;
}

size_t Variable::linearIndex(int a, int b, int c) const
{
	int size0 = bounds_[3] - bounds_[2] + 1;
	int size1 = bounds_[5] - bounds_[4] + 1;
	return (a - bounds_[0]) * size0 * size1 + (b - bounds_[2]) * size1 + c - bounds_[4];
}


void Variable::setConstValue(const Variable &ctab)
{
	if (isReference()) {
		reference_->setConstValue(ctab);
		return;
	}
	const int dim = ctab.dimension();
	int cbounds[7];
	if (dim > 0) {
		ctab.getBounds(cbounds);
		for (int i = 0; i < dim; i++) {
			const int mysize = bounds_[2 * i + 1] - bounds_[2 * i];
			const int csize  = cbounds [2 * i + 1] - cbounds [2 * i];
			if (mysize < csize) {
				Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
				return;
			}
		}
	}
	switch (dim) {
	case 0: {
		setValue(ctab.value());
		break;
	}
	case 1: {
		const int cx = cbounds [0];
		const int mx = bounds_[0];
		const int sx = cbounds [1] - cbounds [0];
		for (int x = 0; x <= sx; x++) {
			setValue(mx + x, ctab.value(cx + x));
		}
		break;
	}
	case 2: {
		const int cy = cbounds [0];
		const int my = bounds_[0];
		const int cx = cbounds [2];
		const int mx = bounds_[2];
		const int sy = cbounds [1] - cbounds [0];
		const int sx = cbounds [3] - cbounds [2];
		for (int y = 0; y <= sy; y++) {
			for (int x = 0; x <= sx; x++) {
				setValue(my + y, mx + x, ctab.value(cy + y, cx + x));
				unsetError();
			}
		}
		break;
	}
	case 3: {
		const int cz = cbounds [0];
		const int mz = bounds_[0];
		const int cy = cbounds [2];
		const int my = bounds_[2];
		const int cx = cbounds [4];
		const int mx = bounds_[4];
		const int sz = cbounds [1] - cbounds [0];
		const int sy = cbounds [3] - cbounds [2];
		const int sx = cbounds [5] - cbounds [4];
		for (int z = 0; z < sz; z++) {
			for (int y = 0; y <= sy; y++) {
				for (int x = 0; x <= sx; x++) {
					setValue(mz + z, my + y, mx + x, ctab.value(cz + z, cy + y, cx + x));
				}
			}
		}
		break;
	}
	default: {
		break;
	}
	}
}

// DIM = 1

bool Variable::hasValue(int index0) const
{
	if (reference_) {
		return reference_->hasValue(index0);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 1) {
		return false;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1]) {
		return false;
	}
	int index = linearIndex(index0);
	return value_.isValid() && value_[index].isValid();
}

AnyValue Variable::value(int index0) const
{
	if (reference_) {
		return reference_->value(index0);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 1) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return AnyValue(VT_void);
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return AnyValue(VT_void);
	}
	int index = linearIndex(index0);
	if (value_[index].type() == VT_void) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Значение элемента таблицы не определено"));
		return AnyValue(VT_void);
	}
	return value_[index];
}

void Variable::setValue(int index0, const AnyValue &value)
{
	if (!reference_ && (value_.rawSize() == 0 || restrictedBounds_[6] < 1)) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return ;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return ;
	}
	if (reference_) {
		reference_->setValue(index0, value);
		return;
	}
	size_t index = linearIndex(index0);
	value_[index] = value;
}



// DIM = 2

bool Variable::hasValue(int index0, int index1) const
{
	if (reference_) {
		return reference_->hasValue(index0, index1);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 2) {
		return false;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3]) {
		return false;
	}
	size_t index = linearIndex(index0, index1);
	return value_.isValid() && value_[index].isValid();
}

AnyValue Variable::value(int index0, int index1) const
{
	if (reference_) {
		return reference_->value(index0, index1);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 2) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return AnyValue(VT_void);
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return AnyValue(VT_void);
	}
	size_t index = linearIndex(index0, index1);
	if (value_[index].type() == VT_void) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Значение элемента таблицы не определено"));
		return AnyValue(VT_void);
	}
	return value_[index];
}

void Variable::setValue(int index0, int index1, const AnyValue &value)
{
	if (!reference_ && (value_.rawSize() == 0 || restrictedBounds_[6] < 2)) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return ;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return ;
	}
	if (reference_) {
		reference_->setValue(index0, index1, value);
		return;
	}
	size_t index = linearIndex(index0, index1);
	value_[index] = value;
}


// DIM = 3

bool Variable::hasValue(int index0, int index1, int index2) const
{
	if (reference_) {
		return reference_->hasValue(index0, index1, index2);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 3) {
		return false;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3] || index2 < restrictedBounds_[4] || index2 > restrictedBounds_[5]) {
		return false;
	}
	size_t index = linearIndex(index0, index1, index2);
	return value_.isValid() && value_[index].isValid();
}

AnyValue Variable::value(int index0, int index1, int index2) const
{
	if (reference_) {
		return reference_->value(index0, index1, index2);
	}
	if (value_.rawSize() == 0 || restrictedBounds_[6] < 3) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return AnyValue(VT_void);
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3] || index2 < restrictedBounds_[4] || index2 > restrictedBounds_[5]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return AnyValue(VT_void);
	}
	size_t index = linearIndex(index0, index1, index2);
	if (value_[index].type() == VT_void) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Значение элемента таблицы не определено"));
		return AnyValue(VT_void);
	}
	return value_[index];
}

void Variable::setValue(int index0, int index1, int index2, const AnyValue &value)
{
	if (!reference_ && (value_.rawSize() == 0 || restrictedBounds_[6] < 3)) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Таблица не инициализирована"));
		return ;
	}
	if (index0 < restrictedBounds_[0] || index0 > restrictedBounds_[1] || index1 < restrictedBounds_[2] || index1 > restrictedBounds_[3] || index2 < restrictedBounds_[4] || index2 > restrictedBounds_[5]) {
		Kumir::Core::abort(Kumir::Core::fromUtf8("Выход за границу таблицы"));
		return ;
	}
	if (reference_) {
		reference_->setValue(index0, index1, index2, value);
		return;
	}
	size_t index = linearIndex(index0, index1, index2);
	value_[index] = value;
}

void Variable::init()
{
	if (reference_) {
		reference_->init();
		return;
	}
	if (dimension_ == 0) {
		value_ = VT_void;
	} else {
		if (dimension_ == 1) {
			for (int x = restrictedBounds_[0]; x <= restrictedBounds_[1]; x++) {
				size_t index = linearIndex(x);
				value_[index] = VT_void;
			}
		} else if (dimension_ == 2) {
			for (int y = restrictedBounds_[0]; y <= restrictedBounds_[1]; y++) {
				for (int x = restrictedBounds_[2]; x <= restrictedBounds_[3]; x++) {
					size_t index = linearIndex(y, x);
					value_[index] = VT_void;
				}
			}
		} else if (dimension_ == 3) {
			for (int z = restrictedBounds_[0]; z <= restrictedBounds_[1]; z++) {
				for (int y = restrictedBounds_[2]; y <= restrictedBounds_[3]; y++) {
					for (int x = restrictedBounds_[4]; x <= restrictedBounds_[5]; x++) {
						size_t index = linearIndex(z, y, x);
						value_[index] = VT_void;
					}
				}
			}
		}

	}
}

void Variable::setBounds(int bounds[7])
{
	size_t size = 0;
	dimension_ = bounds[6] / 2;
	if (dimension_ >= 1) {
		size = bounds[1] - bounds[0] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}
	if (dimension_ >= 2) {
		size *= bounds[3] - bounds[2] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}
	if (dimension_ >= 3) {
		size *= bounds[5] - bounds[4] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}

	value_.resize(size);

	memcpy(bounds_, bounds, 7 * sizeof(int));
	memcpy(restrictedBounds_, bounds_, 7 * sizeof(int));
}

void Variable::updateBounds(int bounds[7])
{
	size_t size = 0;
	dimension_ = bounds[6] / 2;
	if (dimension_ >= 1) {
		size = bounds[1] - bounds[0] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}
	if (dimension_ >= 2) {
		size *= bounds[3] - bounds[2] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}
	if (dimension_ >= 3) {
		size *= bounds[5] - bounds[4] + 1;
		if (size <= 0) {
			Kumir::Core::abort(Kumir::Core::fromUtf8("Неверный размер таблицы"));
			return;
		}
	}

	restrictedBounds_[6] = bounds[6];

	for (int i = 0; i < bounds[6]; i += 2) {
		restrictedBounds_[i] = bounds_[6]
			? Kumir::Math::imax(bounds_[i], bounds[i])
			: bounds[i];
	}
	for (int i = 1; i < bounds[6]; i += 2) {
		restrictedBounds_[i] = bounds_[6]
			? Kumir::Math::imin(bounds_[i], bounds[i])
			: bounds[i];
	}

}

void Variable::getBounds(/*out*/ int bounds[7]) const
{
	memcpy(bounds, bounds_, 7 * sizeof(int));
}

void Variable::getEffectiveBounds(/*out*/ int bounds[7]) const
{
	memcpy(bounds, restrictedBounds_, 7 * sizeof(int));
}


void Variable::setReference(Variable *r, int effectiveBounds[7])
{
	reference_ = r;
	memcpy(bounds_, effectiveBounds, 7 * sizeof(int));
	memcpy(restrictedBounds_, effectiveBounds, 7 * sizeof(int));
}

void Variable::setReferenceIndeces(int v[4])
{
	memcpy(referenceIndeces_, v, 4 * sizeof(int));
}

void Variable::getReferenceIndeces(int result[4]) const
{
	memcpy(result, referenceIndeces_, 4 * sizeof(int));
}


Variable Variable::toReference()
{
	Variable result;
	if (reference_) {
		result.reference_ = reference_;
	} else {
		result.reference_ = this;
	}
	memcpy(result.bounds_, restrictedBounds_, 7 * sizeof(int));
	memcpy(result.restrictedBounds_, restrictedBounds_, 7 * sizeof(int));
	return result;
}



Variable Variable::toReference(int indeces[4])
{
	Variable result;
	if (reference_) {
		result.reference_ = reference_;
	} else {
		result.reference_ = this;
	}
	memcpy(result.referenceIndeces_, indeces, 4 * sizeof(int));
	return result;
}

String Variable::fullReferenceName() const
{
	if (reference_) {
		String result = name();
		if (referenceIndeces_[3]) {
			result.push_back('[');
			for (int i = 0; i < referenceIndeces_[3]; i++) {
				result += Kumir::Converter::sprintfInt(referenceIndeces_[i], 10, 0, 0);
				if (i < referenceIndeces_[3] - 1) {
					result.push_back(',');
				}
			}
			result.push_back(']');
		}
		return result;
	} else {
		return name();
	}
}


bool Variable::ignoreUndefinedError = false;

}
