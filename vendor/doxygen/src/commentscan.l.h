/*****************************************************************************
 *
 * Copyright (C) 1997-2020 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby
 * granted. No representations are made about the suitability of this software
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

%option never-interactive
%option prefix="commentscanYY"
%option reentrant
%option extra-type="struct commentscanYY_state *"
%top{
#include <stdint.h>
// forward declare yyscan_t to improve typesafety
#define YY_TYPEDEF_YY_SCANNER_T
struct yyguts_t;
typedef yyguts_t *yyscan_t;
}

%{

/*
 *        includes
 */

#include <map>
#include <stack>
#include <string>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "qcstring.h"
#include "cite.h"
#include "commentscan.h"
#include "condparser.h"
#include "config.h"
#include "debug.h"
#include "docgroup.h"
#include "doxygen.h"
#include "entry.h"
#include "formula.h"
#include "language.h"
#include "message.h"
#include "parserintf.h"
#include "reflist.h"
#include "section.h"
#include "util.h"
#include "reflist.h"

#define USE_STATE2STRING 0

// forward declarations
static bool handleBrief(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleFn(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleDef(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleOverload(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleEnum(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleDefGroup(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleAddToGroup(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleWeakGroup(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleNamespace(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePackage(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleConcept(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleClass(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHeaderFile(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleProtocol(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCategory(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleUnion(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleStruct(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleInterface(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleIdlException(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePage(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleMainpage(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleFile(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleDir(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleExample(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleDetails(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleRaiseWarning(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleNoop(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleName(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleTodo(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleTest(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleBug(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleSubpage(yyscan_t yyscanner,const QCString &s, const StringVector &);
static bool handleDeprecated(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleXRefItem(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleRelated(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleRelatedAlso(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleMemberOf(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleRefItem(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleSection(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleAnchor(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleImage(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCite(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleFormatBlock(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleAddIndex(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleIf(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleIfNot(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleElseIf(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleElse(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleEndIf(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleIngroup(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleNoSubGrouping(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleShowInitializer(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHideInitializer(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCallgraph(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHideCallgraph(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCallergraph(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHideCallergraph(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleReferencedByRelation(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHideReferencedByRelation(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleReferencesRelation(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleHideReferencesRelation(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleInternal(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleStatic(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePure(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePrivate(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePrivateSection(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleProtected(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleProtectedSection(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handlePublic(yyscan_t yyscanner,const QCString &s, const StringVector &);
static bool handlePublicSection(yyscan_t yyscanner,const QCString &s, const StringVector &);
static bool handleToc(yyscan_t yyscanner,const QCString &s, const StringVector &);
static bool handleInherit(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleExtends(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCopyDoc(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCopyBrief(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleCopyDetails(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleParBlock(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleEndParBlock(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleParam(yyscan_t yyscanner,const QCString &, const StringVector &);
static bool handleRetval(yyscan_t yyscanner,const QCString &, const StringVector &);

#if USE_STATE2STRING
static const char *stateToString(int state);
#endif

typedef bool (*DocCmdFunc)(yyscan_t yyscanner,const QCString &name, const StringVector &optList);

enum class CommandSpacing
{
  Invisible, // command sets some property but does not appear in the output.
  Inline,    // command appears inline in the output which can be a brief description.
  Block,     // command starts a new paragraphs / ends a brief description.
  XRef       // command is a cross reference (todo, bug, test, xrefitem).
};

struct DocCmdMap
{
  DocCmdMap(DocCmdFunc h,CommandSpacing s) : handler(h), spacing(s) {}
  DocCmdFunc handler;
  CommandSpacing spacing;
};

// map of command to handler function
static const std::map< std::string, DocCmdMap > docCmdMap =
{
  // command name      handler function           command spacing
  { "addindex",        { &handleAddIndex,         CommandSpacing::Invisible }},
  { "addtogroup",      { &handleAddToGroup,       CommandSpacing::Invisible }},
  { "anchor",          { &handleAnchor,           CommandSpacing::Invisible }},
  { "arg",             { 0,                       CommandSpacing::Block     }},
  { "attention",       { 0,                       CommandSpacing::Block     }},
  { "author",          { 0,                       CommandSpacing::Block     }},
  { "authors",         { 0,                       CommandSpacing::Block     }},
  { "brief",           { &handleBrief,            CommandSpacing::Invisible }},
  { "bug",             { &handleBug,              CommandSpacing::XRef      }},
  { "callergraph",     { &handleCallergraph,      CommandSpacing::Invisible }},
  { "callgraph",       { &handleCallgraph,        CommandSpacing::Invisible }},
  { "category",        { &handleCategory,         CommandSpacing::Invisible }},
  { "cite",            { &handleCite,             CommandSpacing::Inline    }},
  { "class",           { &handleClass,            CommandSpacing::Invisible }},
  { "code",            { &handleFormatBlock,      CommandSpacing::Block     }},
  { "concept",         { &handleConcept,          CommandSpacing::Invisible }},
  { "copybrief",       { &handleCopyBrief,        CommandSpacing::Invisible }},
  { "copydetails",     { &handleCopyDetails,      CommandSpacing::Block     }},
  { "copydoc",         { &handleCopyDoc,          CommandSpacing::Block     }},
  { "copyright",       { 0,                       CommandSpacing::Block     }},
  { "date",            { 0,                       CommandSpacing::Block     }},
  { "def",             { &handleDef,              CommandSpacing::Invisible }},
  { "defgroup",        { &handleDefGroup,         CommandSpacing::Invisible }},
  { "deprecated",      { &handleDeprecated,       CommandSpacing::XRef      }},
  { "details",         { &handleDetails,          CommandSpacing::Block     }},
  { "dir",             { &handleDir,              CommandSpacing::Invisible }},
  { "docbookinclude",  { 0,                       CommandSpacing::Inline    }},
  { "docbookonly",     { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "dot",             { &handleFormatBlock,      CommandSpacing::Block     }},
  { "dotfile",         { 0,                       CommandSpacing::Block     }},
  { "else",            { &handleElse,             CommandSpacing::Inline    }},
  { "elseif",          { &handleElseIf,           CommandSpacing::Inline    }},
  { "endif",           { &handleEndIf,            CommandSpacing::Inline    }},
  { "endparblock",     { &handleEndParBlock,      CommandSpacing::Block     }},
  { "enum",            { &handleEnum,             CommandSpacing::Invisible }},
  { "example",         { &handleExample,          CommandSpacing::Invisible }},
  { "exception",       { 0,                       CommandSpacing::Block     }},
  { "extends",         { &handleExtends,          CommandSpacing::Invisible }},
  { "file",            { &handleFile,             CommandSpacing::Invisible }},
  { "fn",              { &handleFn,               CommandSpacing::Invisible }},
  { "headerfile",      { &handleHeaderFile,       CommandSpacing::Invisible }},
  { "hidecallergraph", { &handleHideCallergraph,  CommandSpacing::Invisible }},
  { "hidecallgraph",   { &handleHideCallgraph,    CommandSpacing::Invisible }},
  { "hideinitializer", { &handleHideInitializer,  CommandSpacing::Invisible }},
  { "hiderefby",       { &handleHideReferencedByRelation, CommandSpacing::Invisible }},
  { "hiderefs",        { &handleHideReferencesRelation,   CommandSpacing::Invisible }},
  { "htmlinclude",     { 0,                       CommandSpacing::Inline    }},
  { "htmlonly",        { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "idlexcept",       { &handleIdlException,     CommandSpacing::Invisible }},
  { "if",              { &handleIf,               CommandSpacing::Inline    }},
  { "ifnot",           { &handleIfNot,            CommandSpacing::Inline    }},
  { "image",           { &handleImage,            CommandSpacing::Block     }},
  { "implements",      { &handleExtends,          CommandSpacing::Invisible }},
  { "include",         { 0,                       CommandSpacing::Block     }},
  { "includelineno",   { 0,                       CommandSpacing::Block     }},
  { "ingroup",         { &handleIngroup,          CommandSpacing::Invisible }},
  { "inherit",         { &handleInherit,          CommandSpacing::Invisible }},
  { "interface",       { &handleInterface,        CommandSpacing::Invisible }},
  { "internal",        { &handleInternal,         CommandSpacing::Block     }},
  { "invariant",       { 0,                       CommandSpacing::Block     }},
  { "latexinclude",    { 0,                       CommandSpacing::Inline    }},
  { "latexonly",       { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "li",              { 0,                       CommandSpacing::Block     }},
  { "line",            { 0,                       CommandSpacing::Invisible }},
  { "mainpage",        { &handleMainpage,         CommandSpacing::Invisible }},
  { "maninclude",      { 0,                       CommandSpacing::Inline    }},
  { "manonly",         { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "memberof",        { &handleMemberOf,         CommandSpacing::Invisible }},
  { "msc",             { &handleFormatBlock,      CommandSpacing::Block     }},
  { "name",            { &handleName,             CommandSpacing::Invisible }},
  { "namespace",       { &handleNamespace,        CommandSpacing::Invisible }},
  { "noop",            { &handleNoop,             CommandSpacing::Invisible }},
  { "nosubgrouping",   { &handleNoSubGrouping,    CommandSpacing::Invisible }},
  { "note",            { 0,                       CommandSpacing::Block     }},
  { "overload",        { &handleOverload,         CommandSpacing::Invisible }},
  { "package",         { &handlePackage,          CommandSpacing::Invisible }},
  { "page",            { &handlePage,             CommandSpacing::Invisible }},
  { "par",             { 0,                       CommandSpacing::Block     }},
  { "paragraph",       { &handleSection,          CommandSpacing::Block     }},
  { "param",           { &handleParam,            CommandSpacing::Block     }},
  { "parblock",        { &handleParBlock,         CommandSpacing::Block     }},
  { "post",            { 0,                       CommandSpacing::Block     }},
  { "pre",             { 0,                       CommandSpacing::Block     }},
  { "private",         { &handlePrivate,          CommandSpacing::Invisible }},
  { "privatesection",  { &handlePrivateSection,   CommandSpacing::Invisible }},
  { "property",        { &handleFn,               CommandSpacing::Invisible }},
  { "protected",       { &handleProtected,        CommandSpacing::Invisible }},
  { "protectedsection",{ &handleProtectedSection, CommandSpacing::Invisible }},
  { "protocol",        { &handleProtocol,         CommandSpacing::Invisible }},
  { "public",          { &handlePublic,           CommandSpacing::Invisible }},
  { "publicsection",   { &handlePublicSection,    CommandSpacing::Invisible }},
  { "pure",            { &handlePure,             CommandSpacing::Invisible }},
  { "raisewarning",    { &handleRaiseWarning,     CommandSpacing::Invisible }},
  { "refitem",         { &handleRefItem,          CommandSpacing::Inline    }},
  { "related",         { &handleRelated,          CommandSpacing::Invisible }},
  { "relatedalso",     { &handleRelatedAlso,      CommandSpacing::Invisible }},
  { "relates",         { &handleRelated,          CommandSpacing::Invisible }},
  { "relatesalso",     { &handleRelatedAlso,      CommandSpacing::Invisible }},
  { "remark",          { 0,                       CommandSpacing::Block     }},
  { "remarks",         { 0,                       CommandSpacing::Block     }},
  { "result",          { 0,                       CommandSpacing::Block     }},
  { "return",          { 0,                       CommandSpacing::Block     }},
  { "returns",         { 0,                       CommandSpacing::Block     }},
  { "retval",          { &handleRetval,           CommandSpacing::Block     }},
  { "rtfinclude",      { 0,                       CommandSpacing::Inline    }},
  { "rtfonly",         { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "sa",              { 0,                       CommandSpacing::Block     }},
  { "section",         { &handleSection,          CommandSpacing::Block     }},
  { "see",             { 0,                       CommandSpacing::Block     }},
  { "short",           { &handleBrief,            CommandSpacing::Invisible }},
  { "showinitializer", { &handleShowInitializer,  CommandSpacing::Invisible }},
  { "showrefby",       { &handleReferencedByRelation,     CommandSpacing::Invisible }},
  { "showrefs",        { &handleReferencesRelation,       CommandSpacing::Invisible }},
  { "since",           { 0,                       CommandSpacing::Block     }},
  { "snippet",         { 0,                       CommandSpacing::Block     }},
  { "snippetlineno",   { 0,                       CommandSpacing::Block     }},
  { "startuml",        { &handleFormatBlock,      CommandSpacing::Block     }},
  { "static",          { &handleStatic,           CommandSpacing::Invisible }},
  { "struct",          { &handleStruct,           CommandSpacing::Invisible }},
  { "subpage",         { &handleSubpage,          CommandSpacing::Inline    }},
  { "subsection",      { &handleSection,          CommandSpacing::Block     }},
  { "subsubsection",   { &handleSection,          CommandSpacing::Block     }},
  { "tableofcontents", { &handleToc,              CommandSpacing::Invisible }},
  { "test",            { &handleTest,             CommandSpacing::XRef      }},
  { "throw",           { 0,                       CommandSpacing::Block     }},
  { "throws",          { 0,                       CommandSpacing::Block     }},
  { "todo",            { &handleTodo,             CommandSpacing::XRef      }},
  { "tparam",          { 0,                       CommandSpacing::Block     }},
  { "typedef",         { &handleFn,               CommandSpacing::Invisible }},
  { "union",           { &handleUnion,            CommandSpacing::Invisible }},
  { "until",           { 0,                       CommandSpacing::Block     }},
  { "var",             { &handleFn,               CommandSpacing::Invisible }},
  { "verbatim",        { &handleFormatBlock,      CommandSpacing::Block     }},
  { "verbinclude",     { 0,                       CommandSpacing::Inline    }},
  { "version",         { 0,                       CommandSpacing::Block     }},
  { "warning",         { 0,                       CommandSpacing::Block     }},
  { "weakgroup",       { &handleWeakGroup,        CommandSpacing::Invisible }},
  { "xmlinclude",      { 0,                       CommandSpacing::Inline    }},
  { "xmlonly",         { &handleFormatBlock,      CommandSpacing::Invisible }},
  { "xrefitem",        { &handleXRefItem,         CommandSpacing::XRef      }},
  { "iliteral",        { &handleFormatBlock,      CommandSpacing::Inline    }},
};

#define YY_NO_INPUT 1
#define YY_NO_UNISTD_H 1
#define YY_NEVER_INTERACTIVE 1


enum XRefKind
{
  XRef_Item,
  XRef_Todo,
  XRef_Test,
  XRef_Bug,
  XRef_Deprecated,
  XRef_None
};

enum OutputContext
{
  OutputDoc,
  OutputBrief,
  OutputXRef,
  OutputInbody
};

enum GuardType
{
  Guard_If,
  Guard_IfNot,
  Guard_Skip
};

class GuardedSection
{
  public:
    GuardedSection(bool enabled,bool parentVisible)
      : m_enabled(enabled),m_parentVisible(parentVisible) {}
    bool isEnabled() const { return m_enabled; }
    bool parentVisible() const { return m_parentVisible; }

  private:
    bool m_enabled;
    bool m_parentVisible;
};

/* -----------------------------------------------------------------
 *
 *        statics
 */

struct commentscanYY_state
{
  OutlineParserInterface *langParser = 0;  // the language parser that is calling us
  QCString         inputString;            // input string
  QCString         currentCmd;             // the command used
  int              inputPosition = 0;      // read pointer
  QCString         fileName;               // file name that is read from
  int              lineNr = 0;             // line number in the input
  bool             inBody = FALSE;         // was the comment found inside the body of a function?
  OutputContext    inContext;              // are we inside the brief, details or xref part
  bool             briefEndsAtDot = FALSE; // does the brief description stop at a dot?
  QCString         formulaText;            // Running text of a formula
  QCString         formulaEnv;             // environment name
  int              formulaNewLines = 0;    // amount of new lines in the formula
  QCString        *pOutputString = 0;      // pointer to string to which the output is appended.
  QCString         outputXRef;               // temp argument of todo/test/../xrefitem commands
  QCString         blockName;                // preformatted block name (e.g. verbatim, latexonly,...)
  XRefKind         xrefKind    = XRef_Item;  // kind of cross-reference command
  XRefKind         newXRefKind = XRef_Item;  //
  GuardType        guardType = Guard_If;     // kind of guards for conditional section
  bool             enabledSectionFound = FALSE;
  QCString         functionProto;          // function prototype
  std::stack<GuardedSection> guards;             // tracks nested conditional sections (if,ifnot,..)
  Entry           *current = 0;              // working entry

  bool             needNewEntry = FALSE;

  QCString         sectionLabel;
  QCString         sectionTitle;
  int              sectionLevel = 0;
  QCString         xrefItemKey;
  QCString         newXRefItemKey;
  QCString         xrefItemTitle;
  QCString         xrefListTitle;
  Protection       protection = Public;

  bool             xrefAppendFlag = FALSE;
  bool             inGroupParamFound = FALSE;
  int              braceCount = 0;
  bool             insidePre = FALSE;
  bool             parseMore = FALSE;
  int              condCount = 0;

  int              commentCount = 0;
  QCString         spaceBeforeCmd;
  QCString         spaceBeforeIf;
  QCString         copyDocArg;

  QCString         guardExpr;
  int              roundCount = 0;
  int              HTMLDetails = 0;

  bool             insideParBlock = FALSE;
  bool             inInternalDocs = FALSE;
  int              prevPosition = 0;
  DocGroup         docGroup;
  bool             markdownSupport = TRUE;

  QCString         raiseWarning;

  QCString         htmlAnchorStr;
  bool             htmlAnchor = false;
};


static std::mutex g_sectionMutex;
static std::mutex g_formulaMutex;
static std::mutex g_citeMutex;

//-----------------------------------------------------------------------------

static QCString stripQuotes(const char *s);
static bool getDocSectionName(int s);
static SectionType sectionLevelToType(int level);
static void stripTrailingWhiteSpace(QCString &s);

static void initParser(yyscan_t yyscanner);
static bool makeStructuralIndicator(yyscan_t yyscanner,Entry::Sections s);
static void lineCount(yyscan_t yyscanner);
static void addXRefItem(yyscan_t yyscanner,
                        const QCString &listName,const QCString &itemTitle,
                        const QCString &listTitle,bool append);
static QCString addFormula(yyscan_t yyscanner);
static void checkFormula(yyscan_t yyscanner);
static void addSection(yyscan_t yyscanner);
static inline void setOutput(yyscan_t yyscanner,OutputContext ctx);
static void addAnchor(yyscan_t yyscanner,const QCString &anchor);
static inline void addOutput(yyscan_t yyscanner,const char *s);
static inline void addOutput(yyscan_t yyscanner,const QCString &s);
static inline void addOutput(yyscan_t yyscanner,char c);
static void endBrief(yyscan_t yyscanner,bool addToOutput=TRUE);
static void handleGuard(yyscan_t yyscanner,const QCString &expr);
static yy_size_t yyread(yyscan_t yyscanner,char *buf,yy_size_t max_size);
static void addCite(yyscan_t yyscanner);
static void addIline(yyscan_t yyscanner,int lineNr);

#define unput_string(yytext,yyleng) do { for (int i=(int)yyleng-1;i>=0;i--) unput(yytext[i]); } while(0)

//-----------------------------------------------------------------------------

#undef        YY_INPUT
#define        YY_INPUT(buf,result,max_size) result=yyread(yyscanner,buf,max_size);

// otherwise the filename would be the name of the converted file (*.cpp instead of *.l)
static inline const char *getLexerFILE() {return __FILE__;}
#include "doxygen_lex.h"

%}

       /* start command character */
CMD       ("\\"|"@")
XREFCMD   {CMD}("bug"|"deprecated"|"test"|"todo"|"xrefitem")
PRE       [pP][rR][eE]
TABLE     [tT][aA][bB][lL][eE]
P         [pP]
UL        [uU][lL]
OL        [oO][lL]
DL        [dD][lL]
IMG       [iI][mM][gG]
HR        [hH][rR]
PARA      [pP][aA][rR][aA]
CODE      [cC][oO][dD][eE]
CAPTION   [cC][aA][pP][tT][iI][oO][nN]
CENTER    [cC][eE][nN][tT][eE][rR]
DIV       [dD][iI][vV]
DETAILS   [dD][eE][tT][aA][iI][lL][sS]
DETAILEDHTML {CENTER}|{DIV}|{PRE}|{UL}|{TABLE}|{OL}|{DL}|{P}|[Hh][1-6]|{IMG}|{HR}|{PARA}
DETAILEDHTMLOPT {CODE}
SUMMARY   [sS][uU][mM][mM][aA][rR][yY]
REMARKS   [rR][eE][mM][aA][rR][kK][sS]
AHTML     [aA]{BN}*
ANCHTML   ([iI][dD]|[nN][aA][mM][eE])"="("\""{LABELID}"\""|"'"{LABELID}"'"|{LABELID})
BN        [ \t\n\r]
BL        [ \t\r]*"\n"
B         [ \t]
Bopt      {B}*
BS        ^(({B}*"/""/")?)(({B}*"*"+)?){B}*
ATTR      ({B}+[^>\n]*)?
DOCNL     "\n"|"\\ilinebr"
LC        "\\"{B}*"\n"
NW        [^a-z_A-Z0-9]
FILESCHAR [a-z_A-Z0-9\x80-\xFF\\:\\\/\-\+=@&#]
FILEECHAR [a-z_A-Z0-9\x80-\xFF\-\+=@&#]
FILE      ({FILESCHAR}*{FILEECHAR}+("."{FILESCHAR}*{FILEECHAR}+)*)|("\""[^\n\"]*"\"")
ID        [$a-z_A-Z\x80-\xFF][$a-z_A-Z0-9\x80-\xFF]*
LABELID   [a-z_A-Z\x80-\xFF][a-z_A-Z0-9\x80-\xFF\-]*
CITESCHAR [a-z_A-Z0-9\x80-\xFF\-\?]
CITEECHAR [a-z_A-Z0-9\x80-\xFF\-\+:\/\?]*
CITEID    {CITESCHAR}{CITEECHAR}*("."{CITESCHAR}{CITEECHAR}*)*|"\""{CITESCHAR}{CITEECHAR}*("."{CITESCHAR}{CITEECHAR}*)*"\""
SCOPEID   {ID}({ID}*{BN}*"::"{BN}*)*({ID}?)
SCOPENAME "$"?(({ID}?{BN}*("::"|"."){BN}*)*)((~{BN}*)?{ID})
TMPLSPEC  "<"{BN}*[^>]+{BN}*">"
MAILADDR  ("mailto:")?[a-z_A-Z0-9\x80-\xff.+-]+"@"[a-z_A-Z0-9\x80-\xff-]+("."[a-z_A-Z0-9\x80-\xff\-]+)+[a-z_A-Z0-9\x80-\xff\-]+
RCSTAG    "$"{ID}":"[^\n$]+"$"

  // C start comment 
CCS   "/\*"
  // C end comment
CCE   "*\/"
  // Cpp comment 
CPPC  "/\/"

  // end of section title with asterisk
STAopt [^\n@\\*]*
  // end of section title without asterisk
STopt  [^\n@\\]*

%option noyywrap

  /* comment parsing states. */
%x      Comment
%x      PageDocArg1
%x      PageDocArg2
%x      RelatesParam1
%x      ClassDocArg1
%x      ClassDocArg2
%x      ClassDocArg3
%x      CategoryDocArg1
%x      XRefItemParam1
%x      XRefItemParam2
%x      XRefItemParam3
%x      FileDocArg1
%x      ParamArg1
%x      EnumDocArg1
%x      NameSpaceDocArg1
%x      PackageDocArg1
%x      ConceptDocArg1
%x      GroupDocArg1
%x      GroupDocArg2
%x      SectionLabel
%x      SectionTitle
%x      SubpageLabel
%x      SubpageTitle
%x      FormatBlock
%x      LineParam
%x      GuardParam
%x      GuardParamEnd
%x      SkipGuardedSection
%x      SkipInternal
%x      NameParam
%x      InGroupParam
%x      FnParam
%x      OverloadParam
%x      InheritParam
%x      ExtendsParam
%x      ReadFormulaShort
%x      ReadFormulaRound
%x      ReadFormulaLong
%x      AnchorLabel
%x      HtmlComment
%x      HtmlA
%x      SkipLang
%x      CiteLabel
%x      CopyDoc
%x      GuardExpr
%x      CdataSection
%x      Noop
%x      RaiseWarning

%%

  /* What can happen in while parsing a comment block:
   *   commands (e.g. @page, or \page)
   *   escaped commands (e.g. @@page or \\page).
   *   formulas (e.g. \f$...\f$ \f[...\f] \f{...\f} \f(...\f) )
   *   directories (e.g. \doxygen\src\)
   *   autolist end. (e.g. a dot on an otherwise empty line)
   *   newlines.
   *   end of brief description due to blank line.
   *   end of brief description due to some command (@command, or <command>).
   *   words and whitespace and other characters (#,?!, etc).
   *   grouping commands (e.g. @{ and @})
   *   language switch (e.g. \~english or \~).
   *   mail address (e.g. doxygen@gmail.com).
   *   quoted text, such as "foo@bar"
   *   XML commands, <summary></summary><remarks></remarks>
   */

<Comment>{CMD}{CMD}[a-z_A-Z]+{B}*       { // escaped command
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{CMD}{CMD}"~"[a-z_A-Z]*        { // escaped command
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{MAILADDR}                     { // mail address
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"\""[^"\n]*"\""                { // quoted text
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>("\\"[a-z_A-Z]+)+"\\"          { // directory (or chain of commands!)
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"<"{DETAILEDHTML}{ATTR}">"     { // HTML command that ends a brief description
                                          setOutput(yyscanner,OutputDoc);
                                          // continue with the same input
                                          REJECT;
                                        }
<Comment>"<"{DETAILEDHTMLOPT}{ATTR}">"  { // HTML command that ends a brief description
                                          if (yyextra->current->lang==SrcLangExt_CSharp)
                                          {
                                            setOutput(yyscanner,OutputDoc);
                                          }
                                          // continue with the same input
                                          REJECT;
                                        }
<Comment>"<"{DETAILS}{ATTR}">"          { // start of a HTML style details description
                                          yyextra->HTMLDetails++;
                                          setOutput(yyscanner,OutputDoc);
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"</"{DETAILS}">"               { // end of a HTML style details description
                                          if (yyextra->HTMLDetails) yyextra->HTMLDetails--;
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"<"{AHTML}                     { // potential start of HTML anchor, see issue 9200
                                          yyextra->htmlAnchorStr = yytext;
                                          yyextra->htmlAnchor = false;
                                          BEGIN(HtmlA);
                                        }
<HtmlA>{ANCHTML}                        { // only labels that can be converted to doxygen anchor
                                          yyextra->htmlAnchorStr += yytext;
                                          QCString tag(yytext);
                                          int s=tag.find("=");
                                          char c=tag[s+1];
                                          QCString id;
                                          if (c=='\'' || c=='"') // valid start
                                          {
                                            int e=tag.find(c,s+2);
                                            if (e!=-1) // found matching end
                                            {
                                              id=tag.mid(s+2,e-s-2); // extract id
                                              addAnchor(yyscanner,id);
                                            }
                                          }
                                          else
                                          {
                                            id=tag.mid(s+1);
                                            addAnchor(yyscanner,id);
                                          }
                                          if (!id.isEmpty() && !yyextra->htmlAnchor)
                                          {
                                            // only use first analogous to what is in docparser
                                            addOutput(yyscanner,"@anchor ");
                                            addOutput(yyscanner,id.data());
                                            addOutput(yyscanner," ");
                                            yyextra->htmlAnchor = true;
                                          }
                                        }
<HtmlA>("\""[^\n\"]*"\""|"'"[^\n']*"'") {
                                          yyextra->htmlAnchorStr += yytext;
                                        }
<HtmlA>">"|"/>"                         {
                                          if (!yyextra->htmlAnchor)
                                          {
                                            addOutput(yyscanner,yyextra->htmlAnchorStr);
                                            addOutput(yyscanner,yytext);
                                          }
                                          else
                                          {
                                            if (yyleng == 1) // to keep <a></a> pairs, otherwise single </a> present
                                            {
                                              addOutput(yyscanner,"<a>");
                                            }
                                          }
                                          BEGIN(Comment);
                                        }
<HtmlA>{DOCNL}                          { // newline
                                          yyextra->htmlAnchorStr += yytext;
                                          if (*yytext == '\n') yyextra->lineNr++;
                                        }
<HtmlA>.                                { // catch-all for anything else
                                          yyextra->htmlAnchorStr += yytext;
                                        }
<Comment>"<"{SUMMARY}">"                { // start of a .NET XML style brief description
                                          if (!yyextra->HTMLDetails) setOutput(yyscanner,OutputBrief);
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"<"{REMARKS}">"                { // start of a .NET XML style detailed description
                                          setOutput(yyscanner,OutputDoc);
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"</"{SUMMARY}">"               { // start of a .NET XML style detailed description
                                          addOutput(yyscanner,yytext);
                                          if (!yyextra->HTMLDetails) setOutput(yyscanner,OutputDoc);
                                        }
<Comment>"</"{REMARKS}">"               { // end of a brief or detailed description
                                          setOutput(yyscanner,OutputDoc);
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"<"{CAPTION}{ATTR}">"          {
                                          QCString tag(yytext);
                                          int s=tag.find("id=");
                                          if (s!=-1) // command has id attribute
                                          {
                                            char c=tag[s+3];
                                            if (c=='\'' || c=='"') // valid start
                                            {
                                              int e=tag.find(c,s+4);
                                              if (e!=-1) // found matching end
                                              {
                                                QCString id=tag.mid(s+4,e-s-4); // extract id
                                                addAnchor(yyscanner,id);
                                              }
                                            }
                                          }
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"<"{PRE}{ATTR}">"              {
                                          yyextra->insidePre=TRUE;
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"</"{PRE}">"                   {
                                          yyextra->insidePre=FALSE;
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{RCSTAG}                       { // RCS tag which end a brief description
                                          setOutput(yyscanner,OutputDoc);
                                          REJECT;
                                        }
<Comment>"<!--"                         {
                                          BEGIN(HtmlComment);
                                        }
<Comment>"<!\[CDATA\["                  {
                                          BEGIN(CdataSection);
                                        }
<Comment>{B}*{CMD}"endinternal"{B}*     {
                                          addOutput(yyscanner," \\endinternal ");
                                          if (!yyextra->inInternalDocs)
                                              warn(yyextra->fileName,yyextra->lineNr,
                                               "found \\endinternal without matching \\internal"
                                              );
                                          yyextra->inInternalDocs = FALSE;
                                        }
<Comment>{B}*"\\ilinebr"{B}*            { // preserve spacing around \\ilinebr
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{B}*{CMD}[a-z_A-Z]+"{"[a-zA-Z_,:0-9\. ]*"}"{B}*  |
<Comment>{B}*{CMD}[a-z_A-Z]+{B}*        { // potentially interesting command
                                          // the {B}* in the front was added for bug620924
                                          QCString fullMatch = QCString(yytext);
                                          int idx = fullMatch.find('{');
                                          /* handle `\f{` and `@f{` as special cases */
                                          if ((idx > 1) && (yytext[idx-1] == 'f') && (yytext[idx-2] == '\\' || yytext[idx-2] =='@')) REJECT;
                                          int idxEnd = fullMatch.find("}",idx+1);
                                          QCString cmdName;
                                          StringVector optList;
                                          if (idx == -1) // no options
                                          {
                                            cmdName = QCString(yytext).stripWhiteSpace().mid(1); // to remove {CMD}
                                          }
                                          else // options present
                                          {
                                            cmdName = fullMatch.left(idx).stripWhiteSpace().mid(1); // to remove {CMD}
                                            QCString optStr = fullMatch.mid(idx+1,idxEnd-idx-1).stripWhiteSpace();
                                            optList = split(optStr.str(),",");
                                          }
                                          auto it = docCmdMap.find(cmdName.str());
                                          //printf("lookup command '%s' found=%d\n",qPrint(cmdName),it!=docCmdMap.end());
                                          if (it!=docCmdMap.end()) // special action is required
                                          {
                                            int i=0;
                                            while (yytext[i]==' ' || yytext[i]=='\t') i++;
                                            yyextra->spaceBeforeCmd = QCString(yytext).left(i);
                                            CommandSpacing spacing = it->second.spacing;
                                            if ((spacing==CommandSpacing::Block || spacing==CommandSpacing::XRef) &&
                                                !(yyextra->inContext==OutputXRef && cmdName=="parblock"))
                                            {
                                              yyextra->briefEndsAtDot=FALSE;
                                              bool insideXRef = yyextra->inContext==OutputXRef && spacing==CommandSpacing::XRef;
                                              // this command forces the end of brief description
                                              setOutput(yyscanner,insideXRef ? OutputXRef : OutputDoc);
                                            }
                                            //if (i>0) addOutput(yyscanner,QCString(yytext).left(i)); // removed for bug 689341
                                            if (it->second.handler && it->second.handler(yyscanner, cmdName, optList))
                                            {
                                              // implicit split of the comment block into two
                                              // entries. Restart the next block at the start
                                              // of this command.
                                              yyextra->parseMore=TRUE;

                                              // yuk, this is probably not very portable across lex implementations,
                                              // but we need to know the position in the input buffer where this
                                              // rule matched.
                                              // for flex 2.5.33+ we should use YY_CURRENT_BUFFER_LVALUE
#if YY_FLEX_MAJOR_VERSION>=2 && (YY_FLEX_MINOR_VERSION>5 || (YY_FLEX_MINOR_VERSION==5 && YY_FLEX_SUBMINOR_VERSION>=33))
                                              yyextra->inputPosition=yyextra->prevPosition + (int)(yy_bp - YY_CURRENT_BUFFER_LVALUE->yy_ch_buf);
#else
                                              yyextra->inputPosition=yyextra->prevPosition + (int)(yy_bp - yy_current_buffer->yy_ch_buf);
#endif
                                              yyterminate();
                                            }
                                            else if (it->second.handler==0)
                                            {
                                              // command without handler, to be processed
                                              // later by parsedoc.cpp
                                              addOutput(yyscanner,yytext);
                                            }
                                          }
                                          else // command not relevant
                                          {
                                            addOutput(yyscanner,yytext);
                                          }
                                        }
<Comment>{B}*({CMD}{CMD})"f"[$\[{]      { // escaped formula command
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{B}*{CMD}"~"[a-z_A-Z-]*        { // language switch command
                                          QCString langId = QCString(yytext).stripWhiteSpace().mid(2);
                                          if (!langId.isEmpty() &&
                                              qstricmp(Config_getEnumAsString(OUTPUT_LANGUAGE),langId)!=0)
                                          { // enable language specific section
                                            if (!Config_isAvailableEnum(OUTPUT_LANGUAGE,langId))
                                            {
                                               warn(yyextra->fileName,yyextra->lineNr,
                                               "non supported language '%s' specified in '%s'",langId.data(),QCString(yytext).stripWhiteSpace().data());
                                            }
                                            BEGIN(SkipLang);
                                          }
                                        }
<Comment>{B}*{CMD}"f{"[^}\n]+"}"("{"?)  { // start of a formula with custom environment
                                          setOutput(yyscanner,OutputDoc);
                                          yyextra->formulaText="\\begin";
                                          yyextra->formulaEnv=QCString(yytext).stripWhiteSpace().mid(2);
                                          if (yyextra->formulaEnv.at(yyextra->formulaEnv.length()-1)=='{')
                                          {
                                            // remove trailing open brace
                                            yyextra->formulaEnv=yyextra->formulaEnv.left(yyextra->formulaEnv.length()-1);
                                          }
                                          yyextra->formulaText+=yyextra->formulaEnv;
                                          yyextra->formulaNewLines=0;
                                          BEGIN(ReadFormulaLong);
                                        }
<Comment>{B}*{CMD}"f$"                  { // start of a inline formula
                                          yyextra->formulaText="$";
                                          yyextra->formulaNewLines=0;
                                          BEGIN(ReadFormulaShort);
                                        }
<Comment>{B}*{CMD}"f("                  { // start of a inline formula
                                          yyextra->formulaText="";
                                          yyextra->formulaNewLines=0;
                                          BEGIN(ReadFormulaRound);
                                        }
<Comment>{B}*{CMD}"f["                  { // start of a block formula
                                          setOutput(yyscanner,OutputDoc);
                                          yyextra->formulaText="\\[";
                                          yyextra->formulaNewLines=0;
                                          BEGIN(ReadFormulaLong);
                                        }
<Comment>{B}*{CMD}"{"                   { // begin of a group
                                          //yyextra->langParser->handleGroupStartCommand(yyextra->memberGroupHeader);
                                          yyextra->docGroup.open(yyextra->current,yyextra->fileName,yyextra->lineNr);
                                        }
<Comment>{B}*{CMD}"}"                   { // end of a group
                                          //yyextra->langParser->handleGroupEndCommand();
                                          yyextra->docGroup.close(yyextra->current,yyextra->fileName,yyextra->lineNr,TRUE);
                                          yyextra->docGroup.clearHeader();
                                          yyextra->parseMore=TRUE;
                                          yyextra->needNewEntry = TRUE;
#if YY_FLEX_MAJOR_VERSION>=2 && (YY_FLEX_MINOR_VERSION>5 || (YY_FLEX_MINOR_VERSION==5 && YY_FLEX_SUBMINOR_VERSION>=33))
                                          yyextra->inputPosition=yyextra->prevPosition + (int)(yy_bp - YY_CURRENT_BUFFER_LVALUE->yy_ch_buf) + (int)strlen(yytext);
#else
                                          yyextra->inputPosition=yyextra->prevPosition + (int)(yy_bp - yy_current_buffer->yy_ch_buf) + (int)strlen(yytext);
#endif
                                          yyterminate();
                                        }
<Comment>{B}*{CMD}[$@\\&~<>#%]          { // escaped character
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>[a-z_A-Z]+                     { // normal word
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>^{B}*"."{Bopt}/\n                { // explicit end autolist: e.g "  ."
                                            addOutput(yyscanner,yytext);
                                        }
<Comment>^{B}*[1-9][0-9]*"."{B}+        |
<Comment>^{B}*[*+]{B}+                  { // start of autolist
                                          if (!yyextra->markdownSupport)
                                          {
                                            REJECT;
                                          }
                                          else
                                          {
                                            if (yyextra->inContext!=OutputXRef)
                                            {
                                              yyextra->briefEndsAtDot=FALSE;
                                              setOutput(yyscanner,OutputDoc);
                                            }
                                            addOutput(yyscanner,yytext);
                                          }
                                          }
<Comment>^{B}*"-"{B}+                        { // start of autolist
                                          if (yyextra->inContext!=OutputXRef)
                                          {
                                            yyextra->briefEndsAtDot=FALSE;
                                            setOutput(yyscanner,OutputDoc);
                                          }
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>^{B}*([\-:|]{B}*)*("--"|"---")({B}*[\-:|])*{Bopt}/\n { // horizontal line (dashed)
                                            addOutput(yyscanner,yytext);
                                        }
<Comment>{CMD}"---"                     { // escaped mdash
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>{CMD}"--"                      { // escaped mdash
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"---"                          { // mdash
                                          addOutput(yyscanner,yyextra->insidePre || yyextra->markdownSupport ? yytext : "&mdash;");
                                        }
<Comment>"--"                           { // ndash
                                          addOutput(yyscanner,yyextra->insidePre || yyextra->markdownSupport ? yytext : "&ndash;");
                                        }
<Comment>"-#"{B}+                       { // numbered item
                                          if (yyextra->inContext!=OutputXRef)
                                          {
                                            yyextra->briefEndsAtDot=FALSE;
                                            setOutput(yyscanner,OutputDoc);
                                          }
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>("."+)[a-z_A-Z0-9\)]           { // . at start or in the middle of a word, or ellipsis
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>".\\"[ \t]                     { // . with escaped space.
                                          addOutput(yyscanner,yytext[0]);
                                          addOutput(yyscanner,yytext[2]);
                                        }
<Comment>"."[,:;]                       { // . with some puntuations such as "e.g.," or "e.g.:"
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>"...\\"[ \t]                   { // ellipsis with escaped space.
                                          addOutput(yyscanner,"... ");
                                        }
<Comment>"..."/[^\.]                    { // ellipsis
                                          addOutput(yyscanner,"...");
                                        }
<Comment>".."[\.]?/[^ \t\n]             { // internal ellipsis
                                          addOutput(yyscanner,yytext);
                                        }
<Comment>(\n|\\ilinebr)({B}*(\n|\\ilinebr))+ { // at least one blank line (or blank line command)
                                          if (yyextra->inContext==OutputXRef)
                                          {
                                            // see bug 613024, we need to put the newlines after ending the XRef section.
                                            if (!yyextra->insideParBlock) setOutput(yyscanner,OutputDoc);
                                            yy_size_t i;
                                            for (i=0;i<(yy_size_t)yyleng;)
                                            {
                                              if (yytext[i]=='\n') addOutput(yyscanner,'\n'),i++;
                                              else if (strcmp(yytext+i,"\\ilinebr")==0) addOutput(yyscanner,"\\ilinebr"),i+=8;
                                              else i++;
                                            }
                                          }
                                          else if (yyextra->inContext!=OutputBrief)
                                          {
                                            yy_size_t i;
                                            for (i=0;i<(yy_size_t)yyleng;)
                                            {
                                              if (yytext[i]=='\n') addOutput(yyscanner,'\n'),i++;
                                              else if (strcmp(yytext+i,"\\ilinebr")==0) addOutput(yyscanner,"\\ilinebr"),i+=8;
                                              else i++;
                                            }
                                            setOutput(yyscanner,OutputDoc);
                                          }
                                          else // yyextra->inContext==OutputBrief
                                          { // only go to the detailed description if we have
                                            // found some brief description and not just whitespace
                                            endBrief(yyscanner,TRUE);
                                          }
                                          lineCount(yyscanner);
                                        }
<Comment>"."                            { // potential end of a JavaDoc style comment
                                          addOutput(yyscanner,*yytext);
                                          if (yyextra->briefEndsAtDot)
                                          {
                                            setOutput(yyscanner,OutputDoc);
                                            yyextra->briefEndsAtDot=FALSE;
                                          }
                                        }
<Comment>{DOCNL}                             { // newline
                                          addOutput(yyscanner,yytext);
                                          if (*yytext == '\n') yyextra->lineNr++;
                                        }
<Comment>.                              { // catch-all for anything else
                                          addOutput(yyscanner,*yytext);
                                        }


 /* --------------   Rules for handling HTML comments ----------- */

<HtmlComment>"--"[!]?">"{B}*            { BEGIN( Comment ); }
<HtmlComment>{DOCNL}                    {
                                          if (*yytext=='\n')
                                          {
                                            addOutput(yyscanner,*yytext);
                                            yyextra->lineNr++;
                                          }
                                        }
<HtmlComment>[^\\\n\-]+                 { // ignore unimportant characters
                                        }
<HtmlComment>.                          { // ignore every else
                                        }

<CdataSection>"\]\]>"                   {
                                          BEGIN( Comment );
                                        }
<CdataSection>{DOCNL}                   {
                                          addOutput(yyscanner,'\n');
                                          if (*yytext=='\n') yyextra->lineNr++;
                                        }
<CdataSection>[<>&]                     { // the special XML characters for iwhich the CDATA section is especially used
                                          addOutput(yyscanner,'\\');
                                          addOutput(yyscanner,*yytext);
                                        }
<CdataSection>[^\\\n\]<>&]+             {
                                          addOutput(yyscanner,yytext);
                                        }
<CdataSection>.                         {
                                          addOutput(yyscanner,*yytext);
                                        }

 /* --------------   Rules for handling formulas ---------------- */

<ReadFormulaShort>{CMD}"f$"             { // end of inline formula
                                          yyextra->formulaText+="$";
                                          addOutput(yyscanner," "+addFormula(yyscanner));
                                          BEGIN(Comment);
                                        }
<ReadFormulaRound>{CMD}"f)"             { // end of inline formula
                                          addOutput(yyscanner," "+addFormula(yyscanner));
                                          BEGIN(Comment);
                                        }
<ReadFormulaLong>{CMD}"f]"              { // end of block formula
                                          yyextra->formulaText+="\\]";
                                          addOutput(yyscanner," "+addFormula(yyscanner));
                                          BEGIN(Comment);
                                        }
<ReadFormulaLong>{CMD}"f}"              { // end of custom env formula
                                          yyextra->formulaText+="\\end";
                                          yyextra->formulaText+=yyextra->formulaEnv;
                                          addOutput(yyscanner," "+addFormula(yyscanner));
                                          BEGIN(Comment);
                                        }
<ReadFormulaLong,ReadFormulaShort,ReadFormulaRound>[^\\@\n]+ { // any non-special character
                                          yyextra->formulaText+=yytext;
                                        }
<ReadFormulaLong,ReadFormulaShort,ReadFormulaRound>\n    { // new line
                                          yyextra->formulaNewLines++;
                                          yyextra->formulaText+=*yytext;
                                          yyextra->lineNr++;
                                        }
<ReadFormulaLong,ReadFormulaShort,ReadFormulaRound>.     { // any other character
                                          yyextra->formulaText+=*yytext;
                                        }

  /* ------------ handle argument of enum command --------------- */

<EnumDocArg1>{SCOPEID}                  { // handle argument
                                          yyextra->current->name = yytext;
                                          BEGIN( Comment );
                                        }
<EnumDocArg1>{LC}                       { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<EnumDocArg1>{DOCNL}                    { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after \\enum."
                                              );
                                          unput_string(yytext,yyleng);
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          BEGIN( Comment );
                                        }
<EnumDocArg1>.                          { // ignore other stuff
                                        }

  /* ------------ handle argument of namespace command --------------- */

<NameSpaceDocArg1>{SCOPENAME}           { // handle argument
                                          lineCount(yyscanner);
                                          yyextra->current->name = substitute(removeRedundantWhiteSpace(QCString(yytext)),QCString("."),QCString("::"));
                                          BEGIN( Comment );
                                        }
<NameSpaceDocArg1>{LC}                  { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<NameSpaceDocArg1>{DOCNL}               { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after "
                                               "\\namespace."
                                              );
                                          unput_string(yytext,yyleng);
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          BEGIN( Comment );
                                        }
<NameSpaceDocArg1>.                     { // ignore other stuff
                                        }

  /* ------------ handle argument of package command --------------- */

<PackageDocArg1>{ID}("."{ID})*          { // handle argument
                                          yyextra->current->name = yytext;
                                          BEGIN( Comment );
                                        }
<PackageDocArg1>{LC}                    { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<PackageDocArg1>{DOCNL}                 { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after "
                                               "\\package."
                                              );
                                          unput_string(yytext,yyleng);
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          BEGIN( Comment );
                                        }
<PackageDocArg1>.                       { // ignore other stuff
                                        }

  /* ------------ handle argument of concept command --------------- */

<ConceptDocArg1>{SCOPEID}               { // handle argument
                                          yyextra->current->name = yytext;
                                          BEGIN( Comment );
                                        }
<ConceptDocArg1>{LC}                    { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<ConceptDocArg1>{DOCNL}                 { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after "
                                               "\\concept."
                                              );
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<ConceptDocArg1>.                       { // ignore other stuff
                                        }


  /* ------ handle argument of class/struct/union command --------------- */

<ClassDocArg1>{SCOPENAME}{TMPLSPEC}     {
                                          lineCount(yyscanner);
                                          yyextra->current->name = substitute(removeRedundantWhiteSpace(QCString(yytext)),".","::");
                                          BEGIN( ClassDocArg2 );
                                        }
<ClassDocArg1>{SCOPENAME}               { // first argument
                                          lineCount(yyscanner);
                                          yyextra->current->name = substitute(QCString(yytext),".","::");
                                          if (yyextra->current->section==Entry::PROTOCOLDOC_SEC)
                                          {
                                            yyextra->current->name+="-p";
                                          }
                                          // prepend outer scope name
                                          BEGIN( ClassDocArg2 );
                                        }
<CategoryDocArg1>{SCOPENAME}{B}*"("[^\)]+")" {
                                          lineCount(yyscanner);
                                          yyextra->current->name = substitute(QCString(yytext),".","::");
                                          BEGIN( ClassDocArg2 );
                                        }
<ClassDocArg1,CategoryDocArg1>{LC}      { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<ClassDocArg1,CategoryDocArg1>{DOCNL}   {
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after "
                                               "'\\%s'.",qPrint(yyextra->currentCmd)
                                              );
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<ClassDocArg1,CategoryDocArg1>.         { // ignore other stuff
                                        }

<ClassDocArg2>{DOCNL}                   {
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<ClassDocArg2>{FILE}|"<>"               { // second argument; include file
                                          yyextra->current->includeFile = yytext;
                                          BEGIN( ClassDocArg3 );
                                        }
<ClassDocArg2>{LC}                      { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<ClassDocArg2>.                         { // ignore other stuff
                                        }

<ClassDocArg3>[<"]?{FILE}?[">]?         { // third argument; include file name
                                          yyextra->current->includeName = yytext;
                                          BEGIN( Comment );
                                        }
<ClassDocArg3>{LC}                      { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<ClassDocArg3>{DOCNL}                   {
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<ClassDocArg3>.                         { // ignore other stuff
                                        }

  /* --------- handle arguments of {def,add,weak}group commands --------- */

<GroupDocArg1>{LABELID}(".html"?)       { // group name
                                           yyextra->current->name = yytext;
                                          //lastDefGroup.groupname = yytext;
                                          //lastDefGroup.pri = yyextra->current->groupingPri();
                                          // the .html stuff is for Qt compatibility
                                          if (yyextra->current->name.right(5)==".html")
                                          {
                                            yyextra->current->name=yyextra->current->name.left(yyextra->current->name.length()-5);
                                          }
                                          yyextra->current->type.resize(0);
                                          BEGIN(GroupDocArg2);
                                        }
<GroupDocArg1>"\\"{B}*"\n"              { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<GroupDocArg1>{DOCNL}                   { // missing argument!
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "missing group name after %s",
                                               yyextra->current->groupDocCmd()
                                              );
                                          //addOutput(yyscanner,'\n');
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<GroupDocArg1>.                         { // ignore other stuff
                                        }
<GroupDocArg2>"\\"{B}*"\n"              { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<GroupDocArg2>[^\n\\]+                  { // title (stored in type)
                                          yyextra->current->type += yytext;
                                          yyextra->current->type = yyextra->current->type.stripWhiteSpace();
                                        }
<GroupDocArg2>{DOCNL}                   {
                                          if ( yyextra->current->groupDocType==Entry::GROUPDOC_NORMAL &&
                                               yyextra->current->type.isEmpty()
                                             ) // defgroup requires second argument
                                          {
                                              warn(yyextra->fileName,yyextra->lineNr,
                                                 "missing title after "
                                                 "\\defgroup %s", qPrint(yyextra->current->name)
                                                );
                                          }
                                          unput_string(yytext,yyleng);
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<GroupDocArg2>.                         { // title (stored in type)
                                          yyextra->current->type += yytext;
                                          yyextra->current->type = yyextra->current->type.stripWhiteSpace();
                                        }

  /* --------- handle arguments of page/mainpage command ------------------- */

<PageDocArg1>{FILE}                     { // first argument; page name
                                          yyextra->current->name = stripQuotes(yytext);
                                          yyextra->current->args = "";
                                          BEGIN( PageDocArg2 );
                                        }
<PageDocArg1>{LC}                       { yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<PageDocArg1>{DOCNL}                    {
                                            warn(yyextra->fileName,yyextra->lineNr,
                                               "missing argument after "
                                               "\\page."
                                              );
                                          unput_string(yytext,yyleng);
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<PageDocArg1>.                          { // ignore other stuff
                                        }
<PageDocArg2>{DOCNL}                    { // second argument; page title
                                          unput_string(yytext,yyleng);
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<PageDocArg2>{CMD}[<>]                  {
                                          // bug 748927
                                          QCString tmp(yytext);
                                          tmp = substitute(substitute(tmp,"@<","&lt;"),"@>","&gt;");
                                          tmp = substitute(substitute(tmp,"\\<","&lt;"),"\\>","&gt;");
                                          yyextra->current->args += tmp;
                                        }
<PageDocArg2>.                          {
                                          yyextra->current->args += yytext;
                                        }
  /* --------- handle arguments of the param command ------------ */
<ParamArg1>{ID}/{B}*","                 {
                                          addOutput(yyscanner,yytext);
                                        }
<ParamArg1>","                          {
                                          addOutput(yyscanner," , ");
                                        }
<ParamArg1>{DOCNL}                      {
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner," ");
                                        }
<ParamArg1>{ID}                         {
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<ParamArg1>.                            {
                                          unput(yytext[0]);
                                          BEGIN( Comment );
                                        }

  /* --------- handle arguments of the file/dir/example command ------------ */

<FileDocArg1>{DOCNL}                    { // no file name specified
                                          unput_string(yytext,yyleng);
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<FileDocArg1>{FILE}                     { // first argument; name
                                          yyextra->current->name = stripQuotes(yytext);
                                          BEGIN( Comment );
                                        }
<FileDocArg1>{LC}                       { yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<FileDocArg1>.                          { // ignore other stuff
                                        }

  /* --------- handle arguments of the xrefitem command ------------ */

<XRefItemParam1>{LABELID}               { // first argument
                                          yyextra->newXRefItemKey=yytext;
                                          setOutput(yyscanner,OutputXRef);
                                          BEGIN(XRefItemParam2);
                                        }
<XRefItemParam1>{LC}                    { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<XRefItemParam1>{DOCNL}                 { // missing arguments
                                          warn(yyextra->fileName,yyextra->lineNr,
                                               "Missing first argument of \\xrefitem"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          yyextra->inContext = OutputDoc;
                                          BEGIN( Comment );
                                        }
<XRefItemParam1>.                       { // ignore other stuff
                                        }

<XRefItemParam2>"\""[^\n\"]*"\""        { // second argument
                                          yyextra->xrefItemTitle = stripQuotes(yytext);
                                          BEGIN(XRefItemParam3);
                                        }
<XRefItemParam2>{LC}                    { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<XRefItemParam2>{DOCNL}                 { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Missing second argument of \\xrefitem"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          yyextra->inContext = OutputDoc;
                                          BEGIN( Comment );
                                        }
<XRefItemParam2>.                       { // ignore other stuff
                                        }

<XRefItemParam3>"\""[^\n\"]*"\""        { // third argument
                                          yyextra->xrefListTitle = stripQuotes(yytext);
                                          yyextra->xrefKind = XRef_Item;
                                          BEGIN( Comment );
                                        }
<XRefItemParam2,XRefItemParam3>{LC}     { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<XRefItemParam3>{DOCNL}                 { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Missing third argument of \\xrefitem"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          yyextra->inContext = OutputDoc;
                                          BEGIN( Comment );
                                        }
<XRefItemParam3>.                       { // ignore other stuff
                                        }


  /* ----- handle arguments of the relates(also)/memberof command ------- */

<RelatesParam1>({ID}("::"|"."))*{ID}    { // argument
                                          yyextra->current->relates = yytext;
                                          //if (yyextra->current->mGrpId!=DOX_NOGROUP)
                                          //{
                                          //  memberGroupRelates = yytext;
                                          //}
                                          BEGIN( Comment );
                                        }
<RelatesParam1>{LC}                     { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<RelatesParam1>{DOCNL}                  { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Missing argument of '\\%s' command",qPrint(yyextra->currentCmd)
                                              );
                                          unput_string(yytext,yyleng);
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<RelatesParam1>.                        { // ignore other stuff
                                        }


  /* ----- handle arguments of the relates(also)/addindex commands ----- */

<LineParam>{DOCNL}                      { // end of argument
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<LineParam>{LC}                         { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<LineParam>.                            { // ignore other stuff
                                          addOutput(yyscanner,*yytext);
                                        }

  /* ----- handle arguments of the section/subsection/.. commands ------- */

<SectionLabel>{LABELID}                 { // first argument
                                          yyextra->sectionLabel=yytext;
                                          addOutput(yyscanner,yytext);
                                          yyextra->sectionTitle.resize(0);
                                          BEGIN(SectionTitle);
                                        }
<SectionLabel>{DOCNL}                   { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "\\section command has no label"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<SectionLabel>.                         { // invalid character for section label
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Invalid or missing section label"
                                              );
                                          BEGIN(Comment);
                                        }
<SectionTitle>{STAopt}/"\n"             { // end of section title
                                          addSection(yyscanner);
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<SectionTitle>{STopt}/"\\ilinebr"       { // end of section title
                                          addSection(yyscanner);
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<SectionTitle>{LC}                      { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<SectionTitle>[^\n@\\]*                 { // any character without special meaning
                                          yyextra->sectionTitle+=yytext;
                                          addOutput(yyscanner,yytext);
                                        }
<SectionTitle>({CMD}{CMD}){ID}          { // unescape escaped command
                                          yyextra->sectionTitle+=&yytext[1];
                                          addOutput(yyscanner,yytext);
                                        }
<SectionTitle>{CMD}[$@\\&~<>#%]         { // unescape escaped character
                                          yyextra->sectionTitle+=yytext[1];
                                          addOutput(yyscanner,yytext);
                                        }
<SectionTitle>.                         { // anything else
                                          yyextra->sectionTitle+=yytext;
                                          addOutput(yyscanner,*yytext);
                                        }

  /* ----- handle arguments of the subpage command ------- */

<SubpageLabel>{LABELID}                 { // first argument
                                          addOutput(yyscanner,yytext);
                                          // we add subpage labels as a kind of "inheritance" relation to prevent
                                          // needing to add another list to the Entry class.
                                          yyextra->current->extends.push_back(BaseInfo(QCString(yytext),Public,Normal));
                                          BEGIN(SubpageTitle);
                                        }
<SubpageLabel>{DOCNL}                   { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "\\subpage command has no label"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<SubpageLabel>.                         {
                                          unput(yytext[0]);
                                          BEGIN( Comment );
                                        }
<SubpageTitle>{DOCNL}                   { // no title, end command
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<SubpageTitle>[ \t]*"\""[^\"\n]*"\""    { // add title, end of command
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<SubpageTitle>.                         { // no title, end of command
                                          unput(*yytext);
                                          BEGIN( Comment );
                                        }

  /* ----- handle arguments of the anchor command ------- */

<AnchorLabel>{LABELID}                  { // found argument
                                          addAnchor(yyscanner,QCString(yytext));
                                          addOutput(yyscanner,yytext);
                                          BEGIN( Comment );
                                        }
<AnchorLabel>{DOCNL}                    { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "\\anchor command has no label"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<AnchorLabel>.                          { // invalid character for anchor label
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Invalid or missing anchor label"
                                              );
                                          BEGIN(Comment);
                                        }


  /* ----- handle arguments of the preformatted block commands ------- */

<FormatBlock>{CMD}("endverbatim"|"endiliteral"|"endlatexonly"|"endhtmlonly"|"endxmlonly"|"enddocbookonly"|"endrtfonly"|"endmanonly"|"enddot"|"endcode"|"endmsc")/{NW} { // possible ends
                                          addOutput(yyscanner,yytext);
                                          if (&yytext[4]==yyextra->blockName) // found end of the block
                                          {
                                              BEGIN(Comment);
                                          }
                                        }
<FormatBlock>{CMD}"enduml"              {
                                          addOutput(yyscanner,yytext);
                                          if (yyextra->blockName=="startuml") // found end of the block
                                          {
                                              BEGIN(Comment);
                                          }
                                        }
<FormatBlock>[^ \@\*\/\\\n]*            { // some word
                                          addOutput(yyscanner,yytext);
                                        }
<FormatBlock>{DOCNL}                    { // new line
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<FormatBlock>{CCS}                       { // start of a C-comment
                                          if (!(yyextra->blockName=="code" || yyextra->blockName=="verbatim" || yyextra->blockName=="iliteral")) yyextra->commentCount++;
                                          addOutput(yyscanner,yytext);
                                        }
<FormatBlock>{CCE}                       { // end of a C-comment
                                          addOutput(yyscanner,yytext);
                                          if (!(yyextra->blockName=="code" || yyextra->blockName=="verbatim" || yyextra->blockName=="iliteral"))
                                          {
                                            yyextra->commentCount--;
                                            if (yyextra->commentCount<0)
                                            {
                                              QCString endTag = "end"+yyextra->blockName;
                                              if (yyextra->blockName=="startuml") endTag="enduml";
                                              warn(yyextra->fileName,yyextra->lineNr,
                                                 "found */ without matching /* while inside a \\%s block! Perhaps a missing \\%s?\n",qPrint(yyextra->blockName),qPrint(endTag));
                                            }
                                          }
                                        }
<FormatBlock>.                          {
                                          addOutput(yyscanner,*yytext);
                                        }
<FormatBlock><<EOF>>                    {
                                          QCString endTag = "end"+yyextra->blockName;
                                          if (yyextra->blockName=="startuml") endTag="enduml";
                                          warn(yyextra->fileName,yyextra->lineNr,
                                            "reached end of comment while inside a \\%s block; check for missing \\%s tag!",
                                            qPrint(yyextra->blockName),qPrint(endTag)
                                          );
                                          yyterminate();
                                        }

  /* ----- handle arguments of if/ifnot commands ------- */

<GuardParam>{B}*"("                     {
                                          yyextra->guardExpr=yytext;
                                          yyextra->roundCount=1;
                                          BEGIN(GuardExpr);
                                        }
<GuardExpr>[^()]*                       {
                                          yyextra->guardExpr+=yytext;
                                          lineCount(yyscanner);
                                        }
<GuardExpr>"("                          {
                                          yyextra->guardExpr+=yytext;
                                          yyextra->roundCount++;
                                        }
<GuardExpr>")"                          {
                                          yyextra->guardExpr+=yytext;
                                          yyextra->roundCount--;
                                          if (yyextra->roundCount==0)
                                          {
                                            handleGuard(yyscanner,yyextra->guardExpr);
                                          }
                                        }
<GuardExpr>\n                           {
                                          warn(yyextra->fileName,yyextra->lineNr,
                                                "invalid expression '%s' for yyextra->guards",qPrint(yyextra->guardExpr));
                                          unput(*yytext);
                                          BEGIN(GuardParam);
                                        }
<GuardParam>{B}*[a-z_A-Z0-9.\-]+        { // parameter of if/ifnot yyextra->guards
                                          handleGuard(yyscanner,QCString(yytext));
                                        }
<GuardParam>{DOCNL}                     { // end of argument
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          //next line is commented out due to bug620924
                                          //addOutput(yyscanner,'\n');
                                          addIline(yyscanner,yyextra->lineNr);
                                          BEGIN( Comment );
                                        }
<GuardParam>{LC}                        { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<GuardParam>.                           { // ignore other stuff
                                          addOutput(yyscanner,*yytext);
                                        }
<GuardParamEnd>{B}*{DOCNL}              {
                                          lineCount(yyscanner);
                                          yyextra->spaceBeforeIf.resize(0);
                                          addIline(yyscanner,yyextra->lineNr);
                                          BEGIN(Comment);
                                        }
<GuardParamEnd>{B}*                     {
                                          if (!yyextra->spaceBeforeIf.isEmpty()) // needed for 665313 in combination with bug620924
                                          {
                                            addOutput(yyscanner,yyextra->spaceBeforeIf);
                                          }
                                          yyextra->spaceBeforeIf.resize(0);
                                          addIline(yyscanner,yyextra->lineNr);
                                          BEGIN(Comment);
                                        }
<GuardParamEnd>.                        {
                                          unput(*yytext);
                                          addIline(yyscanner,yyextra->lineNr);
                                          BEGIN(Comment);
                                        }

  /* ----- handle skipping of conditional sections ------- */

<SkipGuardedSection>{CMD}"ifnot"/{NW}   {
                                          yyextra->guardType = Guard_IfNot;
                                          BEGIN( GuardParam );
                                        }
<SkipGuardedSection>{CMD}"if"/{NW}      {
                                          yyextra->guardType = Guard_If;
                                          BEGIN( GuardParam );
                                        }
<SkipGuardedSection>{CMD}"endif"/{NW}   {
                                          if (yyextra->guards.empty())
                                          {
                                            warn(yyextra->fileName,yyextra->lineNr,
                                                "found \\endif without matching start command");
                                          }
                                          else
                                          {
                                            GuardedSection s = yyextra->guards.top();
                                            yyextra->guards.pop();
                                            bool parentVisible = s.parentVisible();
                                            if (parentVisible)
                                            {
                                              yyextra->enabledSectionFound=TRUE;
                                              BEGIN( GuardParamEnd );
                                            }
                                          }
                                        }
<SkipGuardedSection>{CMD}"else"/{NW}    {
                                          if (yyextra->guards.empty())
                                          {
                                            warn(yyextra->fileName,yyextra->lineNr,
                                                "found \\else without matching start command");
                                          }
                                          else
                                          {
                                            if (!yyextra->enabledSectionFound && yyextra->guards.top().parentVisible())
                                            {
                                              yyextra->guards.pop();
                                              yyextra->guards.push(GuardedSection(TRUE,TRUE));
                                              yyextra->enabledSectionFound=TRUE;
                                              BEGIN( GuardParamEnd );
                                            }
                                          }
                                        }
<SkipGuardedSection>{CMD}"elseif"/{NW}  {
                                          if (yyextra->guards.empty())
                                          {
                                            warn(yyextra->fileName,yyextra->lineNr,
                                                "found \\elseif without matching start command");
                                          }
                                          else
                                          {
                                            if (!yyextra->enabledSectionFound && yyextra->guards.top().parentVisible())
                                            {
                                              yyextra->guardType=Guard_If;
                                              yyextra->guards.pop();
                                              BEGIN( GuardParam );
                                            }
                                          }
                                        }
<SkipGuardedSection>{DOCNL}             { // skip line
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                        }
<SkipGuardedSection>[^ \\@\n]+          { // skip non-special characters
                                        }
<SkipGuardedSection>.                   { // any other character
                                        }


  /* ----- handle skipping of internal section ------- */

<SkipInternal>{DOCNL}                   { // skip line
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<SkipInternal>[@\\]"if"/[ \t]           {
                                          yyextra->condCount++;
                                          }
<SkipInternal>[@\\]"ifnot"/[ \t]        {
                                          yyextra->condCount++;
                                        }
<SkipInternal>[@\\]/"endif"             {
                                          yyextra->condCount--;
                                          if (yyextra->condCount<0) // handle conditional section around of \internal, see bug607743
                                          {
                                            unput('\\');
                                            BEGIN(Comment);
                                          }
                                        }
<SkipInternal>[@\\]/"section"[ \t]      {
                                          if (yyextra->sectionLevel>0)
                                          {
                                            unput('\\');
                                            BEGIN(Comment);
                                          }
                                        }
<SkipInternal>[@\\]/"subsection"[ \t]   {
                                          if (yyextra->sectionLevel>1)
                                          {
                                            unput('\\');
                                            BEGIN(Comment);
                                          }
                                        }
<SkipInternal>[@\\]/"subsubsection"[ \t] {
                                          if (yyextra->sectionLevel>2)
                                          {
                                            unput('\\');
                                            BEGIN(Comment);
                                          }
                                        }
<SkipInternal>[@\\]/"paragraph"[ \t]    {
                                          if (yyextra->sectionLevel>3)
                                          {
                                            unput('\\');
                                            BEGIN(Comment);
                                          }
                                        }
<SkipInternal>[@\\]"endinternal"[ \t]*  {
                                          BEGIN(Comment);
                                        }
<SkipInternal>[^ \\@\n]+                { // skip non-special characters
                                        }
<SkipInternal>.                         { // any other character
                                        }


  /* ----- handle argument of name command ------- */

<NameParam>{DOCNL}                      { // end of argument
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<NameParam>{LC}                         { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          yyextra->docGroup.appendHeader(' ');
                                        }
<NameParam>.                            { // ignore other stuff
                                          yyextra->docGroup.appendHeader(*yytext);
                                          yyextra->current->name+=*yytext;
                                        }

  /* ----- handle argument of noop command ------- */
<Noop>{DOCNL}                           { // end of argument
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<Noop>.                                 { // ignore other stuff
                                        }
  /* ----- handle argument of raisewarning command ------- */
<RaiseWarning>{DOCNL}                   { // end of argument
                                          warn_doc_error(yyextra->fileName,yyextra->lineNr,
                                                         "%s",qPrint(yyextra->raiseWarning));
                                          yyextra->raiseWarning = "";
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<RaiseWarning>.                         { // ignore other stuff
                                          yyextra->raiseWarning += yytext;
                                        }
  /* ----- handle argument of ingroup command ------- */

<InGroupParam>{LABELID}                 { // group id
                                          yyextra->current->groups.push_back(
                                             Grouping(QCString(yytext), Grouping::GROUPING_INGROUP)
                                          );
                                          yyextra->inGroupParamFound=TRUE;
                                        }
<InGroupParam>{DOCNL}                   { // missing argument
                                          if (!yyextra->inGroupParamFound)
                                          {
                                             warn(yyextra->fileName,yyextra->lineNr,
                                                "Missing group name for \\ingroup command"
                                                );
                                          }
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<InGroupParam>{LC}                      { // line continuation
                                          yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                        }
<InGroupParam>.                         { // ignore other stuff
                                          addOutput(yyscanner,*yytext);
                                        }

  /* ----- handle argument of fn command ------- */

<FnParam>{DOCNL}                        { // end of argument
                                          if (yyextra->braceCount==0)
                                          {
                                            //if (*yytext=='\n') yyextra->lineNr++;
                                            //addOutput(yyscanner,'\n');
                                            unput_string(yytext,yyleng);
                                            yyextra->langParser->parsePrototype(yyextra->functionProto);
                                            BEGIN( Comment );
                                          }
                                        }
<FnParam>{LC}                           { // line continuation
                                          yyextra->lineNr++;
                                          yyextra->functionProto+=' ';
                                        }
<FnParam>[^@\\\n()]+                    { // non-special characters
                                          yyextra->functionProto+=yytext;
                                        }
<FnParam>"("                            {
                                          yyextra->functionProto+=yytext;
                                          yyextra->braceCount++;
                                        }
<FnParam>")"                            {
                                          yyextra->functionProto+=yytext;
                                          yyextra->braceCount--;
                                        }
<FnParam>.                              { // add other stuff
                                          yyextra->functionProto+=*yytext;
                                        }


  /* ----- handle argument of overload command ------- */


<OverloadParam>{DOCNL}                  { // end of argument
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          if (yyextra->functionProto.stripWhiteSpace().isEmpty())
                                          { // plain overload command
                                            addOutput(yyscanner,getOverloadDocs());
                                            addOutput(yyscanner,'\n');
                                          }
                                          else // overload declaration
                                          {
                                            makeStructuralIndicator(yyscanner,Entry::OVERLOADDOC_SEC);
                                            yyextra->langParser->parsePrototype(yyextra->functionProto);
                                          }
                                          BEGIN( Comment );
                                        }
<OverloadParam>{LC}                     { // line continuation
                                          yyextra->lineNr++;
                                          yyextra->functionProto+=' ';
                                        }
<OverloadParam>.                        { // add other stuff
                                          yyextra->functionProto+=*yytext;
                                        }

  /* ----- handle argument of inherit command ------- */

<InheritParam>({ID}("::"|"."))*{ID}     { // found argument
                                          yyextra->current->extends.push_back(
                                            BaseInfo(removeRedundantWhiteSpace(QCString(yytext)),Public,Normal)
                                          );
                                          BEGIN( Comment );
                                        }
<InheritParam>{DOCNL}                   { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "\\inherit command has no argument"
                                              );
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          addOutput(yyscanner,'\n');
                                          BEGIN( Comment );
                                        }
<InheritParam>.                         { // invalid character for anchor label
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "Invalid or missing name for \\inherit command"
                                              );
                                          BEGIN(Comment);
                                        }

  /* ----- handle argument of extends and implements commands ------- */

<ExtendsParam>({ID}("::"|"."))*{ID}     { // found argument
                                          yyextra->current->extends.push_back(
                                            BaseInfo(removeRedundantWhiteSpace(QCString(yytext)),Public,Normal)
                                          );
                                          BEGIN( Comment );
                                        }
<ExtendsParam>{DOCNL}                   { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "'\\%s' command has no argument",qPrint(yyextra->currentCmd)
                                              );
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<ExtendsParam>.                         { // ignore other stuff
                                        }

  /* ----- handle language specific sections ------- */

<SkipLang>[\\@]"~"[a-zA-Z-]*            { /* language switch */
                                          QCString langId(&yytext[2]);
                                          if (!langId.isEmpty() && !Config_isAvailableEnum(OUTPUT_LANGUAGE,langId))
                                          {
                                             warn(yyextra->fileName,yyextra->lineNr,
                                             "non supported language '%s' specified in '%s'",langId.data(),QCString(yytext).stripWhiteSpace().data());
                                          }
                                          else if (langId.isEmpty() ||
                                              qstricmp(Config_getEnumAsString(OUTPUT_LANGUAGE),langId)==0)
                                          { // enable language specific section
                                            BEGIN(Comment);
                                          }
                                        }
<SkipLang>[^*@\\\n]*                    { /* any character not a *, @, backslash or new line */
                                        }
<SkipLang>{DOCNL}                       { /* new line in verbatim block */
                                          if (*yytext=='\n') yyextra->lineNr++;
                                        }
<SkipLang>.                             { /* any other character */
                                        }

  /* ----- handle arguments of the cite command ------- */

<CiteLabel>{CITEID}                     { // found argument
                                          addCite(yyscanner);
                                          addOutput(yyscanner,yytext);
                                          BEGIN(Comment);
                                        }
<CiteLabel>{DOCNL}                      { // missing argument
                                          warn(yyextra->fileName,yyextra->lineNr,
                                              "\\cite command has no label"
                                              );
                                          //if (*yytext=='\n') yyextra->lineNr++;
                                          //addOutput(yyscanner,'\n');
                                          unput_string(yytext,yyleng);
                                          BEGIN( Comment );
                                        }
<CiteLabel>.                            { // invalid character for cite label
                                           warn(yyextra->fileName,yyextra->lineNr,
                                              "Invalid or missing cite label"
                                              );
                                          BEGIN(Comment);
                                        }

  /* ----- handle argument of the copydoc command ------- */

<CopyDoc><<EOF>>                        {
                                          setOutput(yyscanner,OutputDoc);
                                          addOutput(yyscanner," \\ilinebr\\ilinebr\\copydetails ");
                                          addOutput(yyscanner,yyextra->copyDocArg);
                                          addOutput(yyscanner,"\n");
                                          BEGIN(Comment);
                                        }
<CopyDoc>{DOCNL}                        {
                                          if (*yytext=='\n') yyextra->lineNr++;
                                          if (yyextra->braceCount==0)
                                          {
                                            setOutput(yyscanner,OutputDoc);
                                            addOutput(yyscanner," \\ilinebr\\ilinebr\\copydetails ");
                                            addOutput(yyscanner,yyextra->copyDocArg);
                                            addOutput(yyscanner,"\n");
                                            BEGIN(Comment);
                                          }
                                        }
<CopyDoc>{LC}                           { // line continuation
                                          yyextra->lineNr++;
                                        }
<CopyDoc>[^@\\\n()]+                    { // non-special characters
                                          yyextra->copyDocArg+=yytext;
                                          addOutput(yyscanner,yytext);
                                        }
<CopyDoc>"("                            {
                                          yyextra->copyDocArg+=yytext;
                                          addOutput(yyscanner,yytext);
                                          yyextra->braceCount++;
                                        }
<CopyDoc>")"                            {
                                          yyextra->copyDocArg+=yytext;
                                          addOutput(yyscanner,yytext);
                                          yyextra->braceCount--;
                                        }
<CopyDoc>.                              {
                                          yyextra->copyDocArg+=yytext;
                                          addOutput(yyscanner,yytext);
                                        }

 /*
<*>.  { fprintf(stderr,"Lex scanner %s %sdefault rule for state %s: #%s#\n", __FILE__,!yyextra->fileName.isEmpty() ? ("(" + yyextra->fileName +") ").data(): "",stateToString(YY_START),yytext);}
<*>\n  { fprintf(stderr,"Lex scanner %s %sdefault rule newline for state %s.\n", __FILE__, !yyextra->fileName.isEmpty() ? ("(" + yyextra->fileName +") ").data(): "",stateToString(YY_START));}
 */

%%

//----------------------------------------------------------------------------

static bool handleBrief(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  //printf("handleBrief\n");
  setOutput(yyscanner,OutputBrief);
  return FALSE;
}

static bool handleFn(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::MEMBERDOC_SEC);
  yyextra->functionProto.resize(0);
  yyextra->braceCount=0;
  BEGIN(FnParam);
  return stop;
}

static bool handleDef(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::DEFINEDOC_SEC);
  yyextra->functionProto.resize(0);
  BEGIN(FnParam);
  return stop;
}

static bool handleOverload(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->functionProto.resize(0);
  BEGIN(OverloadParam);
  return FALSE;
}

static bool handleEnum(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::ENUMDOC_SEC);
  BEGIN(EnumDocArg1);
  return stop;
}

static bool handleDefGroup(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::GROUPDOC_SEC);
  yyextra->current->groupDocType = Entry::GROUPDOC_NORMAL;
  BEGIN( GroupDocArg1 );
  return stop;
}

static bool handleAddToGroup(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::GROUPDOC_SEC);
  yyextra->current->groupDocType = Entry::GROUPDOC_ADD;
  BEGIN( GroupDocArg1 );
  return stop;
}

static bool handleWeakGroup(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::GROUPDOC_SEC);
  yyextra->current->groupDocType = Entry::GROUPDOC_WEAK;
  BEGIN( GroupDocArg1 );
  return stop;
}

static bool handleNamespace(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::NAMESPACEDOC_SEC);
  BEGIN( NameSpaceDocArg1 );
  return stop;
}

static bool handlePackage(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::PACKAGEDOC_SEC);
  BEGIN( PackageDocArg1 );
  return stop;
}

static bool handleClass(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::CLASSDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handleConcept(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::CONCEPTDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ConceptDocArg1 );
  return stop;
}

static bool handleHeaderFile(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  BEGIN( ClassDocArg2 );
  return FALSE;
}

static bool handleProtocol(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{ // Obj-C protocol
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::PROTOCOLDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handleCategory(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{ // Obj-C category
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::CATEGORYDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( CategoryDocArg1 );
  return stop;
}

static bool handleUnion(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::UNIONDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handleStruct(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::STRUCTDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handleInterface(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::INTERFACEDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handleIdlException(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::EXCEPTIONDOC_SEC);
  yyextra->currentCmd = cmd;
  BEGIN( ClassDocArg1 );
  return stop;
}

static bool handlePage(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::PAGEDOC_SEC);
  BEGIN( PageDocArg1 );
  return stop;
}

static bool handleMainpage(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::MAINPAGEDOC_SEC);
  if (!stop)
  {
    yyextra->current->name = "mainpage";
  }
  setOutput(yyscanner,OutputDoc);
  BEGIN( PageDocArg2 );
  return stop;
}

static bool handleFile(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::FILEDOC_SEC);
  if (!stop)
  {
    yyextra->current->name = yyextra->fileName;
  }
  BEGIN( FileDocArg1 );
  return stop;
}

static bool handleParam(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  // we need process param and retval arguments to escape leading underscores in case of
  // markdown processing, see bug775493
  addOutput(yyscanner,"@param ");
  BEGIN( ParamArg1 );
  return FALSE;
}

static bool handleRetval(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  addOutput(yyscanner,"@retval ");
  BEGIN( ParamArg1 );
  return FALSE;
}

static bool handleDir(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::DIRDOC_SEC);
  if (!stop) yyextra->current->name = yyextra->fileName;
  BEGIN( FileDocArg1 );
  return stop;
}

static bool handleExample(yyscan_t yyscanner,const QCString &cmd, const StringVector &optList)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  Entry::Sections section=Entry::EXAMPLE_SEC;
  for (const auto &opt : optList)
  {
    if (opt=="lineno")
    {
      section=Entry::EXAMPLE_LINENO_SEC;
    }
    else
    {
      warn(yyextra->fileName,yyextra->lineNr,
          "unsupported option '%s' for command '\\%s'",opt.c_str(),qPrint(cmd));
    }
  }
  bool stop=makeStructuralIndicator(yyscanner,section);
  if (!stop) yyextra->current->name = yyextra->fileName;
  BEGIN( FileDocArg1 );
  return stop;
}

static bool handleDetails(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->inContext!=OutputBrief)
  {
    addOutput(yyscanner,"\\ilinebr\\ilinebr "); // treat @details outside brief description
                                                // as a new paragraph
  }
  setOutput(yyscanner,OutputDoc);
  return FALSE;
}

static bool handleRaiseWarning(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->raiseWarning = "";
  BEGIN( RaiseWarning );
  return FALSE;
}

static bool handleNoop(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  BEGIN( Noop );
  return FALSE;
}

static bool handleName(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool stop=makeStructuralIndicator(yyscanner,Entry::MEMBERGRP_SEC);
  if (!stop)
  {
    yyextra->docGroup.clearHeader();
    BEGIN( NameParam );
    if (!yyextra->docGroup.isEmpty()) // end of previous member group
    {
      yyextra->docGroup.close(yyextra->current,yyextra->fileName,yyextra->lineNr,TRUE,true);
    }
  }
  return stop;
}

static bool handleTodo(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->newXRefKind = XRef_Todo;
  setOutput(yyscanner,OutputXRef);
  yyextra->xrefKind = XRef_Todo;
  return FALSE;
}

static bool handleTest(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->newXRefKind = XRef_Test;
  setOutput(yyscanner,OutputXRef);
  yyextra->xrefKind = XRef_Test;
  return FALSE;
}

static bool handleBug(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->newXRefKind = XRef_Bug;
  setOutput(yyscanner,OutputXRef);
  yyextra->xrefKind = XRef_Bug;
  return FALSE;
}

static bool handleDeprecated(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->newXRefKind = XRef_Deprecated;
  setOutput(yyscanner,OutputXRef);
  yyextra->xrefKind = XRef_Deprecated;
  return FALSE;
}

static bool handleXRefItem(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->newXRefKind = XRef_Item;
  BEGIN(XRefItemParam1);
  return FALSE;
}

static bool handleParBlock(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->insideParBlock)
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\parblock command while already in a parblock!");
  }
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"@parblock ");
  yyextra->insideParBlock = TRUE;
  return FALSE;
}

static bool handleEndParBlock(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->insideParBlock)
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\endparblock command without matching \\parblock!");
  }
  addOutput(yyscanner,"@endparblock");
  setOutput(yyscanner,OutputDoc); // to end a parblock inside a xrefitem like context
  yyextra->insideParBlock = FALSE;
  return FALSE;
}

static bool handleRelated(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->current->relates.isEmpty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found multiple \\relates, \\relatesalso or \\memberof commands in a comment block, using last definition");
  }
  yyextra->current->relatesType = Simple;
  yyextra->currentCmd = cmd;
  BEGIN(RelatesParam1);
  return FALSE;
}

static bool handleRelatedAlso(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->current->relates.isEmpty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found multiple \\relates, \\relatesalso or \\memberof commands in a comment block, using last definition");
  }
  yyextra->current->relatesType = Duplicate;
  yyextra->currentCmd = cmd;
  BEGIN(RelatesParam1);
  return FALSE;
}

static bool handleMemberOf(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->current->relates.isEmpty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found multiple \\relates, \\relatesalso or \\memberof commands in a comment block, using last definition");
  }
  yyextra->current->relatesType = MemberOf;
  yyextra->currentCmd = cmd;
  BEGIN(RelatesParam1);
  return FALSE;
}

static bool handleRefItem(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  addOutput(yyscanner,"@refitem ");
  BEGIN(LineParam);
  return FALSE;
}

static bool handleSection(yyscan_t yyscanner,const QCString &s, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  setOutput(yyscanner,OutputDoc);
  addOutput(yyscanner,"@"+s+" ");
  BEGIN(SectionLabel);
  if      (s=="section")       yyextra->sectionLevel=1;
  else if (s=="subsection")    yyextra->sectionLevel=2;
  else if (s=="subsubsection") yyextra->sectionLevel=3;
  else if (s=="paragraph")     yyextra->sectionLevel=4;
  return FALSE;
}

static bool handleSubpage(yyscan_t yyscanner,const QCString &s, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->current->section!=Entry::EMPTY_SEC &&
      yyextra->current->section!=Entry::PAGEDOC_SEC &&
      yyextra->current->section!=Entry::MAINPAGEDOC_SEC
     )
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\subpage command in a comment block that is not marked as a page!");
  }
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"@"+s+" ");
  BEGIN(SubpageLabel);
  return FALSE;
}

static bool handleAnchor(yyscan_t yyscanner,const QCString &s, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  addOutput(yyscanner,"@"+s+" ");
  BEGIN(AnchorLabel);
  return FALSE;
}

static bool handleImage(yyscan_t yyscanner,const QCString &s, const StringVector &optList)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  for (const auto &opt : optList)
  {
    QCString locOpt(opt);
    locOpt = locOpt.stripWhiteSpace();
    if (locOpt.lower().startsWith("anchor:"))
    {
      addAnchor(yyscanner,locOpt.mid(7));
      break; // real option handling will be done later on
    }
  }
  if (optList.empty())
  {
    addOutput(yyscanner,"@"+s+" ");
  }
  else
  {
    addOutput(yyscanner,"@"+s+"{"+QCString(join(optList,","))+"} ");
  }
  BEGIN(Comment);
  return FALSE;
}

static bool handleCite(yyscan_t yyscanner,const QCString &s, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"@"+s+" ");
  BEGIN(CiteLabel);
  return FALSE;
}

static bool handleFormatBlock(yyscan_t yyscanner,const QCString &s, const StringVector &optList)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  if (optList.empty())
  {
    addOutput(yyscanner,"@"+s+" ");
  }
  else
  {
    addOutput(yyscanner,"@"+s+"{"+QCString(join(optList,","))+"} ");
  }
  //printf("handleFormatBlock(%s) with option(%s)\n",qPrint(s),qPrint(opt));
  yyextra->blockName=s;
  yyextra->commentCount=0;
  BEGIN(FormatBlock);
  return FALSE;
}

static bool handleAddIndex(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  addOutput(yyscanner,"@addindex ");
  BEGIN(LineParam);
  return FALSE;
}

static bool handleIf(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->enabledSectionFound=FALSE;
  yyextra->guardType = Guard_If;
  yyextra->spaceBeforeIf = yyextra->spaceBeforeCmd;
  BEGIN(GuardParam);
  return FALSE;
}

static bool handleIfNot(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->enabledSectionFound=FALSE;
  yyextra->guardType = Guard_IfNot;
  yyextra->spaceBeforeIf = yyextra->spaceBeforeCmd;
  BEGIN(GuardParam);
  return FALSE;
}

static bool handleElseIf(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->guards.empty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\else without matching start command");
  }
  else
  {
    yyextra->guardType = yyextra->enabledSectionFound ? Guard_Skip : Guard_If;
    yyextra->spaceBeforeIf = yyextra->spaceBeforeCmd;
    BEGIN(GuardParam);
  }
  return FALSE;
}

static bool handleElse(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->guards.empty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\else without matching start command");
  }
  else
  {
    yyextra->spaceBeforeIf = yyextra->spaceBeforeCmd;
    BEGIN( SkipGuardedSection );
  }
  return FALSE;
}

static bool handleEndIf(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->guards.empty())
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "found \\endif without matching start command");
  }
  else
  {
    yyextra->guards.pop();
  }
  yyextra->enabledSectionFound=FALSE;
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  BEGIN( GuardParamEnd );
  return FALSE;
}

static bool handleIngroup(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->inGroupParamFound=FALSE;
  BEGIN( InGroupParam );
  return FALSE;
}

static bool handleNoSubGrouping(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->subGrouping = FALSE;
  return FALSE;
}

static bool handleShowInitializer(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->initLines = 100000; // ON
  return FALSE;
}

static bool handleHideInitializer(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->initLines = 0; // OFF
  return FALSE;
}

static bool handleCallgraph(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->callGraph = TRUE; // ON
  return FALSE;
}

static bool handleHideCallgraph(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->callGraph = FALSE; // OFF
  return FALSE;
}

static bool handleCallergraph(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->callerGraph = TRUE; // ON
  return FALSE;
}

static bool handleHideCallergraph(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->callerGraph = FALSE; // OFF
  return FALSE;
}

static bool handleReferencedByRelation(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->referencedByRelation = TRUE; // ON
  return FALSE;
}

static bool handleHideReferencedByRelation(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->referencedByRelation = FALSE; // OFF
  return FALSE;
}

static bool handleReferencesRelation(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->referencesRelation = TRUE; // ON
  return FALSE;
}

static bool handleHideReferencesRelation(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->referencesRelation = FALSE; // OFF
  return FALSE;
}

static bool handleInternal(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!Config_getBool(INTERNAL_DOCS))
  {
    // make sure some whitespace before a \internal command
    // is not treated as "documentation"
    if (yyextra->current->doc.stripWhiteSpace().isEmpty())
    {
      yyextra->current->doc.resize(0);
    }
    yyextra->condCount=0;
    BEGIN( SkipInternal );
  }
  else
  {
    // re-enabled for bug640828
    addOutput(yyscanner," \\internal ");
    yyextra->inInternalDocs = TRUE;
  }
  return FALSE;
}

static bool handleStatic(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->stat = TRUE;
  return FALSE;
}

static bool handlePure(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->virt = Pure;
  return FALSE;
}

static bool handlePrivate(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = Private;
  return FALSE;
}

static bool handlePrivateSection(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = yyextra->protection = Private;
  return FALSE;
}

static bool handleProtected(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = Protected;
  return FALSE;
}

static bool handleProtectedSection(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = yyextra->protection = Protected ;
  return FALSE;
}

static bool handlePublic(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = Public;
  return FALSE;
}

static bool handlePublicSection(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->current->protection = yyextra->protection = Public;
  return FALSE;
}

static bool handleToc(yyscan_t yyscanner,const QCString &, const StringVector &optList)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->current->section==Entry::PAGEDOC_SEC ||
      yyextra->current->section==Entry::MAINPAGEDOC_SEC)
  {
    for (const auto &opt_ : optList)
    {
      QCString opt = QCString(opt_).stripWhiteSpace().lower();
      char dum;
      int level = 5;
      int i = opt.find(':');
      if (i>0)  // found ':' but not on position 0 what would mean just a level
      {
        if (sscanf(opt.right(opt.length() - i - 1).data(),"%d%c",&level,&dum) != 1)
        {
          warn(yyextra->fileName,yyextra->lineNr,"Unknown option:level specified with \\tableofcontents: '%s'", qPrint(QCString(opt_).stripWhiteSpace()));
          opt = "";
        }
        else
        {
          level = (level > 5 ? 5 : level);
          level = (level <= 0 ? 5 : level);
          opt = opt.left(i).stripWhiteSpace();
        }
      }
      if (!opt.isEmpty())
      {
        if (opt == "html")
        {
          yyextra->current->localToc.enableHtml(level);
        }
        else if (opt == "latex")
        {
          yyextra->current->localToc.enableLatex(level);
        }
        else if (opt == "xml")
        {
          yyextra->current->localToc.enableXml(level);
        }
        else if (opt == "docbook")
        {
          yyextra->current->localToc.enableDocbook(level);
        }
        else
        {
          warn(yyextra->fileName,yyextra->lineNr,"Unknown option specified with \\tableofcontents: '%s'", qPrint(QCString(opt_).stripWhiteSpace()));
        }
      }
    }
    if (yyextra->current->localToc.nothingEnabled())
    {
      // for backward compatibility
      yyextra->current->localToc.enableHtml(5);
      yyextra->current->localToc.enableXml(5);
    }
  }
  return FALSE;
}

static bool handleInherit(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  BEGIN(InheritParam);
  return FALSE;
}

static bool handleExtends(yyscan_t yyscanner,const QCString &cmd, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->currentCmd = cmd;
  BEGIN(ExtendsParam);
  return FALSE;
}

static bool handleCopyBrief(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->current->brief.isEmpty() && yyextra->current->doc.isEmpty())
  { // if we don't have a brief or detailed description yet,
    // then the @copybrief should end up in the brief description.
    // otherwise it will be copied inline (see bug691315 & bug700788)
    setOutput(yyscanner,OutputBrief);
  }
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"\\copybrief ");
  return FALSE;
}

static bool handleCopyDetails(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  setOutput(yyscanner,OutputDoc);
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"\\copydetails ");
  return FALSE;
}

static bool handleCopyDoc(yyscan_t yyscanner,const QCString &, const StringVector &)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (yyextra->current->brief.isEmpty() && yyextra->current->doc.isEmpty())
  { // if we don't have a brief or detailed description yet,
    // then the @copybrief should end up in the brief description.
    // otherwise it will be copied inline (see bug691315 & bug700788)
    setOutput(yyscanner,OutputBrief);
  }
  if (!yyextra->spaceBeforeCmd.isEmpty())
  {
    addOutput(yyscanner,yyextra->spaceBeforeCmd);
    yyextra->spaceBeforeCmd.resize(0);
  }
  addOutput(yyscanner,"\\copybrief ");
  yyextra->copyDocArg.resize(0);
  yyextra->braceCount = 0;
  BEGIN(CopyDoc);
  return FALSE;
}

//-----------------------------------------------------------------------------------------

static void initParser(yyscan_t yyscanner)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->sectionLabel.resize(0);
  yyextra->sectionTitle.resize(0);
  yyextra->docGroup.clearHeader();
  yyextra->insideParBlock = FALSE;
}


static bool getDocSectionName(int s)
{
  switch(s)
  {
    case Entry::CLASSDOC_SEC:
    case Entry::STRUCTDOC_SEC:
    case Entry::UNIONDOC_SEC:
    case Entry::EXCEPTIONDOC_SEC:
    case Entry::NAMESPACEDOC_SEC:
    case Entry::PROTOCOLDOC_SEC:
    case Entry::CATEGORYDOC_SEC:
    case Entry::ENUMDOC_SEC:
    case Entry::PAGEDOC_SEC:
    case Entry::VARIABLEDOC_SEC:
    case Entry::MEMBERDOC_SEC:
    case Entry::OVERLOADDOC_SEC:
    case Entry::FILEDOC_SEC:
    case Entry::DEFINEDOC_SEC:
    case Entry::GROUPDOC_SEC:
    case Entry::MAINPAGEDOC_SEC:
    case Entry::PACKAGEDOC_SEC:
    case Entry::DIRDOC_SEC:
    case Entry::EXAMPLE_SEC:
    case Entry::MEMBERGRP_SEC:
    case Entry::CONCEPTDOC_SEC:
      return TRUE;
    default:
      return FALSE;
  }
}

//-----------------------------------------------------------------------------

static bool makeStructuralIndicator(yyscan_t yyscanner,Entry::Sections s)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  //printf("yyextra->current->section=%x\n",yyextra->current->section);
  if (getDocSectionName(yyextra->current->section))
  {
    return TRUE;
  }
  else
  {
    yyextra->needNewEntry = TRUE;
    yyextra->current->section = s;
    yyextra->current->fileName = yyextra->fileName;
    yyextra->current->startLine = yyextra->lineNr;
    yyextra->current->docLine = yyextra->lineNr;
    return FALSE;
  }
}

//-----------------------------------------------------------------

static void lineCount(yyscan_t yyscanner)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  for( const char* c = yytext ; *c ; ++c )
    yyextra->lineNr += (*c == '\n') ;
}

//-----------------------------------------------------------------

static QCString stripQuotes(const char *s)
{
  QCString name;
  if (s==0 || *s==0) return name;
  name=s;
  if (name.at(0)=='"' && name.at(name.length()-1)=='"')
  {
    name=name.mid(1,name.length()-2);
  }
  return name;
}

//-----------------------------------------------------------------

static void addXRefItem(yyscan_t yyscanner,
                        const QCString &listName,const QCString &itemTitle,
                        const QCString &listTitle,bool append)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (listName.isEmpty()) return;
  //printf("addXRefItem(%s,%s,%s,%d)\n",listName,itemTitle,listTitle,append);

  std::unique_lock<std::mutex> lock(g_sectionMutex);

  RefList *refList = RefListManager::instance().add(listName,listTitle,itemTitle);
  RefItem *item = 0;
  for (auto it = yyextra->current->sli.rbegin(); it != yyextra->current->sli.rend(); ++it)
  {
    RefItem *i = *it;
    if (i && i->list()->listName()==listName)
    {
      //printf("found %s lii->type=%s\n",listName,qPrint(i->list()->listName()));
      item = i;
      break;
    }
  }
  if (item && append) // already found item of same type just before this one
  {
    //printf("listName=%s item id = %d existing\n",listName,item->id());
    item->setText(item->text() + " <p>" + yyextra->outputXRef);
    //printf("%s: text +=%s\n",listName,qPrint(item->text));
  }
  else // new item
  {

    // if we have already an item from the same list type (e.g. a second @todo)
    // in the same Entry (i.e. lii!=0) then we reuse its link anchor.
    item = refList->add();
    //printf("listName=%s item id = %d new yyextra->current=%p\n",listName,item->id(),yyextra->current);
    QCString anchorLabel;
    anchorLabel.sprintf("_%s%06d",listName.data(),item->id());
    item->setText(yyextra->outputXRef);
    item->setAnchor(anchorLabel);
    yyextra->current->sli.push_back(item);
    QCString cmdString;
    cmdString.sprintf(" \\xrefitem %s %d.",qPrint(listName),item->id());
    if (yyextra->inBody)
    {
      yyextra->current->inbodyDocs += cmdString;
    }
    else
    {
      yyextra->current->doc += cmdString;
    }

    {
      SectionManager &sm = SectionManager::instance();
      const SectionInfo *si = sm.find(anchorLabel);
      if (si)
      {
        if (!si->ref().isEmpty()) // we are from a tag file
        {
          si = sm.replace(anchorLabel,listName,yyextra->lineNr,
              yyextra->sectionTitle,SectionType::Anchor,
              yyextra->sectionLevel);
          yyextra->current->anchors.push_back(si);
        }
        else if (si->lineNr() != -1)
        {
          warn(listName,yyextra->lineNr,"multiple use of section label '%s', (first occurrence: %s, line %d)",qPrint(anchorLabel),qPrint(si->fileName()),si->lineNr());
        }
        else
        {
          warn(listName,yyextra->lineNr,"multiple use of section label '%s', (first occurrence: %s)",qPrint(anchorLabel),qPrint(si->fileName()));
        }
      }
      else
      {
        si = sm.add(anchorLabel,listName,yyextra->lineNr,
            yyextra->sectionTitle,SectionType::Anchor,
            yyextra->sectionLevel);
        yyextra->current->anchors.push_back(si);
      }
    }
  }
  yyextra->outputXRef.resize(0);
}

//-----------------------------------------------------------------------------

// Adds a formula text to the list/dictionary of formulas if it was
// not already added. Returns the label of the formula.
static QCString addFormula(yyscan_t yyscanner)
{
  std::unique_lock<std::mutex> lock(g_formulaMutex);
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  QCString formLabel;
  int id = FormulaManager::instance().addFormula(yyextra->formulaText.str());
  formLabel.sprintf("\\_form#%d",id);
  for (int i=0;i<yyextra->formulaNewLines;i++) formLabel+="@_fakenl"; // add fake newlines to
                                                         // keep the warnings
                                                         // correctly aligned.
  return formLabel;
}

//-----------------------------------------------------------------------------

static SectionType sectionLevelToType(int level)
{
  if (level>=0 && level<5) return (SectionType)level;
  return SectionType::Anchor;
}

static void addSection(yyscan_t yyscanner)
{
  std::unique_lock<std::mutex> lock(g_sectionMutex);
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  SectionManager &sm = SectionManager::instance();
  const SectionInfo *si = sm.find(yyextra->sectionLabel);
  if (si)
  {
    if (!si->ref().isEmpty()) // we are from a tag file
    {
      // create a new section element
      yyextra->sectionTitle+=yytext;
      yyextra->sectionTitle=yyextra->sectionTitle.stripWhiteSpace();
      si = sm.replace(yyextra->sectionLabel,yyextra->fileName,yyextra->lineNr,
                      yyextra->sectionTitle,sectionLevelToType(yyextra->sectionLevel),
                      yyextra->sectionLevel);

      // add section to this entry
      yyextra->current->anchors.push_back(si);
    }
    else if (si->lineNr() != -1)
    {
      warn(yyextra->fileName,yyextra->lineNr,"multiple use of section label '%s' while adding section, (first occurrence: %s, line %d)",qPrint(yyextra->sectionLabel),qPrint(si->fileName()),si->lineNr());
    }
    else
    {
      warn(yyextra->fileName,yyextra->lineNr,"multiple use of section label '%s' while adding section, (first occurrence: %s)",qPrint(yyextra->sectionLabel),qPrint(si->fileName()));
    }
  }
  else
  {
    // create a new section element
    yyextra->sectionTitle+=yytext;
    yyextra->sectionTitle=yyextra->sectionTitle.stripWhiteSpace();
    si = sm.add(yyextra->sectionLabel,yyextra->fileName,yyextra->lineNr,
                yyextra->sectionTitle,sectionLevelToType(yyextra->sectionLevel),
                yyextra->sectionLevel);

    // add section to this entry
    yyextra->current->anchors.push_back(si);
  }
}

//-----------------------------------------------------------------------------

static void addCite(yyscan_t yyscanner)
{
  std::unique_lock<std::mutex> lock(g_citeMutex);
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  QCString name(yytext);
  if (yytext[0] =='"')
  {
    name=yytext+1;
    name=name.left((int)yyleng-2);
  }
  CitationManager::instance().insert(name);
}

//-----------------------------------------------------------------------------

// strip trailing whitespace (excluding newlines) from string s
static void stripTrailingWhiteSpace(QCString &s)
{
  uint len = s.length();
  int i = (int)len-1;
  char c;
  while (i>=0)
  {
    c = s.at(i);
    if (c==' ' || c=='\t' || c=='\r') // normal whitespace
    {
      i--;
    }
    else if (c=='r' && i>=7 && qstrncmp("\\ilinebr",s.data()+i-7,8)==0) // special line break marker
    {
      i-=8;
    }
    else // non-whitespace
    {
      break;
    }
  }
  //printf("stripTrailingWhitespace(%s) i=%d len=%d\n",qPrint(s),i,len);
  if (i!=(int)len-1)
  {
    s.resize(i+2); // string up to and including char at pos i and \0 terminator
  }
}

// selects the output to write to
static inline void setOutput(yyscan_t yyscanner,OutputContext ctx)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  bool xrefAppendToPrev = yyextra->xrefAppendFlag;
  // determine append flag for the next item (i.e. the end of this item)
  yyextra->xrefAppendFlag = !yyextra->inBody &&
                   yyextra->inContext==OutputXRef && ctx==OutputXRef && // two consecutive xref items
                   yyextra->newXRefKind==yyextra->xrefKind &&                    // of the same kind
                   (yyextra->xrefKind!=XRef_Item ||
                    yyextra->newXRefItemKey==yyextra->xrefItemKey);              // with the same key if \xrefitem
  //printf("%d && %d && %d && (%d || %d)\n",
  //                 yyextra->inContext==OutputXRef,
  //                 ctx==OutputXRef,
  //                 yyextra->newXRefKind==yyextra->xrefKind,
  //                 yyextra->xrefKind!=XRef_Item,
  //                 yyextra->newXRefItemKey==yyextra->xrefItemKey);
  //printf("refKind=%d yyextra->newXRefKind=%d xrefAppendToPrev=%d yyextra->xrefAppendFlag=%d\n",
  //             yyextra->xrefKind,yyextra->newXRefKind,xrefAppendToPrev,yyextra->xrefAppendFlag);

  //printf("setOutput(yyscanner,yyextra->inContext=%d ctx=%d)\n",yyextra->inContext,ctx);
  if (yyextra->inContext==OutputXRef) // end of XRef section => add the item
  {
    // See if we can append this new xref item to the previous one.
    // We know this at the start of the next item of the same
    // type and need to remember this until the end of that item.
    switch(yyextra->xrefKind)
    {
      case XRef_Todo:
        addXRefItem(yyscanner,QCString("todo"),
            theTranslator->trTodo(),
            theTranslator->trTodoList(),
            xrefAppendToPrev
            );
        break;
      case XRef_Test:
        addXRefItem(yyscanner,QCString("test"),
            theTranslator->trTest(),
            theTranslator->trTestList(),
            xrefAppendToPrev
            );
        break;
      case XRef_Bug:
        addXRefItem(yyscanner,QCString("bug"),
            theTranslator->trBug(),
            theTranslator->trBugList(),
            xrefAppendToPrev
            );
        break;
      case XRef_Deprecated:
        addXRefItem(yyscanner,QCString("deprecated"),
            theTranslator->trDeprecated(),
            theTranslator->trDeprecatedList(),
            xrefAppendToPrev
            );
        break;
      case XRef_Item:  // user defined list
        addXRefItem(yyscanner,yyextra->xrefItemKey,
            yyextra->xrefItemTitle,
            yyextra->xrefListTitle,
            xrefAppendToPrev
            );
        break;
      case XRef_None:
        ASSERT(0);
        break;
    }
  }
  yyextra->xrefItemKey = yyextra->newXRefItemKey;

  int oldContext = yyextra->inContext;
  yyextra->inContext = ctx;
  if (yyextra->inContext!=OutputXRef && yyextra->inBody) yyextra->inContext=OutputInbody;
  switch(yyextra->inContext)
  {
    case OutputDoc:
      if (oldContext!=yyextra->inContext)
      {
        stripTrailingWhiteSpace(yyextra->current->doc);
        if (yyextra->current->doc.isEmpty()) yyextra->current->docLine = yyextra->lineNr;
        if (yyextra->current->docFile.isEmpty())
        {
          yyextra->current->docFile = yyextra->fileName;
          yyextra->current->docLine = yyextra->lineNr;
        }
      }
      yyextra->pOutputString = &yyextra->current->doc;
      break;
    case OutputBrief:
      if (oldContext!=yyextra->inContext)
      {
        if (yyextra->current->brief.isEmpty()) yyextra->current->briefLine = yyextra->lineNr;
        if (yyextra->current->briefFile.isEmpty())
        {
          yyextra->current->briefFile = yyextra->fileName;
          yyextra->current->briefLine = yyextra->lineNr;
        }
      }
      if (yyextra->current->brief.stripWhiteSpace().isEmpty()) // we only want one brief
        // description even if multiple
        // are given...
      {
        yyextra->pOutputString = &yyextra->current->brief;
      }
      else
      {
        if (!yyextra->current->doc.isEmpty()) // when appending parts add a new line
        {
          yyextra->current->doc += "\n";
        }
        yyextra->pOutputString = &yyextra->current->doc;
        yyextra->inContext = OutputDoc; // need to switch to detailed docs, see bug 631380
      }
      break;
    case OutputXRef:
      yyextra->pOutputString = &yyextra->outputXRef;
      // first item found, so can't append to previous
      //yyextra->xrefAppendFlag = FALSE;
      break;
    case OutputInbody:
      yyextra->pOutputString = &yyextra->current->inbodyDocs;
      break;
  }
}


static void addAnchor(yyscan_t yyscanner,const QCString &anchor)
{
  std::unique_lock<std::mutex> lock(g_sectionMutex);
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  SectionManager &sm = SectionManager::instance();
  const SectionInfo *si = sm.find(anchor);
  if (si)
  {
    if (!si->ref().isEmpty()) // we are from a tag file
    {
      si = sm.replace(anchor,yyextra->fileName,yyextra->lineNr,QCString(),SectionType::Anchor,0);
      yyextra->current->anchors.push_back(si);
    }
    else if (si->lineNr() != -1)
    {
      warn(yyextra->fileName,yyextra->lineNr,
          "multiple use of section label '%s' while adding anchor, (first occurrence: %s, line %d)",
          qPrint(anchor),qPrint(si->fileName()),si->lineNr());
    }
    else
    {
      warn(yyextra->fileName,yyextra->lineNr,"multiple use of section label '%s' while adding anchor, (first occurrence: %s)",
          qPrint(anchor),qPrint(si->fileName()));
    }
  }
  else
  {
    si = sm.add(anchor,yyextra->fileName,yyextra->lineNr,QCString(),SectionType::Anchor,0);
    yyextra->current->anchors.push_back(si);
  }
}

// add a string to the output
static inline void addOutput(yyscan_t yyscanner,const char *s)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  //printf("addOutput(yyscanner,%s)\n",s);
  *yyextra->pOutputString+=s;
}

// add a string to the output
static inline void addOutput(yyscan_t yyscanner,const QCString &s)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  //printf("addOutput(yyscanner,%s)\n",s);
  *yyextra->pOutputString+=s;
}

// add a character to the output
static inline void addOutput(yyscan_t yyscanner,char c)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  *yyextra->pOutputString+=c;
}

static void addIline(yyscan_t yyscanner,int lineNr)
{
  char cmd[30];
  sprintf(cmd,"\\iline %d \\ilinebr ",lineNr);
  addOutput(yyscanner, cmd);
}

static void endBrief(yyscan_t yyscanner,bool addToOutput)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (!yyextra->current->brief.stripWhiteSpace().isEmpty())
  { // only go to the detailed description if we have
    // found some brief description and not just whitespace
    yyextra->briefEndsAtDot=FALSE;
    setOutput(yyscanner,OutputDoc);
    if (addToOutput) addOutput(yyscanner,yytext);
  }
}

static yy_size_t yyread(yyscan_t yyscanner,char *buf,yy_size_t max_size)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  yyextra->prevPosition=yyextra->inputPosition;
  yy_size_t c=0;
  while( c < max_size && yyextra->inputString[yyextra->inputPosition] )
  {
    *buf = yyextra->inputString[yyextra->inputPosition++] ;
    //printf("%d (%c)\n",*buf,*buf);
    c++; buf++;
  }
  return c;
}

//----------------------------------------------------------------------------

static void checkFormula(yyscan_t yyscanner)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  if (YY_START==ReadFormulaShort || YY_START==ReadFormulaRound || YY_START==ReadFormulaLong)
  {
    warn(yyextra->fileName,yyextra->lineNr,"End of comment block while inside formula.");
  }
}

//----------------------------------------------------------------------------

struct CommentScanner::Private
{
  yyscan_t yyscanner;
  commentscanYY_state extra;
};

CommentScanner::CommentScanner() : p(std::make_unique<Private>())
{
  commentscanYYlex_init_extra(&p->extra,&p->yyscanner);
#ifdef FLEX_DEBUG
  commentscanYYset_debug(1,p->yyscanner);
#endif
}

CommentScanner::~CommentScanner()
{
  commentscanYYlex_destroy(p->yyscanner);
}

bool CommentScanner::parseCommentBlock(/* in */     OutlineParserInterface *parser,
                       /* in */     Entry *curEntry,
                       /* in */     const QCString &comment,
                       /* in */     const QCString &fileName,
                       /* in,out */ int  &lineNr,
                       /* in */     bool isBrief,
                       /* in */     bool isAutoBriefOn,
                       /* in */     bool isInbody,
                       /* in,out */ Protection &prot,
                       /* in,out */ int &position,
                       /* out */    bool &newEntryNeeded,
                       /* in */     bool markdownSupport
                      )
{
  yyscan_t yyscanner = p->yyscanner;
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  //printf("parseCommentBlock() isBrief=%d isAutoBriefOn=%d lineNr=%d\n",
  //    isBrief,isAutoBriefOn,lineNr);

  initParser(yyscanner);
  yyextra->guards = std::stack<GuardedSection>();
  yyextra->langParser     = parser;
  yyextra->current        = curEntry;
  yyextra->current->docLine = (lineNr > 1 ? lineNr : 1);
  if (comment.isEmpty()) return FALSE; // avoid empty strings
  yyextra->inputString    = comment;
  yyextra->inputString.append(" ");
  yyextra->inputPosition  = position;
  yyextra->lineNr         = lineNr;
  yyextra->fileName       = fileName;
  yyextra->protection     = prot;
  yyextra->needNewEntry   = FALSE;
  yyextra->xrefKind       = XRef_None;
  yyextra->xrefAppendFlag = FALSE;
  yyextra->insidePre      = FALSE;
  yyextra->parseMore      = FALSE;
  yyextra->inBody         = isInbody;
  yyextra->markdownSupport= markdownSupport;
  yyextra->outputXRef.resize(0);
  if (!isBrief && !isAutoBriefOn && !yyextra->current->doc.isEmpty())
  { // add newline separator between detailed comment blocks
    yyextra->current->doc += '\n';
  }
  setOutput(yyscanner, isBrief || isAutoBriefOn ? OutputBrief : OutputDoc );
  yyextra->briefEndsAtDot = isAutoBriefOn;
  yyextra->condCount    = 0;
  yyextra->sectionLevel = 0;
  yyextra->spaceBeforeCmd.resize(0);
  yyextra->spaceBeforeIf.resize(0);

  printlex(yy_flex_debug, TRUE, __FILE__, !fileName.isEmpty() ? qPrint(fileName): NULL);
  if (!yyextra->current->inbodyDocs.isEmpty() && isInbody) // separate in body fragments
  {
    yyextra->current->inbodyDocs+="\n\n";
  }

  Debug::print(Debug::CommentScan,0,"-----------\nCommentScanner: %s:%d\n"
               "input=[\n%s]\n",qPrint(fileName),lineNr,qPrint(yyextra->inputString)
              );

  commentscanYYrestart( 0, yyscanner );
  BEGIN( Comment );
  commentscanYYlex(yyscanner);
  setOutput(yyscanner, OutputDoc );

  if (YY_START==OverloadParam) // comment ended with \overload
  {
    addOutput(yyscanner,getOverloadDocs());
  }

  if (!yyextra->guards.empty())
  {
    warn(yyextra->fileName,yyextra->lineNr,"Documentation block ended in the middle of a conditional section!");
  }

  if (yyextra->insideParBlock)
  {
    warn(yyextra->fileName,yyextra->lineNr,
        "Documentation block ended while inside a \\parblock. Missing \\endparblock");
  }

  yyextra->current->doc=stripLeadingAndTrailingEmptyLines(yyextra->current->doc,yyextra->current->docLine);

  if (yyextra->current->section==Entry::FILEDOC_SEC && yyextra->current->doc.isEmpty())
  {
    // to allow a comment block with just a @file command.
    yyextra->current->doc="\n\n";
  }

  if (yyextra->current->section==Entry::MEMBERGRP_SEC &&
      yyextra->docGroup.isEmpty()) // @name section but no group started yet
  {
    yyextra->docGroup.open(yyextra->current,yyextra->fileName,yyextra->lineNr,true);
  }

  Debug::print(Debug::CommentScan,0,"-----------\nCommentScanner: %s:%d\noutput=[\n"
               "brief=[line=%d\n%s]\ndocs=[line=%d\n%s]\ninbody=[line=%d\n%s]\n]\n===========\n",
               qPrint(fileName),lineNr,
               yyextra->current->briefLine,qPrint(yyextra->current->brief),
               yyextra->current->docLine,qPrint(yyextra->current->doc),
               yyextra->current->inbodyLine,qPrint(yyextra->current->inbodyDocs)
              );

  checkFormula(yyscanner);
  prot = yyextra->protection;

  yyextra->docGroup.addDocs(curEntry);

  newEntryNeeded = yyextra->needNewEntry;

  // if we did not proceed during this call, it does not make
  // sense to continue, since we get stuck. See bug 567346 for situations
  // were this happens
  if (yyextra->parseMore && position==yyextra->inputPosition) yyextra->parseMore=FALSE;

  if (yyextra->parseMore) position=yyextra->inputPosition; else position=0;

  lineNr = yyextra->lineNr;
  //printf("position=%d yyextra->parseMore=%d newEntryNeeded=%d\n",
  //  position,yyextra->parseMore,newEntryNeeded);

  printlex(yy_flex_debug, FALSE, __FILE__, !fileName.isEmpty() ? qPrint(fileName): NULL);
  return yyextra->parseMore;
}

static void handleGuard(yyscan_t yyscanner,const QCString &expr)
{
  struct yyguts_t *yyg = (struct yyguts_t*)yyscanner;
  CondParser prs;
  bool sectionEnabled=prs.parse(yyextra->fileName,yyextra->lineNr,expr.stripWhiteSpace());
  bool parentEnabled = TRUE;
  if (!yyextra->guards.empty()) parentEnabled = yyextra->guards.top().isEnabled();
  if (parentEnabled)
  {
    if (
        (sectionEnabled && yyextra->guardType==Guard_If) ||
        (!sectionEnabled && yyextra->guardType==Guard_IfNot)
       ) // section is visible
    {
      yyextra->guards.push(GuardedSection(TRUE,TRUE));
      yyextra->enabledSectionFound=TRUE;
      BEGIN( GuardParamEnd );
    }
    else // section is invisible
    {
      if (yyextra->guardType!=Guard_Skip)
      {
        yyextra->guards.push(GuardedSection(FALSE,TRUE));
      }
      BEGIN( SkipGuardedSection );
    }
  }
  else // invisible because of parent
  {
    yyextra->guards.push(GuardedSection(FALSE,FALSE));
    BEGIN( SkipGuardedSection );
  }
}

void CommentScanner::initGroupInfo(Entry *entry)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.initGroupInfo(entry);
}

void CommentScanner::enterFile(const QCString &fileName,int lineNr)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.enterFile(fileName,lineNr);
}

void CommentScanner::leaveFile(const QCString &fileName,int lineNr)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.leaveFile(fileName,lineNr);
}

void CommentScanner::enterCompound(const QCString &fileName,int lineNr,const QCString &name)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.enterCompound(fileName,lineNr,name);
}

void CommentScanner::leaveCompound(const QCString &fileName,int lineNr,const QCString &name)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.leaveCompound(fileName,lineNr,name);
}

void CommentScanner::open(Entry *e,const QCString &fileName,int lineNr,bool implicit)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.open(e,fileName,lineNr,implicit);
}

void CommentScanner::close(Entry *e,const QCString &fileName,int lineNr,bool foundInline,bool implicit)
{
  struct yyguts_t *yyg = (struct yyguts_t*)p->yyscanner;
  yyextra->docGroup.close(e,fileName,lineNr,foundInline,implicit);
}

#if USE_STATE2STRING
#include "commentscan.l.h"
#endif
