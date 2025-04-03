#pragma once
#include <unordered_map>
#include <memory>
using std::shared_ptr;
using std::unordered_map;

class ShellScriptParameterGenInterface {
public:
	virtual int generateParameter() = 0;
};

class ShellScriptLoopIdxGetter {
public:
	virtual int getCurrentLoopIdx() = 0;
};


class ShellScriptRandValStore {
public:
	static ShellScriptRandValStore& getShellScriptRandValStore() {
		static ShellScriptRandValStore shellScriptRandValStore;
		return shellScriptRandValStore;
	}
	void addRandValue(int randIdx, int val) {
		mRandValStore[randIdx] = val;
	}
	int getRandVal(int randIdx) {
		/// 있는지 체크 필요.
		if (mRandValStore.find(randIdx) != mRandValStore.end())
			throw std::runtime_error("rand val error");
		return mRandValStore[randIdx];
	}
	void clear() {
		mRandValStore.clear();
	}
private:
	ShellScriptRandValStore() {

	}
	unordered_map<int, int> mRandValStore;
};

class ShellScriptRandGetGen : public ShellScriptParameterGenInterface {
public:
	ShellScriptRandGetGen(int randStoreIdx) {
		mRandStoreIdx = randStoreIdx;
	}
	int generateParameter() {
		return ShellScriptRandValStore::getShellScriptRandValStore().getRandVal(mRandStoreIdx);
	}
private:
	int mRandStoreIdx;
};


class ScriptParamIdxGen : public ShellScriptParameterGenInterface {
public:
	ScriptParamIdxGen(shared_ptr<ShellScriptLoopIdxGetter> looper, int rel) {
		mpLooper = looper;
		mRel = rel;
	}
	int generateParameter() {
		return mpLooper->getCurrentLoopIdx() + mRel;
	}
private:
	shared_ptr<ShellScriptLoopIdxGetter> mpLooper;
	int mRel;
};

class ScriptParamValGen : public ShellScriptParameterGenInterface {
public:
	ScriptParamValGen(int val) {
		mVal = val;
	}
	int generateParameter() {
		return mVal;
	}
private:
	int mVal;
};

class ScriptParamRanGen : public ShellScriptParameterGenInterface {
public:
	ScriptParamRanGen(int storeIdx, int startRand, int endRand) {
		mStart = startRand;
		mEnd = endRand;
		mStoreIdx = storeIdx;
	}
	int generateParameter() {
		int randVal = mStart + std::rand() % (mEnd - mStart + 1);
		ShellScriptRandValStore::getShellScriptRandValStore().addRandValue(mStoreIdx, randVal);
		return randVal;
	}
private:
	int mStart;
	int mEnd;
	int mStoreIdx;
};