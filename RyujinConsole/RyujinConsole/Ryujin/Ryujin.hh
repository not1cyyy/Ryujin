#pragma once
#include <Zydis/Zydis.h>
#include <Zydis/SharedTypes.h>
#include <iostream>
#include <memory>
#include <vector>
#include "PDB/RyujinPdbParsing.hh"
#include "Utils/RyujinUtils.hh"
#include "Models/RyujinObfuscatorConfig.hh"
#include "RyujinCore/BasicBlockerBuilder.hh"

class Ryujin {

private:
	std::shared_ptr<unsigned char> m_mappedPE;
	const std::string& m_strInputFilePath;
	const std::string& m_strPdbFilePath;
	const std::string& m_strOutputFilePath;
	uintptr_t m_szFile;
	BOOL m_isInitialized;
	std::vector<RyujinProcedure> m_ryujinProcedures;

	bool todoAction() { return FALSE;  }

public:
	Ryujin(const std::string& strInputFilePath, const std::string& strPdbFilePath, const std::string& strOutputFilePath);
	bool run(const RyujinObfuscatorConfig& config);
	void listRyujinProcedures();
	~Ryujin();

};

