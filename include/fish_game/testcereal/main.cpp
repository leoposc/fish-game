#include "myclasses.hpp"

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <iostream>

int main() {
	std::vector<std::unique_ptr<test::BaseClass>> components;

	auto *c = new test::DerivedClassOne();
	std::unique_ptr<test::BaseClass> uPtr{c};
	components.emplace_back(std::move(uPtr));

	auto *c2 = new test::EmbarrassingDerivedClass();
	std::unique_ptr<test::BaseClass> uPtr2{c2};
	components.emplace_back(std::move(uPtr2));

	{
		std::ofstream os("polymorphism_test.xml");
		cereal::XMLOutputArchive oarchive(os);
		oarchive(components);
		// oarchive(cereal::make_nvp("components", components));
		// // Create instances of the derived classes, but only keep base class pointers
		// std::shared_ptr<test::BaseClass> ptr1 = std::make_shared<test::DerivedClassOne>();
		// std::shared_ptr<test::BaseClass> ptr2 = std::make_shared<test::EmbarrassingDerivedClass>();
		// oarchive(ptr1, ptr2);
	}

	{
		std::ifstream is("polymorphism_test.xml");
		cereal::XMLInputArchive iarchive(is);

		iarchive(components);

		// De-serialize the data as base class pointers, and watch as they are
		// re-instantiated as derived classes
		// std::shared_ptr<test::BaseClass> ptr1;
		// std::shared_ptr<test::BaseClass> ptr2;
		// iarchive(ptr1, ptr2);

		// // Ta-da! This should output:
		for (auto &c : components) {
			c->sayType();
		}
		// Ta-da! This should output:
		// ptr1->sayType(); // "DerivedClassOne"
		// ptr2->sayType(); // "EmbarrassingDerivedClass. Wait.. I mean DerivedClassTwo!"
	}

	return 0;
}