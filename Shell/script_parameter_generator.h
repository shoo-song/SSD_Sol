#pragma once
#include <memory>
#include <unordered_map>
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
    static ShellScriptRandValStore& getShellScriptRandValStore();
    void addRandValue(int randIdx, int val);
    int getRandVal(int randIdx);
    void clear();

   private:
    ShellScriptRandValStore();
    unordered_map<int, int> mRandValStore;
};

class ShellScriptRandGetGen : public ShellScriptParameterGenInterface {
   public:
    ShellScriptRandGetGen(int randStoreIdx);
    int generateParameter();

   private:
    int mRandStoreIdx;
};

class ScriptParamIdxGen : public ShellScriptParameterGenInterface {
   public:
    ScriptParamIdxGen(shared_ptr<ShellScriptLoopIdxGetter> looper, int rel);
    int generateParameter();

   private:
    shared_ptr<ShellScriptLoopIdxGetter> mpLooper;
    int mRel;
};

class ScriptParamValGen : public ShellScriptParameterGenInterface {
   public:
    ScriptParamValGen(int val);
    int generateParameter();

   private:
    int mVal;
};

class ScriptParamRanGen : public ShellScriptParameterGenInterface {
   public:
    ScriptParamRanGen(int storeIdx, int startRand, int endRand);
    int generateParameter();

   private:
    int mStart;
    int mEnd;
    int mStoreIdx;
};