#pragma once
#include <iostream>
#include <memory>
#include <Windows.h>

#define ALIGN_UP(value, alignment) ((value + alignment - 1) & ~(alignment - 1))

namespace RyujinUtils {

	inline std::pair<BOOL, uintptr_t> MapPortableExecutableFileIntoMemory(const std::string& m_strInputFilePath, std::shared_ptr<unsigned char>& mappedPE) {

		auto hFile = ::CreateFileA(

			_In_ m_strInputFilePath.c_str(),
			_In_ GENERIC_READ,
			_In_ FILE_SHARE_READ | FILE_SHARE_DELETE,
			_In_opt_ nullptr,
			_In_ OPEN_EXISTING,
			_In_ FILE_ATTRIBUTE_NORMAL,
			_In_opt_ nullptr

		);

		if (hFile == INVALID_HANDLE_VALUE) {

			OutputDebugStringA(
				
				_In_opt_ "RyujinUtils::MapExecutableFileIntoMemory: failed because cannot open a handle for input PE FILE\n"
			
			);

			return std::make_pair(FALSE, 0);
		}

		LARGE_INTEGER szFile;
		if (!::GetFileSizeEx(
			_In_ hFile,
			_Out_ &szFile
		)) {

			::OutputDebugStringA(

				_In_opt_ "RyujinUtils::MapExecutableFileIntoMemory: was not possible to get filesizeru for the PE FILE\n"

			);

			::CloseHandle(

				_In_ hFile

			);

			return std::make_pair(FALSE, 0);
		}

		auto hMap = ::CreateFileMappingA(

			_In_ hFile,
			_In_opt_ nullptr,
			_In_ PAGE_READONLY | SEC_IMAGE,
			_In_ NULL,
			_In_ NULL,
			_In_opt_ nullptr

		);

		if (!hMap) {

			::OutputDebugStringA(

				_In_opt_ "RyujinUtils::MapExecutableFileIntoMemory: was not possible to create a mapping to the PE FILE.\n"

			);

			::CloseHandle(

				_In_ hMap

			);

			return std::make_pair(FALSE, 0);
		}

		mappedPE = std::shared_ptr<unsigned char>(

			reinterpret_cast<unsigned char*>(

				::MapViewOfFile(
					
					_In_ hMap,
					_In_ FILE_MAP_READ,
					_In_ NULL,
					_In_ NULL,
					_In_ NULL
				
				)

			),
			[](unsigned char* p) {

				if (p) ::UnmapViewOfFile(
					
					_In_ p
				
				);

			}

		);

		::CloseHandle(

			_In_ hMap

		);

		::CloseHandle(
		
			_In_ hFile

		);

		return std::make_pair(TRUE, szFile.QuadPart);
	}

	inline BOOL AddNewSection(std::shared_ptr<unsigned char>& mappedPE, uintptr_t szFile, char chSectionName[8]) {

		auto ucModifiedPeMap = new unsigned char[szFile] { 0 };
		std::memcpy(
			
			ucModifiedPeMap,
			mappedPE.get(),
			szFile
		
		);

		auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(ucModifiedPeMap);
		if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
			
			delete[] ucModifiedPeMap;

			return FALSE;
		}

		auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(ucModifiedPeMap + dosHeader->e_lfanew);
		if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {

			delete[] ucModifiedPeMap;

			return FALSE;
		}

		auto sectionTableSize = ntHeaders->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
		if (dosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS) + sectionTableSize > ntHeaders->OptionalHeader.SizeOfHeaders) {
			
			//No space to insert a new section on this PE FILE :(

			delete[] ucModifiedPeMap;

			return FALSE;
		}

		//Adding the new section
		IMAGE_SECTION_HEADER newSection{ 0 };
		std::memcpy(
			
			newSection.Name,
			chSectionName,
			sizeof(chSectionName)
		
		);

		auto imgLastSection = IMAGE_FIRST_SECTION(ntHeaders) + (ntHeaders->FileHeader.NumberOfSections - 1);

		newSection.VirtualAddress = ALIGN_UP(
			imgLastSection->VirtualAddress + imgLastSection->Misc.VirtualSize,
			ntHeaders->OptionalHeader.SectionAlignment
		);

		newSection.PointerToRawData = ALIGN_UP(
			imgLastSection->PointerToRawData + imgLastSection->SizeOfRawData,
			ntHeaders->OptionalHeader.FileAlignment
		);

		newSection.Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

		//Return the following in order to put data and calculate relocs: 
		//newSection.VirtualAddress
		//newSection.Misc.VirtualSize = size of new code;
		//newSection.SizeOfRawData = ALIGN_UP(size of new code, ntHeaders->OptionalHeader.FileAlignment);
		//Maybe is better wrap this things in a full new class ??

		return FALSE;
	}

	inline void randomizeSectionName(char* chName) {

		const char charset[] { "abcdefghijklmnopqrstuvwxyz" };
		for (size_t i = 0; i < 8 - 1; ++i) chName[i] = charset[std::rand() % (sizeof(charset) - 1)];
		chName[8 - 1] = '\0';

	}

};

