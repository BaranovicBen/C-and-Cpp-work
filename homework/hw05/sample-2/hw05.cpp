#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <memory>
#include <stdexcept>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#endif /* __PROGTEST__ */

struct Export {
    std::string name;
    uint32_t offset;
    uint32_t endOffset;
};

struct Import {
    std::string name;
    std::vector<uint32_t> callOffsets;
};

struct ObjectFile {
    std::vector<Export> exports;
    std::vector<Import> imports;
    std::vector<uint8_t> code;
};

class CLinker
{
  public:
    std::vector<ObjectFile> m_Files;

    CLinker(const CLinker&) = delete;
    CLinker& operator= (const CLinker&) = delete;
    CLinker() = default;

    CLinker& addFile(const std::string& fileName)
    {
        std::ifstream file(fileName, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot read input file");
        }

        ObjectFile objFile;
        uint32_t exportCount = 0, importCount = 0, codeSize = 0;

        file.read(reinterpret_cast<char*>(&exportCount), sizeof(exportCount));
        if (!file) {
            throw std::runtime_error("Invalid object file format (cannot read export count)");
        }

        file.read(reinterpret_cast<char*>(&importCount), sizeof(importCount));
        if (!file) {
            throw std::runtime_error("Invalid object file format (cannot read import count)");
        }

        file.read(reinterpret_cast<char*>(&codeSize), sizeof(codeSize));
        if (!file) {
            throw std::runtime_error("Invalid object file format (cannot read code size)");
        }

        for (uint32_t i = 0; i < exportCount; ++i) {
            Export exp;
            uint8_t nameLen = 0;
            file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read export name length)");
            }

