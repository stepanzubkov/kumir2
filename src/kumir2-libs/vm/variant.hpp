#ifndef VARIANT_HPP
#define VARIANT_HPP

#ifdef DO_NOT_DECLARE_STATIC
#define DO_NOT_DECLARE_STATIC_VARIANT
#endif

#define DO_NOT_DECLARE_STATIC
#include <kumir2-libs/stdlib/kumirstdlib.hpp>
#include "vm_enums.h"

namespace VM
{

using Kumir::Char;
using Kumir::String;
using Kumir::real;
using namespace Bytecode;

class AnyValue;
struct Record {
	std::vector<AnyValue> fields;
};

class AnyValue
{
	ValueType type_;
	String *svalue_;
	std::vector<AnyValue> *avalue_;
	Record *uvalue_;
	union {
		int ivalue_;
		real rvalue_;
		Char cvalue_;
		bool bvalue_;
	};

	void __init__()
	{
		if (avalue_) {
			avalue_->clear();
			delete avalue_;
			avalue_ = 0;
		}

		if (svalue_) {
			delete svalue_;
			svalue_ = 0;
		}

		if (uvalue_) {
			delete uvalue_;
			uvalue_ = 0;
		}

		type_ = VT_void;
		ivalue_ = 0;
	}

public:
	explicit AnyValue(): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
	}

