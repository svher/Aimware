#pragma once

typedef enum
{
    DPT_Int=0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
    DPT_String,
    DPT_Array,	// An array of the base types (can't be of datatables).
    DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
    DPT_Quaternion,
#endif
    DPT_Int64,
    DPT_NUMSendPropTypes
} SendPropType;

class RecvTable;

class RecvProp {
public:

    char					*m_pVarName;
    SendPropType			m_RecvType;
    int						m_Flags;
    int						m_StringBufferSize;


private:

    bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

    // Extra data that certain special property types bind to the property here.
    const void *m_pExtraData;

    // If this is an array (DPT_Array).
    RecvProp				*m_pArrayProp;
    void                 	*m_ArrayLengthProxy;

    void	                *m_ProxyFn;
    void                	*m_DataTableProxyFn;	// For RDT_DataTable.

    RecvTable				*m_pDataTable;		// For RDT_DataTable.
    int						m_Offset;

    int						m_ElementStride;
    int						m_nElements;

    // If it's one of the numbered "000", "001", etc properties in an array, then
    // these can be used to get its array property name for debugging.
    const char				*m_pParentArrayPropName;
};

class RecvTable {
public:

    // Properties described in a table.
    RecvProp		*m_pProps;
    int				m_nProps;

    // The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
    // will have their own decoders that include props for all their children).
    void        	*m_pDecoder;

    char			*m_pNetTableName;	// The name matched between client and server.


private:

    bool			m_bInitialized;
    bool			m_bInMainList;
};

class ClientClass {
public:
    void	            	*m_pCreateFn;
    void       			    *m_pCreateEventFn;	// Only called for event objects.
    char					*m_pNetworkName;
    RecvTable				*m_pRecvTable;
    ClientClass				*m_pNext;
    int						m_ClassID;	// Managed by the engine.
    const char				*m_pMapClassname;
};