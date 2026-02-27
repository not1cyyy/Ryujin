#pragma once
#include <Zydis/Zydis.h>
#include <Zydis/SharedTypes.h>
#include <iostream>
#include <memory>
#include <vector>
#include "PDB/RyujinPdbParsing.h"
#include "Utils/RyujinUtils.h"
#include "Models/RyujinObfuscatorConfig.h"
#include "RyujinCore/BasicBlockerBuilder.h"
#include "RyujinCore/RyujinObfuscationCore.h"
#include "Utils/RyujinPESections.h"

class Ryujin {

private:
	std::shared_ptr<unsigned char> m_mappedPE;
	std::string m_strInputFilePath;
	std::string m_strPdbFilePath;
	std::string m_strOutputFilePath;
	uintptr_t m_szFile;
	BOOL m_isInitialized;
	std::vector<RyujinProcedure> m_ryujinProcedures;

	bool todoAction() { return FALSE;  }

public:
	Ryujin(const std::string& strInputFilePath, const std::string& strPdbFilePath, const std::string& strOutputFilePath);
	bool run(const RyujinObfuscatorConfig& config, const std::shared_ptr<RyuJinConfigInternal>& ryujConfigInternal);
	void listRyujinProcedures();
	~Ryujin() { }

};

