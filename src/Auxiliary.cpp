#include <string>

bool isValidIPv4(const std::string ip) {
	int num = 0;
	int dots = 0;

	for (int i = 0; i < ip.length(); i++) {
		if (ip[i] == '.') {
			dots++;
			if (num > 255) {
				return false;
			}
			num = 0;
		} else if (ip[i] >= '0' && ip[i] <= '9') {
			num = num * 10 + (ip[i] - '0');
		} else {
			return false;
		}
	}

	if (dots != 3) {
		return false;
	}

	if (num > 255) {
		return false;
	}

	return true;
}