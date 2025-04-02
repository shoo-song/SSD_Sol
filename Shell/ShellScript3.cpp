#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
#include <cstdlib> 

using std::rand;

class ShellScript3 : public ShellCommandInterface {
public:
	ShellScript3(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "PASS";

			for (int i = 0; i < 200; i++) {
				unsigned int rand1 = (unsigned int)rand() % 0xFFFFFFFF;
				unsigned int rand2 = (unsigned int)rand() % 0xFFFFFFFF;

				mpDriverInterface->writeSSD((int)0, rand1);
				mpDriverInterface->writeSSD((int)99, rand2);

				if (mpDriverInterface->readSSD((int)0) != rand1) {
					return "FAIL";
				}
				if (mpDriverInterface->readSSD((int)99) != rand2) {
					return "FAIL";
				}
			}

			return output;
		}
		catch (ShellArgConvertException e) {
			return "FAIL";
		}
		catch (exception e) {
			return "FAIL";
		}
	}
private:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;
			if (args.size() != 1) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			return output;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

	SsdDriverInterface* mpDriverInterface;
};