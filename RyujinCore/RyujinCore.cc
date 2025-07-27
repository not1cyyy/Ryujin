#include "RyujinCore.hh"

/*
    Disable all optimizations before compile for release - MSVC sucks - Build ryujincore in debug or use contexpr mainly on fix relocs
*/

RYUJINCORE_API BOOL __stdcall RunRyujinCore(const char* strInputFilePath, const char* strPdbFilePath, const char* strOutputFilePath, RyujinObfuscatorConfig& config) {

    if (!strInputFilePath || !strPdbFilePath || !strOutputFilePath) return FALSE;

    std::vector<std::string> strProcsProcessed;

    strProcsProcessed.reserve(config.m_strProceduresToObfuscate.procedureCount);

    for (int i = 0; i < config.m_strProceduresToObfuscate.procedureCount; ++i)
        strProcsProcessed.emplace_back(config.m_strProceduresToObfuscate.procedures[i]);

    std::shared_ptr<RyuJinConfigInternal> ryujConfigInternal = std::make_shared<RyuJinConfigInternal>();
    ryujConfigInternal->m_strdProceduresToObfuscate.assign(strProcsProcessed.begin(), strProcsProcessed.end());

    std::unique_ptr<Ryujin> ryujin = std::make_unique<Ryujin>(strInputFilePath, strPdbFilePath, strOutputFilePath);

    ryujin.get()->listRyujinProcedures();

    ryujin.get()->run(config, ryujConfigInternal);

    ryujin.reset();

    return TRUE;
}
