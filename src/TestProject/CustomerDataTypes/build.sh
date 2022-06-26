!/bin/bash

NODESETS="--nodeset ../Config/Opc.Ua.NodeSet2.xml --nodeset ../Config/plc.xml"

OpcUaDataTypeGenerator4 ${NODESETS} --datatype SimaticStructures --namespaces "1:TestProject"
OpcUaDataTypeGenerator4 ${NODESETS} --datatype "\"DB OPC UA Additonal Data\".\"Line Data Handling\"" --namespaces "1:TestProject" --namespaces "2:TestProject" --classname LineDataHandling 
OpcUaDataTypeGenerator4 ${NODESETS} --datatype "\"DB OPC UA Additonal Data\".\"PLC Data Handling\"" --namespaces "1:TestProject" --namespaces "2:TestProject" --classname PLCDataHandling 



#OpcUaDataTypeGenerator4 ${NODESETS} --datatype ServerStatusDataType
