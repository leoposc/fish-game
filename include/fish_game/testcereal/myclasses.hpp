// A pure virtual base class
namespace test {

struct BaseClass {
	virtual void sayType() = 0;
};

// A class derived from BaseClass
struct DerivedClassOne : public BaseClass {
	void sayType();

	int x;

	template <class Archive>
	void serialize(Archive &ar) {}
};

// Another class derived from BaseClass
struct EmbarrassingDerivedClass : public BaseClass {
	void sayType();

	float y;

	template <class Archive>
	void serialize(Archive &ar) {
		ar(y);
	}
};

// Potentially necessary if no explicit reference
// to objects in myclasses.cpp will take place
// from other translation units
//#include <cereal/types/polymorphic.hpp>
// CEREAL_FORCE_DYNAMIC_INIT(myclasses)

} // namespace test