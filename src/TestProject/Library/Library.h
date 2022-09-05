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

		void writeCallbackProductionStart(
			OpcUaStackCore::ApplicationWriteContext* applicationWriteContext
		);

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
		OpcUaStackCore::OpcUaStatusCode getValue(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			OpcUaStackServer::BaseNodeClass::WPtr& baseNode,
			OpcUaStackCore::OpcUaBoolean& value
		);
		bool registerWriteCallback(
			const OpcUaStackCore::OpcUaNodeId& nodeId,
			WriteCallback writeCallback
		);

	  private:
		uint32_t serverNamespace_ = 0;

		OpcUaStackCore::IOThread::SPtr ioThread_;
		OpcUaStackCore::SlotTimerElement::SPtr slotTimerElement_;

		OpcUaStackCore::OpcUaNodeId productionStartNodeId_;
		OpcUaStackCore::OpcUaNodeId partsAvailableNodeId_;
		OpcUaStackCore::OpcUaNodeId resultsAvailableNodeId_;

		OpcUaStackServer::BaseNodeClass::WPtr productionStartNode_;
		OpcUaStackServer::BaseNodeClass::WPtr  partsAvailableNode_;
		OpcUaStackServer::BaseNodeClass::WPtr  resultsAvailableNode_;

		OpcUaStackCore::OpcUaStatusCode statusCode_ = OpcUaStackCore::OpcUaStatusCode::Success;
		OpcUaStackCore::OpcUaBoolean value_ = true;
	};

}

#endif
