#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
/*
0 ~4번 LBA까지 Write 명령어를 수행한다.
0 ~4번 LBA까지 ReadCompare 수행
6 ~9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
6 ~9번 LBA까지 ReadCompare 수행
10 ~14번 LBA까지 다른 값으로Write 명령어를 수행한다.
10 ~14번 LBA까지 ReadCompare 수행

위와같은규칙으로전체영역에대해Full Write + ReadCompare를 수행한다
*/

class ShellScript2 : public ShellCommandInterface {
public:
	ShellScript2(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "PASS";

			for (int i = 0; i < 30; i++) {
				mpDriverInterface->writeSSD((int)0, i);
				mpDriverInterface->writeSSD((int)3, i);
				mpDriverInterface->writeSSD((int)1, i);
				mpDriverInterface->writeSSD((int)2, i);

				if (mpDriverInterface->readSSD((int)0) != i) {
					return "FAIL";
				}
				if (mpDriverInterface->readSSD((int)3) != i) {
					return "FAIL";
				}
				if (mpDriverInterface->readSSD((int)1) != i) {
					return "FAIL";
				}
				if (mpDriverInterface->readSSD((int)2) != i) {
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