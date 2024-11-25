#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

class MyRecord {
  public:
	uint8_t x, y;
	float z;

	MyRecord() = default;
	MyRecord(uint8_t x, uint8_t y, float z) : x(x), y(y), z(z) {}

	template <class Archive>
	void serialize(Archive &ar) {
		ar(x, y, z);
	}
};

class SomeData {
  public:
	std::vector<std::unique_ptr<MyRecord>> data;
	std::string name;
	double value;
	std::vector<int> largeVector; // This member will not be serialized

	SomeData() = default;
	SomeData(std::vector<std::unique_ptr<MyRecord>> data, std::string name, double value, std::vector<int> largeVector)
	    : data(std::move(data)), name(name), value(value), largeVector(largeVector) {}

	template <class Archive>
	void save(Archive &ar) const {
		ar(data, name, value);
		// largeVector is not serialized
	}

	template <class Archive>
	void load(Archive &ar) {
		ar(data, name, value);
		// largeVector is not deserialized
	}
};

int main() {
	// Create an output string stream
	std::ostringstream os;
	{
		cereal::JSONOutputArchive archive(os);
		std::vector<std::unique_ptr<MyRecord>> records;
		records.push_back(std::make_unique<MyRecord>(1, 2, 3.0f));
		records.push_back(std::make_unique<MyRecord>(4, 5, 6.0f));
		SomeData myData(std::move(records), "ExampleName", 42.42, {1, 2, 3, 4, 5});
		archive(myData);
	}

	// Get the serialized string
	std::string serializedData = os.str();
	std::cout << "Serialized Data: " << serializedData << std::endl;

	// Create an existing SomeData object with a different largeVector
	std::vector<std::unique_ptr<MyRecord>> existingRecords;
	existingRecords.push_back(std::make_unique<MyRecord>(7, 8, 9.0f));
	SomeData existingData(std::move(existingRecords), "OldName", 24.24, {10, 20, 30});

	// Deserialize from the string into the existing object
	try {
		std::istringstream is(serializedData);
		cereal::JSONInputArchive archive(is);
		archive(existingData);
	} catch (const cereal::Exception &e) {
		std::cerr << "Deserialization error: " << e.what() << std::endl;
		return 1;
	}

	// Print the values of the loaded data
	std::cout << "Name: " << existingData.name << std::endl;
	std::cout << "Value: " << existingData.value << std::endl;
	for (const auto &record : existingData.data) {
		std::cout << "x: " << static_cast<int>(record->x) << ", y: " << static_cast<int>(record->y)
		          << ", z: " << record->z << std::endl;
	}

	// Print the largeVector to show it retains its previous value
	std::cout << "largeVector: ";
	for (const auto &val : existingData.largeVector) {
		std::cout << val << " ";
	}
	std::cout << std::endl;

	return 0;
}
