
SET NODESETS=--nodeset ../Config/Opc.Ua.NodeSet2.xml --nodeset ../Config/plc.xml

OpcUaDataTypeGenerator4.exe %NODESETS% --datatype SimaticStructures --namespaces "1:TestProject"
OpcUaDataTypeGenerator4.exe %NODESETS% --datatype "\"DB OPC UA Additonal Data\".\"Line Data Handling\"" --namespaces "1:TestProject" --namespaces "2:TestProject" --classname LineDataHandling 
OpcUaDataTypeGenerator4.exe %NODESETS% --datatype "\"DB OPC UA Additonal Data\".\"PLC Data Handling\"" --namespaces "1:TestProject" --namespaces "2:TestProject" --classname PLCDataHandling 



REM OpcUaDataTypeGenerator4.exe %NODESETS% --datatype ServerStatusDataType
