#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <QtCore/QDebug>

#include <qdom.h>
#include <qxml.h>
#include <qxml.h>
#include <qdom.h>
#include <qxml.h>
#include <qdom.h>
#include <qxml.h>
#include <qxml.h>
#include <qxml.h>
#include <qxml.h>
#include <qxml.h>
#include <qxml.h>
#include <qxml.h>
#include <qdom.h>
#include <qxml.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qxml.h>
#include <qdom.h>
#include <qxml.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>
#include <qdom.h>

QScriptValue qtscript_create_QDomNamedNodeMap_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlReader_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlLexicalHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomNodeList_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlErrorHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomImplementation_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlLocator_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlAttributes_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlDTDHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlContentHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlInputSource_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlEntityResolver_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlDeclHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomNode_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlParseException_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomNotation_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomDocumentType_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomProcessingInstruction_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlDefaultHandler_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomDocumentFragment_class(QScriptEngine *engine);
QScriptValue qtscript_create_QXmlSimpleReader_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomCharacterData_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomDocument_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomEntity_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomEntityReference_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomAttr_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomElement_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomText_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomComment_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDomCDATASection_class(QScriptEngine *engine);

static const char * const qtscript_com_trolltech_qt_xml_class_names[] = {
    "QDomNamedNodeMap"
    , "QXmlReader"
    , "QXmlLexicalHandler"
    , "QDomNodeList"
    , "QXmlErrorHandler"
    , "QDomImplementation"
    , "QXmlLocator"
    , "QXmlAttributes"
    , "QXmlDTDHandler"
    , "QXmlContentHandler"
    , "QXmlInputSource"
    , "QXmlEntityResolver"
    , "QXmlDeclHandler"
    , "QDomNode"
    , "QXmlParseException"
    , "QDomNotation"
    , "QDomDocumentType"
    , "QDomProcessingInstruction"
    , "QXmlDefaultHandler"
    , "QDomDocumentFragment"
    , "QXmlSimpleReader"
    , "QDomCharacterData"
    , "QDomDocument"
    , "QDomEntity"
    , "QDomEntityReference"
    , "QDomAttr"
    , "QDomElement"
    , "QDomText"
    , "QDomComment"
    , "QDomCDATASection"
};

typedef QScriptValue (*QtBindingCreator)(QScriptEngine *engine);
static const QtBindingCreator qtscript_com_trolltech_qt_xml_class_functions[] = {
    qtscript_create_QDomNamedNodeMap_class
    , qtscript_create_QXmlReader_class
    , qtscript_create_QXmlLexicalHandler_class
    , qtscript_create_QDomNodeList_class
    , qtscript_create_QXmlErrorHandler_class
    , qtscript_create_QDomImplementation_class
    , qtscript_create_QXmlLocator_class
    , qtscript_create_QXmlAttributes_class
    , qtscript_create_QXmlDTDHandler_class
    , qtscript_create_QXmlContentHandler_class
    , qtscript_create_QXmlInputSource_class
    , qtscript_create_QXmlEntityResolver_class
    , qtscript_create_QXmlDeclHandler_class
    , qtscript_create_QDomNode_class
    , qtscript_create_QXmlParseException_class
    , qtscript_create_QDomNotation_class
    , qtscript_create_QDomDocumentType_class
    , qtscript_create_QDomProcessingInstruction_class
    , qtscript_create_QXmlDefaultHandler_class
    , qtscript_create_QDomDocumentFragment_class
    , qtscript_create_QXmlSimpleReader_class
    , qtscript_create_QDomCharacterData_class
    , qtscript_create_QDomDocument_class
    , qtscript_create_QDomEntity_class
    , qtscript_create_QDomEntityReference_class
    , qtscript_create_QDomAttr_class
    , qtscript_create_QDomElement_class
    , qtscript_create_QDomText_class
    , qtscript_create_QDomComment_class
    , qtscript_create_QDomCDATASection_class
};

class com_trolltech_qt_xml_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList com_trolltech_qt_xml_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("qt");
    list << QLatin1String("qt.xml");
    return list;
}

void com_trolltech_qt_xml_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("qt")) {
    } else if (key == QLatin1String("qt.xml")) {
        QScriptValue extensionObject = engine->globalObject();
        for (int i = 0; i < 30; ++i) {
            extensionObject.setProperty(qtscript_com_trolltech_qt_xml_class_names[i],
                qtscript_com_trolltech_qt_xml_class_functions[i](engine),
                QScriptValue::SkipInEnumeration);
        }
    } else {
        Q_ASSERT_X(false, "com_trolltech_qt_xml::initialize", qPrintable(key));
    }
}
Q_EXPORT_STATIC_PLUGIN(com_trolltech_qt_xml_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_com_trolltech_qt_xml, com_trolltech_qt_xml_ScriptPlugin)

