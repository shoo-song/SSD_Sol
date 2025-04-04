#include "script_parameter_generator.h"
#include "shell_exception.h"

ShellScriptRandValStore& ShellScriptRandValStore::getShellScriptRandValStore() {
	static ShellScriptRandValStore shellScriptRandValStore;
	return shellScriptRandValStore;
}
void ShellScriptRandValStore::addRandValue(int randIdx, int val) {
	mRandValStore[randIdx] = val;
}
int ShellScriptRandValStore::getRandVal(int randIdx) {
	/// 있는지 체크 필요.
	if (mRandValStore.find(randIdx) == mRandValStore.end())
		throw std::runtime_error("rand val error");
	return mRandValStore[randIdx];
}
void ShellScriptRandValStore::clear() {
	mRandValStore.clear();
}
ShellScriptRandValStore::ShellScriptRandValStore() {
}


ShellScriptRandGetGen::ShellScriptRandGetGen(int randStoreIdx) {
	mRandStoreIdx = randStoreIdx;
}
int ShellScriptRandGetGen::generateParameter() {
	return ShellScriptRandValStore::getShellScriptRandValStore().getRandVal(mRandStoreIdx);
}

ScriptParamIdxGen::ScriptParamIdxGen(shared_ptr<ShellScriptLoopIdxGetter> looper, int rel) {
	mpLooper = looper;
	mRel = rel;
}
int ScriptParamIdxGen::generateParameter() {
	return mpLooper->getCurrentLoopIdx() + mRel;
}

ScriptParamValGen::ScriptParamValGen(int val) {
	mVal = val;
}
int ScriptParamValGen::generateParameter() {
	return mVal;
}

ScriptParamRanGen::ScriptParamRanGen(int storeIdx, int startRand, int endRand) {
	mStart = startRand;
	mEnd = endRand;
	mStoreIdx = storeIdx;
}
int ScriptParamRanGen::generateParameter() {
	int randVal = mStart + std::rand() % (mEnd - mStart + 1);
	ShellScriptRandValStore::getShellScriptRandValStore().addRandValue(mStoreIdx, randVal);
	return randVal;
}