	AnyValue(const AnyValue &other): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = other.type_;
		if (other.svalue_) {
			svalue_ = new String(*(other.svalue_));
		}
		if (other.uvalue_) {
			uvalue_ = new Record(*(other.uvalue_));
		}
		if (other.avalue_) {
			avalue_ = new std::vector<AnyValue>(*(other.avalue_));
		}
		if (type_ == VT_int) {
			ivalue_ = other.ivalue_;
		}
		if (type_ == VT_real) {
			rvalue_ = other.rvalue_;
		}
		if (type_ == VT_bool) {
			bvalue_ = other.bvalue_;
		}
		if (type_ == VT_char) {
			cvalue_ = other.cvalue_;
		}
	}

	explicit AnyValue(ValueType t): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = t;
		svalue_ = t == VT_string ? new String() : 0;
		ivalue_ = 0;
	}

	explicit AnyValue(int v): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_int;
		ivalue_ = v;
	}

	explicit AnyValue(real v): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_real;
		rvalue_ = v;
	}

	explicit AnyValue(bool v): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_bool;
		bvalue_ = v;
	}

	explicit AnyValue(Char v): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_char;
		cvalue_ = v;
	}

	explicit AnyValue(const String &v): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_string;
		svalue_ = new String(v);
	}

	explicit AnyValue(const Record &value): svalue_(0), avalue_(0), uvalue_(0)
	{
		__init__();
		type_ = VT_record;
		uvalue_ = new Record(value);
	}

	void operator=(ValueType t)
	{
		__init__();
		type_ = t;
		svalue_ = t == VT_string ? new String() : 0;
	}

	void operator=(int v)
	{
		__init__();
		type_ = VT_int;
		ivalue_ = v;
	}

	void operator=(real v)
	{
		__init__();
		type_ = VT_real;
		rvalue_ = v;
	}

	void operator=(bool v)
	{
		__init__();
		type_ = VT_bool;
		bvalue_ = v;
	}

	void operator=(Char v)
	{
		__init__();
		type_ = VT_char;
		cvalue_ = v;
	}

	void operator=(const String &v)
	{
		__init__();
		type_ = VT_string;
		svalue_ = new String(v);
	}

	void operator=(const Record &value)
	{
		__init__();
		type_ = VT_record;
		uvalue_ = new Record(value);
	}

	void operator=(const AnyValue &other)
	{
		__init__();
		type_ = other.type_;
		if (other.svalue_) {
			svalue_ = new String(*(other.svalue_));
		}
		if (other.uvalue_) {
			uvalue_ = new Record(*(other.uvalue_));
		}
		if (other.avalue_) {
			avalue_ = new std::vector<AnyValue>(*(other.avalue_));
		}
		if (type_ == VT_int) {
			ivalue_ = other.ivalue_;
		}
		if (type_ == VT_real) {
			rvalue_ = other.rvalue_;
		}
		if (type_ == VT_bool) {
			bvalue_ = other.bvalue_;
		}
		if (type_ == VT_char) {
			cvalue_ = other.cvalue_;
		}
	}

	int toInt() const
	{
		if (type_ == VT_bool) {
			return bvalue_ ? 1 : 0;
		} else if (type_ == VT_char) {
			return static_cast<int>(cvalue_);
		} else {
			return ivalue_;
		}
	}

	real toReal() const
	{
		if (type_ == VT_bool || type_ == VT_int) {
			return static_cast<real>(toInt());
		} else {
			return rvalue_;
		}
	}

	bool toBool() const
	{
		if (type_ == VT_int) {
			return ivalue_ > 0;
		} else if (type_ == VT_real) {
			return rvalue_ > 0.0;
		} else if (type_ == VT_char) {
			return cvalue_ != '\0';
		} else if (type_ == VT_string) {
			return svalue_ && svalue_->length() > 0;
		} else {
			return bvalue_;
		}
	}

	Char toChar() const
	{
		if (type_ == VT_int) {
			return static_cast<Char>(ivalue_);
		} else if (type_ == VT_string && svalue_ && svalue_->length() == 1) {
			return svalue_->at(0);
		} else {
			return cvalue_;
		}
	}

	String toString() const
	{
		if (type_ == VT_int) {
			return Kumir::Converter::sprintfInt(ivalue_, 10, 0, 0);
		} else if (type_ == VT_real) {
			return Kumir::Converter::sprintfReal(rvalue_, '.', false, 0, -1, 0);
		} else if (type_ == VT_bool) {
			return bvalue_ ? Kumir::Core::fromUtf8("да") : Kumir::Core::fromUtf8("нет");
		} else if (type_ == VT_char) {
			String sval;
			sval.push_back(cvalue_);
			return sval;
		} else if (type_ == VT_void) {
			return String();
		} else if (svalue_) {
			return String(*svalue_);
		} else {
			return String();
		}
	}

	const Record &toRecord() const
	{
		return *uvalue_;
	}

	Record &toRecord()
	{
		return *uvalue_;
	}

	bool isValid() const
	{
		return type_ != VT_void || (avalue_ && avalue_->size() > 0);
	}

	ValueType type() const
	{
		return type_;
	}

	const AnyValue &at(size_t index) const
	{
		return avalue_->at(index);
	}

	const AnyValue &operator[](size_t index) const
	{
		return at(index);
	}

	AnyValue &at(size_t index)
	{
		return avalue_->at(index);
	}

	AnyValue &operator[](size_t index)
	{
		return at(index);
	}

	size_t rawSize() const
	{
		return avalue_ ? avalue_->size() : 0;
	}

	~AnyValue()
	{
		if (svalue_) {
			delete svalue_;
		}
		if (avalue_) {
			avalue_->clear();
			delete avalue_;
		}
		if (uvalue_) {
			delete uvalue_;
		}
	}


	void resize(size_t size)
	{
		if (!avalue_) {
			avalue_ = new std::vector<AnyValue>(size);
		}
		if (size == 0) {
			if (avalue_->size()) {
				avalue_->clear();
			}
		} else {
			if (size != avalue_->size()) {
				avalue_->resize(size);
			}
		}
	}

};



class Variable
{
	AnyValue value_;
	uint8_t dimension_;
	int bounds_[7];
	int restrictedBounds_[7];
	ValueType baseType_;
	Variable *reference_;
	int referenceIndeces_[4];
	String name_;
	String algorhitmName_;
	String moduleName_;
	std::string recordModuleAsciiName_;
	String recordModuleLocalizedName_;
	std::string recordClassAsciiName_;
	String recordClassLocalizedName_;
	bool constant_;
	int referenceStackContextNo_;