            std::vector<char> nameBuffer(nameLen);
            file.read(nameBuffer.data(), nameLen);
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read export name)");
            }

            exp.name.assign(nameBuffer.begin(), nameBuffer.end());

            file.read(reinterpret_cast<char*>(&exp.offset), sizeof(exp.offset));
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read export offset)");
            }

            objFile.exports.push_back(std::move(exp));
        }

        for (uint32_t i = 0; i < importCount; ++i) {
            Import imp;
            uint8_t nameLen = 0;
            file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read import name length)");
            }

            std::vector<char> nameBuffer(nameLen);
            file.read(nameBuffer.data(), nameLen);
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read import name)");
            }

            imp.name.assign(nameBuffer.begin(), nameBuffer.end());

            uint32_t refCount = 0;
            file.read(reinterpret_cast<char*>(&refCount), sizeof(refCount));
            if (!file) {
                throw std::runtime_error("Invalid object file format (cannot read import reference count)");
            }

            for (uint32_t j = 0; j < refCount; ++j) {
                uint32_t callOffset = 0;
                file.read(reinterpret_cast<char*>(&callOffset), sizeof(callOffset));
                if (!file) {
                    throw std::runtime_error("Invalid object file format (cannot read import call offset)");
                }
                imp.callOffsets.push_back(callOffset);
            }

            objFile.imports.push_back(std::move(imp));
        }

        objFile.code.resize(codeSize);
        file.read(reinterpret_cast<char*>(objFile.code.data()), codeSize);
        if (!file) {
            throw std::runtime_error("Invalid object file format (cannot read compiled code)");
        }

        std::sort(objFile.exports.begin(), objFile.exports.end(), [](const Export& a, const Export& b) {
            return a.offset < b.offset;
        });

        for (size_t i = 0; i + 1 < objFile.exports.size(); ++i) {
            objFile.exports[i].endOffset = objFile.exports[i + 1].offset;
        }
        if (!objFile.exports.empty()) {
            objFile.exports.back().endOffset = codeSize;
        }

        m_Files.push_back(std::move(objFile));
        return *this;
    }

    void linkOutput(const std::string& outFileName, const std::string& entryPoint)
{
    std::unordered_map<std::string, std::pair<const ObjectFile*, const Export*>> symbolTable;

    for (std::vector<ObjectFile>::const_iterator it = m_Files.begin(); it != m_Files.end(); ++it) {
        for (std::vector<Export>::const_iterator jt = it->exports.begin(); jt != it->exports.end(); ++jt) {
            if (symbolTable.count(jt->name)) {
                throw std::runtime_error("Duplicate symbol: " + jt->name);
            }
            symbolTable[jt->name] = std::make_pair(&(*it), &(*jt));
        }
    }

    std::unordered_map<std::string, std::pair<const ObjectFile*, const Export*>>::const_iterator itEntry = symbolTable.find(entryPoint);
    if (itEntry == symbolTable.end()) {
        throw std::runtime_error("Undefined symbol " + entryPoint);
    }

    std::queue<std::string> toProcess;
    std::unordered_set<std::string> processed;
    std::unordered_set<std::string> inQueue;
    std::vector<std::pair<const ObjectFile*, const Export*>> orderedFunctions;

    toProcess.push(entryPoint);
    inQueue.insert(entryPoint);

    while (!toProcess.empty()) {
        std::string currentFunction = toProcess.front();
        toProcess.pop();

        if (processed.count(currentFunction)) {
            continue;
        }
        processed.insert(currentFunction);

        const ObjectFile* objFile = symbolTable.at(currentFunction).first;
        const Export* exp = symbolTable.at(currentFunction).second;

        orderedFunctions.push_back(std::make_pair(objFile, exp));

        for (std::vector<Import>::const_iterator itImp = objFile->imports.begin(); itImp != objFile->imports.end(); ++itImp) {
            for (std::vector<uint32_t>::const_iterator itCall = itImp->callOffsets.begin(); itCall != itImp->callOffsets.end(); ++itCall) {
                if (*itCall >= exp->offset && *itCall < exp->endOffset) {
                    if (symbolTable.find(itImp->name) == symbolTable.end()) {
                        throw std::runtime_error("Undefined symbol " + itImp->name);
                    }
                    if (!processed.count(itImp->name) && !inQueue.count(itImp->name)) {
                        toProcess.push(itImp->name);
                        inQueue.insert(itImp->name);
                    }
                }
            }
        }
    }

    std::ofstream outFile(outFileName, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Cannot write output file");
    }

    std::unordered_map<std::string, uint32_t> functionNewOffsets;
    uint32_t currentOffset = 0;

    for (std::vector<std::pair<const ObjectFile*, const Export*>>::const_iterator it = orderedFunctions.begin(); it != orderedFunctions.end(); ++it) {
        const ObjectFile* objFile = it->first;
        const Export* exp = it->second;

        functionNewOffsets[exp->name] = currentOffset;

        uint32_t start = exp->offset;
        uint32_t end = exp->endOffset;
        uint32_t length = end - start;

        outFile.write(reinterpret_cast<const char*>(&objFile->code[start]), length);
        currentOffset += length;
    }

    outFile.flush();

    for (std::vector<std::pair<const ObjectFile*, const Export*>>::const_iterator it = orderedFunctions.begin(); it != orderedFunctions.end(); ++it) {
        const ObjectFile* objFile = it->first;
        const Export* exp = it->second;

        uint32_t funcStartInOutput = functionNewOffsets.at(exp->name);

        for (std::vector<Import>::const_iterator itImp = objFile->imports.begin(); itImp != objFile->imports.end(); ++itImp) {
            std::unordered_map<std::string, uint32_t>::const_iterator targetIt = functionNewOffsets.find(itImp->name);
            if (targetIt == functionNewOffsets.end()) {
                continue;
            }
            uint32_t targetAddress = targetIt->second;

            for (std::vector<uint32_t>::const_iterator itCall = itImp->callOffsets.begin(); itCall != itImp->callOffsets.end(); ++itCall) {
                if (*itCall >= exp->offset && *itCall < exp->endOffset) {
                    uint32_t outputAddress = funcStartInOutput + (*itCall - exp->offset);

                    outFile.seekp(outputAddress);
                    outFile.write(reinterpret_cast<const char*>(&targetAddress), sizeof(targetAddress));
                }
            }
        }
    }
}

};

#ifndef __PROGTEST__
int main (){
  CLinker () . addFile ( "0in0.o" ) . linkOutput ( "0out", "strlen" );

  CLinker () . addFile ( "1in0.o" ) . linkOutput ( "1out", "main" );

  CLinker () . addFile ( "2in0.o" ) . addFile ( "2in1.o" ) . linkOutput ( "2out", "main" );

  CLinker () . addFile ( "3in0.o" ) . addFile ( "3in1.o" ) . linkOutput ( "3out", "towersOfHanoi" );

  try
  {
    CLinker () . addFile ( "4in0.o" ) . addFile ( "4in1.o" ) . linkOutput ( "4out", "unusedFunc" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Undefined symbol qsort
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "5in0.o" ) . linkOutput ( "5out", "main" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Duplicate symbol: printf
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "6in0.o" ) . linkOutput ( "6out", "strlen" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Cannot read input file
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "7in0.o" ) . linkOutput ( "7out", "strlen2" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Undefined symbol strlen2
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */