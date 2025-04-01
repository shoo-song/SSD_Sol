#include "iostream"
#include "fstream"

class FILEIO {
	virtual bool open(const std::string& filename) = 0;
};
class SSDInterface : public FILEIO {
public:
	 bool open(const std::string& filename) override {
		file_.open(filename, std::ios::out | std::ios::binary);
		return file_.is_open();
	}
	bool isOpen() const {
		return file_.is_open();
	}
	void write(const std::string& data) {
		if (file_.is_open()) {
			file_ << data;
		}
	}
	void close() {
		if (file_.is_open()) {
			file_.close();
		}
	}



protected:
	std::ofstream file_;
};