	void create()
	{
		unsigned int i;
		referenceStackContextNo_ = -2;
		for (i = 0; i < 4; i++) {
			referenceIndeces_[i] = 0;
		}
		for (i = 0; i < 7; i++) {
			bounds_[i] = 0;
			restrictedBounds_[i] = 0;
		}
		value_ = VT_void;
		dimension_ = 0;
		baseType_ = VT_void;
		reference_ = 0;
		constant_ = false;
	}

public:
	static bool ignoreUndefinedError;

	explicit Variable()
	{
		create();
	}

	explicit Variable(int v)
	{
		create() ;
		baseType_ = VT_int;
		value_ = v;
	}

	explicit Variable(double v)
	{
		create();
		baseType_ = VT_real;
		value_ = v;
	}

	explicit Variable(Char v)
	{
		create();
		baseType_ = VT_char;
		value_ = v;
	}

	explicit Variable(const String &v)
	{
		create();
		baseType_ = VT_string;
		value_ = v;
	}

	explicit Variable(bool v)
	{
		create();
		baseType_ = VT_bool;
		value_ = v;
	}

	explicit Variable(
		const Record &value,
		const String &clazz,
		const std::string &asciiClazz
	) {
		create();
		baseType_ = VT_record;
		value_ = value;
		setRecordClassLocalizedName(clazz);
		setRecordClassAsciiName(asciiClazz);
	}

	explicit Variable(Variable *ref)
	{
		create();
		reference_ = ref;
	}

	explicit Variable(const AnyValue &v)
	{
		create();
		baseType_ = v.type();
		value_ = v;
	}

	bool isValid() const
	{
		return reference_
			? reference_->isValid()
			: value_.type() != VT_void || dimension_ > 0;
	}

	bool isConstant() const
	{
		return reference_ ? reference_->isConstant() : constant_;
	}

	void setConstantFlag(bool value)
	{
		constant_ = value;
	}

	void init();

	uint8_t dimension() const
	{
		return reference_ ? reference_->dimension() : dimension_;
	}

	void setDimension(uint8_t v)
	{
		dimension_ = v;
	}

	void setName(const String &n)
	{
		name_ = n;
	}

	String fullReferenceName() const;

	const String &name() const
	{
		if (reference_) {
			return reference_->name();
		} else {
			return name_;
		}
	}

	const String &myName() const
	{
		return name_;
	}

	const String &recordModuleLocalizedName() const
	{
		if (reference_) {
			return reference_->recordModuleLocalizedName();
		} else {
			return recordModuleLocalizedName_;
		}
	}

	const std::string &recordModuleAsciiName() const
	{
		if (reference_) {
			return reference_->recordModuleAsciiName();
		} else {
			return recordModuleAsciiName_;
		}
	}

	const std::string &recordClassAsciiName() const
	{
		if (reference_) {
			return reference_->recordClassAsciiName();
		} else {
			return recordClassAsciiName_;
		}
	}

	const String &recordClassLocalizedName() const
	{
		if (reference_) {
			return reference_->recordClassLocalizedName();
		} else {
			return recordClassLocalizedName_;
		}
	}

	void setRecordModuleLocalizedName(const String &n)
	{
		if (reference_) {
			reference_->setRecordModuleLocalizedName(n);
		} else {
			recordModuleLocalizedName_ = n;
		}
	}

	void setRecordModuleAsciiName(const std::string &n)
	{
		if (reference_) {
			reference_->setRecordModuleAsciiName(n);
		} else {
			recordModuleAsciiName_ = n;
		}
	}

	void setRecordClassAsciiName(const std::string &n)
	{
		if (reference_) {
			reference_->setRecordClassAsciiName(n);
		} else {
			recordClassAsciiName_ = n;
		}
	}

	void setRecordClassLocalizedName(const String &n)
	{
		if (reference_) {
			reference_->setRecordClassLocalizedName(n);
		} else {
			recordClassLocalizedName_ = n;
		}
	}

