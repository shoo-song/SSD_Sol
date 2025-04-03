#pragma once

#include <memory>
using std::shared_ptr;

class ShellScriptParameterGenInterface {
public:
	virtual int generateParameter() = 0;
};

class ShellScriptLoopIdxGetter {
public:
	virtual int getCurrentLoopIdx() = 0;
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
	ScriptParamRanGen(int startRand, int endRand) {
		mStart = startRand;
		mEnd = endRand;
	}
	int generateParameter() {
		return mStart + std::rand() % (mEnd - mStart + 1);
	}
private:
	int mStart;
	int mEnd;
};