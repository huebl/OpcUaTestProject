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

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Log.h"
#include "TestProject/Library/Library.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"
#include "OpcUaStackServer/ServiceSetApplication/GetNamespaceInfo.h"
#include "OpcUaStackServer/ServiceSetApplication/GetNodeReference.h"
#include "OpcUaStackServer/ServiceSetApplication/RegisterForwardNode.h"
#include <iostream>
#include "BuildConfig.h"

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

		// map namespace name to namespace index
		if (!getNamespaceInfo("http://Server-Schnittstelle_1", serverNamespace_)) return false;

		productionStartNodeId_.set((uint32_t)20, serverNamespace_);
		partsAvailableNodeId_.set((uint32_t)293, serverNamespace_);
		resultsAvailableNodeId_.set((uint32_t)285, serverNamespace_);

		// get references to nodes in opc ua information model
		if (!readNodeReference(productionStartNodeId_, productionStartNode_)) return false;
		if (!readNodeReference(partsAvailableNodeId_, partsAvailableNode_)) return false;
		if (!readNodeReference(resultsAvailableNodeId_, resultsAvailableNode_)) return false;

		// set default values after information model startup
		if (!setValue(productionStartNodeId_, productionStartNode_, false)) return false;
		if (!setValue(partsAvailableNodeId_, partsAvailableNode_, true)) return false;
		if (!setValue(resultsAvailableNodeId_, resultsAvailableNode_, true)) return false;

		// register write callback function
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
		OpcUaBoolean value;

		// get a value
		getValue(partsAvailableNodeId_, partsAvailableNode_, value);

		// set a value
		setValue(resultsAvailableNodeId_, resultsAvailableNode_, value);

		// set production call value
		//setValue(productionStartNodeId_, productionStartNode_, false);
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
		OpcUaBoolean value
	)
	{
  		auto baseNodeClass = baseNode.lock();
  		if (baseNodeClass.get() == nullptr) {
			Log(Error, "set value error, because node no longer exist in opc ua information model")
				.parameter("NodeId", nodeId);
  			return false;
  		}

		OpcUaDateTime dateTime(boost::posix_time::microsec_clock::universal_time());

		OpcUaDataValue dataValue;
		dataValue.serverTimestamp(dateTime);
		dataValue.sourceTimestamp(dateTime);
		dataValue.setValue(value);
		dataValue.statusCode(Success);
		baseNodeClass->setValueSync(dataValue);

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

  		// check value type
  		if (dataValue.variant()->variantType() != OpcUaBuildInType_OpcUaBoolean) {
			Log(Error, "get value error, because value type invalid")
				.parameter("NodeId", nodeId)
				.parameter("ValueType", (uint32_t)dataValue.variant()->variantType());
			return BadInternalError;
  		}

  		// get value
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