	void setAlgorhitmName(const String &n)
	{
		algorhitmName_ = n;
	}

	void setModuleName(const String &n)
	{
		moduleName_ = n;
	}

	const String &moduleName() const
	{
		return moduleName_;
	}

	const String &algorhitmName() const
	{
		if (reference_) {
			return reference_->algorhitmName();
		} else {
			return algorhitmName_;
		}
	}

	void setBounds(int bounds[7]);
	void updateBounds(int bounds[7]);
	void getBounds(/*out*/ int bounds[7]) const
	{
		memcpy(bounds, bounds_, 7 * sizeof(int));
	}

	void getEffectiveBounds(/*out*/ int bounds[7]) const
	{
		memcpy(bounds, restrictedBounds_, 7 * sizeof(int));
	}

	bool hasValue() const;
	bool hasValue(int indeces[4]) const;
	bool hasValue(int index0) const;
	bool hasValue(int index0, int index1) const;
	bool hasValue(int index0, int index1, int index2) const;

	AnyValue value() const;
	AnyValue value(int index0) const;
	AnyValue value(int index0, int index1) const;
	AnyValue value(int index0, int index1, int index2) const;

	AnyValue value(int indeces[4]) const;

	size_t rawSize() const
	{
		return value_.rawSize();
	}

	const AnyValue &at(size_t index) const
	{
		return value_.at(index);
	}

	const AnyValue &operator[](size_t index) const
	{
		return at(index);
	}

	AnyValue &at(size_t index)
	{
		return value_.at(index);
	}

	AnyValue &operator[](size_t index)
	{
		return at(index);
	}

	bool isReference() const
	{
		return reference_ != 0;
	}

	void setReference(Variable *r, int effectiveBounds[7])
	{
		reference_ = r;
		memcpy(bounds_, effectiveBounds, 7 * sizeof(int));
		memcpy(restrictedBounds_, effectiveBounds, 7 * sizeof(int));
	}

	Variable *reference()
	{
		return reference_;
	}

	void setReferenceIndeces(int v[4])
	{
		memcpy(referenceIndeces_, v, 4 * sizeof(int));
	}

	void getReferenceIndeces(int result[4]) const
	{
		memcpy(result, referenceIndeces_, 4 * sizeof(int));
	}

	int toInt() const
	{
		return value().toInt();
	}

	real toReal() const
	{
		return value().toReal();
	}

	double toDouble() const
	{
		return toReal();
	}

	bool toBool() const
	{
		return value().toBool();
	}

	Char toChar() const
	{
		return value().toChar();
	}

	String toString() const;
	String toString(int indeces[4]) const;

	Record toRecord() const
	{
		if (reference_) {
			const Record result = reference_->toRecord();
			return result;
		} else {
			return value_.toRecord();
		}
	}

	Record &toRecord()
	{
		if (reference_) {
			Record &result = reference_->toRecord();
			return result;
		} else {
			Record &result = value_.toRecord();
			return result;
		}
	}

	Variable toReference();
	static Variable toConstReference(const AnyValue &value);

	int referenceStackContextNo() const
	{
		return referenceStackContextNo_;
	}

	void setReferenceStackContextNo(int v)
	{
		referenceStackContextNo_ = v;
	}

	Variable toReference(int indeces[4]);

	void setValue(const AnyValue &value);
	void setValue(int index0, const AnyValue &value);
	void setValue(int index0, int index1, const AnyValue &value);
	void setValue(int index0, int index1, int index2, const AnyValue &value);
	void setValue(int indeces[4], const AnyValue &value);

	void setConstValue(const Variable &ctab);

	ValueType baseType() const
	{
		return reference_ ? reference_->baseType() : baseType_;
	}

	void setBaseType(ValueType vt)
	{
		baseType_ = vt;
	}

	static void unsetError()
	{
		Kumir::Core::unsetError();
	}

private:
	size_t linearIndex(int a) const;
	size_t linearIndex(int a, int b) const;
	size_t linearIndex(int a, int b, int c) const;

};

}

#endif
