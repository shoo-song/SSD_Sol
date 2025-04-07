#pragma once

#include "ssddriver_interface.h"
#include <memory>

using std::shared_ptr;

class SsdDriverStore
{
public:
	static SsdDriverStore& getSsdDriverStore() {
		static SsdDriverStore ssdStore;
		return ssdStore;
	}
	void setSsdDriver(shared_ptr<SsdDriverInterface> driver) {
		mpSsdDriver = driver;
	}
	shared_ptr<SsdDriverInterface> getSsdDriver() {
		return mpSsdDriver;
	}
private:
	SsdDriverStore() {

	}
	shared_ptr< SsdDriverInterface> mpSsdDriver;
};

