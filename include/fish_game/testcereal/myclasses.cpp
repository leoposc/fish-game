#include "myclasses.hpp"
#include <iostream>

// Include any archives you plan on using with your type before you register it.
// This could occur in multiple source files for the same type, if desired.
// Any archives included from other header files included here will also be
// registered.
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

// Include the polymorphic serialization and registration mechanisms
#include <cereal/types/polymorphic.hpp>

namespace test {

void DerivedClassOne::sayType() {
	std::cout << "DerivedClassOne" << std::endl;
}

void EmbarrassingDerivedClass::sayType() {
	std::cout << "EmbarrassingDerivedClass. Wait.. I mean DerivedClassTwo!" << std::endl;
}

} // namespace test

// Register DerivedClassOne
CEREAL_REGISTER_TYPE(test::DerivedClassOne);

// Register EmbarassingDerivedClass with a less embarrasing name
CEREAL_REGISTER_TYPE_WITH_NAME(test::EmbarrassingDerivedClass, "DerivedClassTwo");

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(test::BaseClass, test::DerivedClassOne)
CEREAL_REGISTER_POLYMORPHIC_RELATION(test::BaseClass, test::EmbarrassingDerivedClass)

// Potentially necessary if no explicit reference
// to objects in myclasses.cpp will take place
// from other translation units
// CEREAL_REGISTER_DYNAMIC_INIT(myclasses)