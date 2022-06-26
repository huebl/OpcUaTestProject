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

#ifndef __TestProject_Library_h__
#define __TestProject_Library_h__

#include "OpcUaStackCore/BuildInTypes/OpcUaStatusCode.h"
#include "OpcUaStackServer/Application/ApplicationIf.h"
#include "OpcUaStackServer/AddressSpaceModel/BaseNodeClass.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaExtensionObject.h"

#include "TestProject/CustomerDataTypes/PLCDataHandling.h"

namespace TestProject
{

	class Library
	: public OpcUaStackServer::ApplicationIf
	{
	  public:
		Library(void);
		virtual ~Library(void);

		using WriteCallback = std::function<void (OpcUaStackCore::ApplicationWriteContext* applicationWriteContext)>;

		//- ApplicationIf -----------------------------------------------------
		bool startup(void) override;
		bool shutdown(void) override;
		std::string version(void) override;
		std::string gitCommit(void) override;
		std::string gitBranch(void) override;
		//- ApplicationIf -----------------------------------------------------

	  private:
		uint32_t serverNamespaceIdx_ = 0;


		// node ids
		OpcUaStackCore::OpcUaNodeId productionStartNodeId_;

		OpcUaStackCore::OpcUaNodeId plcDataHandlingNodeId_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId1_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId2_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId3_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId4_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId5_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId1_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId2_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId3_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId4_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId5_;


		// node references
		OpcUaStackServer::BaseNodeClass::WPtr productionStartNode_;

		OpcUaStackServer::BaseNodeClass::WPtr plcDataHandlingNode_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode1_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode2_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode3_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode4_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode5_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode1_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode2_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode3_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode4_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode5_;

		void writeCallbackProductionStart(
			OpcUaStackCore::ApplicationWriteContext* applicationWriteContext
		);


		bool registerCustomerDataTypes(void);
		bool getNamespaceInfo(
			const std::string namespaceName,
			uint32_t& namespaceIndex
		);
		bool readNodeReference(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			OpcUaStackServer::BaseNodeClass::WPtr& baseNode
		);
		bool setValue(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			OpcUaStackServer::BaseNodeClass::WPtr& baseNode,
			OpcUaStackCore::OpcUaBoolean value,
			OpcUaStackCore::OpcUaStatusCode statusCode = OpcUaStackCore::Success
		);
		bool setValue(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			OpcUaStackServer::BaseNodeClass::WPtr& baseNode,
			OpcUaStackCore::OpcUaExtensionObject& value,
			OpcUaStackCore::OpcUaStatusCode statusCode = OpcUaStackCore::Success
		);
		OpcUaStackCore::OpcUaStatusCode getValue(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			OpcUaStackServer::BaseNodeClass::WPtr& baseNode,
			OpcUaStackCore::OpcUaBoolean& value
		);
		bool registerWriteCallback(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			WriteCallback writeCallback
		);
	};

}

#endif
