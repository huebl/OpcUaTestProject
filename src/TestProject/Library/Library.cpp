/*
   Copyright 2015-2022 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */


#include <iostream>
#include "BuildConfig.h"

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Log.h"

#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"
#include "OpcUaStackServer/ServiceSetApplication/GetNamespaceInfo.h"
#include "OpcUaStackServer/ServiceSetApplication/GetNodeReference.h"
#include "OpcUaStackServer/ServiceSetApplication/RegisterForwardNode.h"

#include "TestProject/Library/Library.h"


using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace TestProject
{

	Library::Library(void)
	: ApplicationIf()
	{
	}

	Library::~Library(void)
	{
	}

	bool
	Library::startup(void)
	{
		Log(Debug, "Library::startup");

		//
		// map namespace name to namespace index
		//
		if (!getNamespaceInfo("http://Server-Schnittstelle_1", serverNamespaceIdx_)) return false;

		//
		// set node ids
		//
		productionStartNodeId_.set((uint32_t)20, serverNamespaceIdx_);
		plcDataHandlingNodeId_.set((uint32_t)284, serverNamespaceIdx_);
		partsAvailableNodeId1_.set((uint32_t)293, serverNamespaceIdx_);
		partsAvailableNodeId2_.set((uint32_t)294, serverNamespaceIdx_);
		partsAvailableNodeId3_.set((uint32_t)295, serverNamespaceIdx_);
		partsAvailableNodeId4_.set((uint32_t)296, serverNamespaceIdx_);
		partsAvailableNodeId5_.set((uint32_t)297, serverNamespaceIdx_);
		resultsAvailableNodeId1_.set((uint32_t)285, serverNamespaceIdx_);
		resultsAvailableNodeId2_.set((uint32_t)286, serverNamespaceIdx_);
		resultsAvailableNodeId3_.set((uint32_t)287, serverNamespaceIdx_);
		resultsAvailableNodeId4_.set((uint32_t)288, serverNamespaceIdx_);
		resultsAvailableNodeId5_.set((uint32_t)289, serverNamespaceIdx_);

		//
		// register customer data types
		//
		if (!registerCustomerDataTypes()) return false;

		//
		// get references to nodes in the opc ua information model
		//
		if (!readNodeReference(productionStartNodeId_, productionStartNode_)) return false;
		if (!readNodeReference(plcDataHandlingNodeId_, plcDataHandlingNode_)) return false;
		if (!readNodeReference(partsAvailableNodeId1_, partsAvailableNode1_)) return false;
		if (!readNodeReference(partsAvailableNodeId2_, partsAvailableNode2_)) return false;
		if (!readNodeReference(partsAvailableNodeId3_, partsAvailableNode3_)) return false;
		if (!readNodeReference(partsAvailableNodeId4_, partsAvailableNode4_)) return false;
		if (!readNodeReference(partsAvailableNodeId5_, partsAvailableNode5_)) return false;
		if (!readNodeReference(resultsAvailableNodeId1_, resultsAvailableNode1_)) return false;
		if (!readNodeReference(resultsAvailableNodeId2_, resultsAvailableNode2_)) return false;
		if (!readNodeReference(resultsAvailableNodeId3_, resultsAvailableNode3_)) return false;
		if (!readNodeReference(resultsAvailableNodeId4_, resultsAvailableNode4_)) return false;
		if (!readNodeReference(resultsAvailableNodeId5_, resultsAvailableNode5_)) return false;

		//
		// set default values after information model startup
		//

		// set default PLCDataHandling
		OpcUaExtensionObject plcDataHandlingEO;
		auto plcDataHandling = plcDataHandlingEO.getObjectRef<PLCDataHandling>();
		plcDataHandling->results_available_1() = false;
		plcDataHandling->results_available_2() = false;
		plcDataHandling->results_available_3() = false;
		plcDataHandling->results_available_4() = false;
		plcDataHandling->results_available_5() = false;

		// set default values
		if (!setValue(productionStartNodeId_, productionStartNode_, false)) return false;
		if (!setValue(plcDataHandlingNodeId_, plcDataHandlingNode_, plcDataHandlingEO)) return false;
		if (!setValue(partsAvailableNodeId1_, partsAvailableNode1_, false)) return false;
		if (!setValue(partsAvailableNodeId2_, partsAvailableNode2_, false)) return false;
		if (!setValue(partsAvailableNodeId3_, partsAvailableNode3_, false)) return false;
		if (!setValue(partsAvailableNodeId4_, partsAvailableNode4_, false)) return false;
		if (!setValue(partsAvailableNodeId5_, partsAvailableNode5_, false)) return false;
		if (!setValue(resultsAvailableNodeId1_, resultsAvailableNode1_, false)) return false;
		if (!setValue(resultsAvailableNodeId2_, resultsAvailableNode2_, false)) return false;
		if (!setValue(resultsAvailableNodeId3_, resultsAvailableNode3_, false)) return false;
		if (!setValue(resultsAvailableNodeId4_, resultsAvailableNode4_, false)) return false;
		if (!setValue(resultsAvailableNodeId5_, resultsAvailableNode5_, false)) return false;

		//
		// register write callback function
		//
		if (!registerWriteCallback(productionStartNodeId_, std::bind(&Library::writeCallbackProductionStart, this, std::placeholders::_1))) return false;

		return true;
	}

	bool
	Library::shutdown(void)
	{
		Log(Debug, "Library::shutdown");
		return true;
	}

	std::string
	Library::version(void)
	{
		std::stringstream version;

		version << LIBRARY_VERSION_MAJOR << "." << LIBRARY_VERSION_MINOR << "." << LIBRARY_VERSION_PATCH;
		return version.str();
	}

	std::string
	Library::gitCommit(void)
	{
		return LIBRARY_GIT_COMMIT;
	}

	std::string
	Library::gitBranch(void)
	{
		return LIBRARY_GIT_BRANCH;
	}

	void
	Library::writeCallbackProductionStart(
		ApplicationWriteContext* applicationWriteContext
	)
	{
		OpcUaStatusCode statusCode;
		OpcUaBoolean value;
		OpcUaExtensionObject plcDataHandlingEO;
		auto plcDataHandling = plcDataHandlingEO.getObjectRef<PLCDataHandling>();

		// get/set partsAvailable1 -> partsAvailable1
		statusCode = getValue(partsAvailableNodeId1_, partsAvailableNode1_, value);
		setValue(resultsAvailableNodeId1_, resultsAvailableNode1_, value, statusCode);
		plcDataHandling->results_available_1() = value;

		// get/set partsAvailable2 -> partsAvailable2
		statusCode = getValue(partsAvailableNodeId2_, partsAvailableNode2_, value);
		setValue(resultsAvailableNodeId2_, resultsAvailableNode2_, value, statusCode);
		plcDataHandling->results_available_2() = value;

		// get/set partsAvailable3 -> partsAvailable3
		statusCode = getValue(partsAvailableNodeId3_, partsAvailableNode3_, value);
		setValue(resultsAvailableNodeId3_, resultsAvailableNode3_, value, statusCode);
		plcDataHandling->results_available_3() = value;

		// get/set partsAvailable4 -> partsAvailable4
		statusCode = getValue(partsAvailableNodeId4_, partsAvailableNode4_, value);
		setValue(resultsAvailableNodeId4_, resultsAvailableNode4_, value, statusCode);
		plcDataHandling->results_available_4() = value;

		// get/set partsAvailable5 -> partsAvailable5
		statusCode = getValue(partsAvailableNodeId5_, partsAvailableNode5_, value);
		setValue(resultsAvailableNodeId5_, resultsAvailableNode5_, value, statusCode);
		plcDataHandling->results_available_5() = value;


		// set PLCDataHandling value
		setValue(plcDataHandlingNodeId_, plcDataHandlingNode_, plcDataHandlingEO);

		// set production call value
		applicationWriteContext->dataValue_.set((OpcUaBoolean)false);
	}

	bool
	Library::registerCustomerDataTypes(void)
	{
		OpcUaExtensionObject eo;

		// register data type PLCDataHandling
		if (!eo.registerFactoryObject<PLCDataHandling>()) {
			Log(Error, "register data type error")
				.parameter("DataType", "PLCDataHandling");
			return false;
		}

		return true;
	}

	bool
	Library::getNamespaceInfo(
		const std::string namespaceName,
		uint32_t& namespaceIndex
	)
	{
		GetNamespaceInfo getNamespaceInfo;

		// read namespace array
		if (!getNamespaceInfo.query(&service())) {
			Log(Error, "get namespace info error")
			    .parameter("NamespaceName", namespaceName);
			return false;
		}

		// map namespace name to namespace index
		namespaceIndex = getNamespaceInfo.getNamespaceIndex(namespaceName);
		if (namespaceIndex == -1) {
			Log(Error, "namespace name not exist in namespace array")
			    .parameter("NamespaceName", namespaceName);
			return false;
		}

		return true;
	}

	bool
	Library::readNodeReference(
		const OpcUaNodeId& nodeId,
		BaseNodeClass::WPtr& baseNode
	)
	{
		// read node references
		GetNodeReference getNodeReference(nodeId);
		if (!getNodeReference.query(&service())) {
			Log(Error, "get node reference error")
			    .parameter("NodeId", nodeId);
	  		return false;
		}

		// check number of references in result
		if (getNodeReference.nodeReferences().size() != 1) {
			Log(Error, "number of results in get node reference error")
			    .parameter("NodeId", nodeId)
				.parameter("Results", getNodeReference.nodeReferences().size());
	  		return false;
		}

		// set reference to node
		baseNode = getNodeReference.nodeReferences()[0];

		return true;
	}

	bool
	Library::setValue(
		const OpcUaNodeId& nodeId,
		BaseNodeClass::WPtr& baseNode,
		OpcUaBoolean value,
		OpcUaStackCore::OpcUaStatusCode statusCode
	)
	{
  		auto baseNodeClass = baseNode.lock();
  		if (baseNodeClass.get() == nullptr) {
			Log(Error, "set value error, because node no longer exist in opc ua information model")
				.parameter("NodeId", nodeId);
  			return false;
  		}

  		if (statusCode == Success) {
  			baseNodeClass->setValueSync(OpcUaDataValue((OpcUaBoolean)value, statusCode));
  		}
  		else {
  			baseNodeClass->setValueSync(OpcUaDataValue(OpcUaNullValue(), statusCode));
  		}

		return true;
	}

	bool
	Library::setValue(
		const OpcUaNodeId& nodeId,
		BaseNodeClass::WPtr& baseNode,
		OpcUaExtensionObject& value,
		OpcUaStatusCode statusCode
	)
	{
  		auto baseNodeClass = baseNode.lock();
  		if (baseNodeClass.get() == nullptr) {
			Log(Error, "set value error, because node no longer exist in opc ua information model")
				.parameter("NodeId", nodeId);
  			return false;
  		}

  		if (statusCode == Success) {
  			baseNodeClass->setValueSync(OpcUaDataValue(value, statusCode));
  		}
  		else {
  			baseNodeClass->setValueSync(OpcUaDataValue(OpcUaNullValue(), statusCode));
  		}
		return true;
	}

	OpcUaStatusCode
	Library::getValue(
		const OpcUaNodeId& nodeId,
		BaseNodeClass::WPtr& baseNode,
		OpcUaBoolean& value
	)
	{
  		auto baseNodeClass = baseNode.lock();
  		if (baseNodeClass.get() == nullptr) {
			Log(Error, "get value error, because node no longer exist in opc ua information model")
				.parameter("NodeId", nodeId);
  			return BadInternalError;
  		}

  		OpcUaDataValue dataValue;
  		baseNodeClass->getValueSync(dataValue);

  		// check opc ua value type
  		if (dataValue.variant()->variantType() != OpcUaBuildInType_OpcUaBoolean) {
			Log(Error, "get value error, because value type invalid")
				.parameter("NodeId", nodeId)
				.parameter("ValueType", (uint32_t)dataValue.variant()->variantType());
			return BadInternalError;
  		}

  		// get value from opc ua value
  		dataValue.getValue(value);

		return Success;
	}

	bool
	Library::registerWriteCallback(
		const OpcUaNodeId& nodeId,
		WriteCallback writeCallback
	)
	{
		RegisterForwardNode registerForwardNode(nodeId);
		registerForwardNode.setWriteCallback(writeCallback);
		if (!registerForwardNode.query(&service(), true)) {
			Log(Error, "register write callback error")
				.parameter("NodeId", nodeId);
			return false;
		}

		return true;
	}

}

extern "C" DLLEXPORT void  init(ApplicationIf** applicationIf) {
    *applicationIf = new TestProject::Library();
}

