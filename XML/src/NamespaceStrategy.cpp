//
// NamespaceStrategy.cpp
//
// $Id: //poco/1.2/XML/src/NamespaceStrategy.cpp#2 $
//
// Library: XML
// Package: XML
// Module:  NamespaceStrategy
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/XML/NamespaceStrategy.h"
#include "Poco/SAX/AttributesImpl.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/XML/XMLException.h"
#include "Poco/XML/Name.h"


namespace Poco {
namespace XML {


const XMLString NamespaceStrategy::NOTHING;
const XMLString NamespaceStrategy::CDATA = toXMLString("CDATA");
const XMLString NamespaceStrategy::COLON = toXMLString(":");


NamespaceStrategy::~NamespaceStrategy()
{
}


void NamespaceStrategy::splitName(const XMLChar* qname, XMLString& uri, XMLString& localName)
{
	for (const XMLChar* p = qname; *p; ++p)
	{
		if (*p == '\t')
		{
			uri.assign(qname, p - qname);
			localName.assign(p + 1);
			return;
		}
	}
	localName = qname;
}


void NamespaceStrategy::splitName(const XMLChar* qname, XMLString& uri, XMLString& localName, XMLString& prefix)
{
	const XMLChar* p = qname;
	while (*p && *p != '\t') ++p;
	if (*p)
	{
		uri.assign(qname, p - qname);
		++p;
		const XMLChar* loc = p;
		while (*p && *p != '\t') ++p;
		localName.assign(loc, p - loc);
		if (*p)
			prefix.assign(++p);
	}
	else localName = qname;
}


NoNamespacesStrategy::NoNamespacesStrategy()
{
}


NoNamespacesStrategy::~NoNamespacesStrategy()
{
}


void NoNamespacesStrategy::startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && atts && pContentHandler);

	AttributesImpl attributes;
	for (int i = 0; *atts; ++i)
	{
		const XMLChar* attrName  = *atts++;
		const XMLChar* attrValue = *atts++;
		attributes.addAttribute(NOTHING, NOTHING, attrName, CDATA, attrValue, i < specifiedCount);
	}
	pContentHandler->startElement(NOTHING, NOTHING, name, attributes);
}


void NoNamespacesStrategy::endElement(const XMLChar* name, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && pContentHandler);

	pContentHandler->endElement(NOTHING, NOTHING, name);
}


NoNamespacePrefixesStrategy::NoNamespacePrefixesStrategy()
{
}


NoNamespacePrefixesStrategy::~NoNamespacePrefixesStrategy()
{
}


void NoNamespacePrefixesStrategy::startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && atts && pContentHandler);

	AttributesImpl attributes;
	for (int i = 0; *atts; ++i)
	{
		const XMLChar* attrName  = *atts++;
		const XMLChar* attrValue = *atts++;
		XMLString attrURI;
		XMLString attrLocal;
		splitName(attrName, attrURI, attrLocal);
		attributes.addAttribute(attrURI, attrLocal, NOTHING, CDATA, attrValue, i < specifiedCount);
	}
	XMLString uri;
	XMLString local;
	splitName(name, uri, local);
	pContentHandler->startElement(uri, local, NOTHING, attributes);
}


void NoNamespacePrefixesStrategy::endElement(const XMLChar* name, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && pContentHandler);

	XMLString uri;
	XMLString local;
	splitName(name, uri, local);
	pContentHandler->endElement(uri, local, NOTHING);
}


NamespacePrefixesStrategy::NamespacePrefixesStrategy()
{
}


NamespacePrefixesStrategy::~NamespacePrefixesStrategy()
{
}


void NamespacePrefixesStrategy::startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && atts && pContentHandler);

	AttributesImpl attributes;
	for (int i = 0; *atts; ++i)
	{
		const XMLChar* attrName  = *atts++;
		const XMLChar* attrValue = *atts++;
		XMLString attrURI;
		XMLString attrLocal;
		XMLString attrQName;
		splitName(attrName, attrURI, attrLocal, attrQName);
		if (!attrQName.empty()) attrQName.append(":");
		attrQName.append(attrLocal);
		attributes.addAttribute(attrURI, attrLocal, attrQName, CDATA, attrValue, i < specifiedCount);
	}
	XMLString uri;
	XMLString local;
	XMLString qname;
	splitName(name, uri, local, qname);
	if (!qname.empty()) qname.append(":");
	qname.append(local);
	pContentHandler->startElement(uri, local, qname, attributes);
}


void NamespacePrefixesStrategy::endElement(const XMLChar* name, ContentHandler* pContentHandler)
{
	poco_assert_dbg (name && pContentHandler);

	XMLString uri;
	XMLString local;
	XMLString qname;
	splitName(name, uri, local, qname);
	if (!qname.empty()) qname.append(":");
	qname.append(local);
	pContentHandler->endElement(uri, local, qname);
}


} } // namespace Poco::XML
