#include "includes.h"
#include <format>
#include <iostream>

extern Hack *hack;

void SetupNetvars() {
    for (auto clientClass = hack->chlClient->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext) {
        if (clientClass->m_pRecvTable) {
            Dump(clientClass->m_pNetworkName, clientClass->m_pRecvTable);
        }
    }
}

void Dump(const char *baseClass, RecvTable *table, uint32_t offset) {
    for (auto i = 0; i < table->m_nProps; ++i) {
        const auto prop = &table->m_pProps[i];
        if (isdigit(prop->m_pVarName[0])) {
            continue;
        }
        if (fnv::Hash(prop->m_pVarName) == fnv::HashConst("baseclass")) {
            continue;
        }
        if (prop->m_RecvType == SendPropType::DPT_DataTable && prop->m_pDataTable && prop->m_pDataTable->m_pNetTableName[0] == 'D') {
            Dump(baseClass, prop->m_pDataTable, offset + prop->m_Offset);
        }
        const auto netvarName = std::format("{}->{}", baseClass, prop->m_pVarName);
        netvars[fnv::Hash(netvarName.c_str())] = offset + prop->m_Offset;
        std::cout << netvarName << ": 0x" << std::hex << (offset + prop->m_Offset) << std::endl;
    }
}
