#include "iostream"
#include "fstream"
#include <string>
#include <iomanip>
#include "stdexcept"

using namespace std;

class FILEIO {
	virtual bool open(const std::string& filename) = 0;
	virtual void write(const int& LBA, const unsigned int& data) = 0;
};
class SSDInterface : public FILEIO {
public:
	 bool open(const std::string& filename) override {
		if (file_.is_open()) {
			file_.close();
		}
		file_.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		return file_.is_open();
	}
	bool isOpen() const {
		return file_.is_open();
	}
	void write(const int& LBA, const unsigned int& data) {
		if (file_.is_open()) {
			file_.seekp(0, std::ios::end);  // 파일 끝으로 이동 후 기록
			file_ << std::setw(3) << LBA << std::setw(10) << data << "\n";  // LBA(3자리), 데이터(10자리) 고정 길이 저장
			file_.flush();
		}			
	}

	unsigned int read(const int& LBA) {
		std::string data;

		int readLBA;
		unsigned int readData;
		file_.seekg(0, std::ios::beg);
		while (file_ >> readLBA >> readData) {
			if (readLBA == LBA) {
				return readData;
			}
		}

		return 0;
	}
	void close() {
		if (file_.is_open()) {
			file_.close();
		}
	}

protected:
	std::fstream file_;
};