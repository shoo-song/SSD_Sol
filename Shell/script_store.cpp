#include "script_store.h"

ScriptStore& ScriptStore::getScriptStore() {
    static ScriptStore scriptStore;
    return scriptStore;
}
bool ScriptStore::isThereScript(string scriptName) {
    if (mCommandMap.find(scriptName) != mCommandMap.end()) {
        return true;
    }
    int wildcardNum = getWildcardNumber(scriptName);
    if (mWildCardCommandMap.find(wildcardNum) != mWildCardCommandMap.end()) {
        return true;
    }
    return false;
}

vector<string>& ScriptStore::getScriptNames() { return mFileNames; }

vector<shared_ptr<ShellScriptCommandInterface>> ScriptStore::getScriptCommand(string scriptName) {
    if (mCommandMap.find(scriptName) != mCommandMap.end()) {
        return mCommandMap[scriptName];
    }
    int wildcardNum = getWildcardNumber(scriptName);
    if (mWildCardCommandMap.find(wildcardNum) != mWildCardCommandMap.end()) {
        return mWildCardCommandMap[wildcardNum];
    }
    throw ShellArgConvertException("err");
}

void ScriptStore::addScript(string scriptName,
                            vector<shared_ptr<ShellScriptCommandInterface>> command) {
    mWildCardCommandMap[getWildcardNumber(scriptName)] = command;
    mCommandMap[scriptName] = command;
    mFileNames.push_back(scriptName);
}

ScriptStore::ScriptStore() {}

int ScriptStore::getWildcardNumber(string fileName) {
    size_t pos = fileName.find('_');
    if (pos != std::string::npos) {
        std::string numberStr = fileName.substr(0, pos);
        try {
            // 숫자 문자열을 int로 변환합니다.
            int number = std::stoi(numberStr);
            return number;
        } catch (const std::exception& e) {
            throw e;
        }
    } else {
        throw ShellArgConvertException("err");
    }
}