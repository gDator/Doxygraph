static const char *stateToString(int state)
{
  switch(state)
  {
    case INITIAL: return "INITIAL";
    case AlignAs: return "AlignAs";
    case AlignAsEnd: return "AlignAsEnd";
    case Define: return "Define";
    case DefineEnd: return "DefineEnd";
    case CompoundName: return "CompoundName";
    case ClassVar: return "ClassVar";
    case CSConstraintName: return "CSConstraintName";
    case CSConstraintType: return "CSConstraintType";
    case CSIndexer: return "CSIndexer";
    case ClassCategory: return "ClassCategory";
    case ClassTemplSpec: return "ClassTemplSpec";
    case CliPropertyType: return "CliPropertyType";
    case CliPropertyIndex: return "CliPropertyIndex";
    case CliOverride: return "CliOverride";
    case Bases: return "Bases";
    case BasesProt: return "BasesProt";
    case NextSemi: return "NextSemi";
    case BitFields: return "BitFields";
    case EnumBaseType: return "EnumBaseType";
    case FindMembers: return "FindMembers";
    case FindMembersPHP: return "FindMembersPHP";
    case FindMemberName: return "FindMemberName";
    case FindFields: return "FindFields";
    case Function: return "Function";
    case FuncRound: return "FuncRound";
    case ExcpRound: return "ExcpRound";
    case ExcpList: return "ExcpList";
    case FuncQual: return "FuncQual";
    case TrailingReturn: return "TrailingReturn";
    case Operator: return "Operator";
    case Array: return "Array";
    case ReadBody: return "ReadBody";
    case ReadNSBody: return "ReadNSBody";
    case ReadBodyIntf: return "ReadBodyIntf";
    case Using: return "Using";
    case UsingAlias: return "UsingAlias";
    case UsingAliasEnd: return "UsingAliasEnd";
    case UsingDirective: return "UsingDirective";
    case SkipCurly: return "SkipCurly";
    case SkipCurlyCpp: return "SkipCurlyCpp";
    case SkipCurlyEndDoc: return "SkipCurlyEndDoc";
    case SkipString: return "SkipString";
    case SkipPHPString: return "SkipPHPString";
    case SkipInits: return "SkipInits";
    case SkipC11Inits: return "SkipC11Inits";
    case SkipC11Attribute: return "SkipC11Attribute";
    case SkipCPP: return "SkipCPP";
    case SkipComment: return "SkipComment";
    case SkipCxxComment: return "SkipCxxComment";
    case SkipCurlyBlock: return "SkipCurlyBlock";
    case SkipRoundBlock: return "SkipRoundBlock";
    case Sharp: return "Sharp";
    case SkipRound: return "SkipRound";
    case SkipSquare: return "SkipSquare";
    case StaticAssert: return "StaticAssert";
    case DeclType: return "DeclType";
    case TypedefName: return "TypedefName";
    case TryFunctionBlock: return "TryFunctionBlock";
    case TryFunctionBlockEnd: return "TryFunctionBlockEnd";
    case Comment: return "Comment";
    case PackageName: return "PackageName";
    case JavaImport: return "JavaImport";
    case PHPUse: return "PHPUse";
    case PHPUseAs: return "PHPUseAs";
    case CSAccessorDecl: return "CSAccessorDecl";
    case CSGeneric: return "CSGeneric";
    case PreLineCtrl: return "PreLineCtrl";
    case DefinePHP: return "DefinePHP";
    case DefinePHPEnd: return "DefinePHPEnd";
    case OldStyleArgs: return "OldStyleArgs";
    case SkipVerbString: return "SkipVerbString";
    case ObjCMethod: return "ObjCMethod";
    case ObjCReturnType: return "ObjCReturnType";
    case ObjCParams: return "ObjCParams";
    case ObjCParamType: return "ObjCParamType";
    case ObjCProtocolList: return "ObjCProtocolList";
    case ObjCPropAttr: return "ObjCPropAttr";
    case ObjCSkipStatement: return "ObjCSkipStatement";
    case QtPropType: return "QtPropType";
    case QtPropAttr: return "QtPropAttr";
    case QtPropRead: return "QtPropRead";
    case QtPropWrite: return "QtPropWrite";
    case ReadInitializer: return "ReadInitializer";
    case ReadInitializerPtr: return "ReadInitializerPtr";
    case UNOIDLAttributeBlock: return "UNOIDLAttributeBlock";
    case GetCallType: return "GetCallType";
    case CppQuote: return "CppQuote";
    case EndCppQuote: return "EndCppQuote";
    case MemberSpec: return "MemberSpec";
    case MemberSpecSkip: return "MemberSpecSkip";
    case EndTemplate: return "EndTemplate";
    case FuncPtr: return "FuncPtr";
    case FuncPtrOperator: return "FuncPtrOperator";
    case EndFuncPtr: return "EndFuncPtr";
    case ReadFuncArgType: return "ReadFuncArgType";
    case ReadTempArgs: return "ReadTempArgs";
    case IDLUnionCase: return "IDLUnionCase";
    case NSAliasName: return "NSAliasName";
    case NSAliasArg: return "NSAliasArg";
    case CopyString: return "CopyString";
    case CopyPHPString: return "CopyPHPString";
    case CopyGString: return "CopyGString";
    case CopyPHPGString: return "CopyPHPGString";
    case CopyRound: return "CopyRound";
    case CopySharp: return "CopySharp";
    case CopyCurly: return "CopyCurly";
    case GCopyRound: return "GCopyRound";
    case GCopySquare: return "GCopySquare";
    case GCopyCurly: return "GCopyCurly";
    case SkipUnionSwitch: return "SkipUnionSwitch";
    case Specialization: return "Specialization";
    case SpecializationSingleQuote: return "SpecializationSingleQuote";
    case SpecializationDoubleQuote: return "SpecializationDoubleQuote";
    case FuncFunc: return "FuncFunc";
    case FuncFuncEnd: return "FuncFuncEnd";
    case FuncFuncType: return "FuncFuncType";
    case FuncFuncArray: return "FuncFuncArray";
    case CopyArgString: return "CopyArgString";
    case CopyArgPHPString: return "CopyArgPHPString";
    case CopyArgRound: return "CopyArgRound";
    case CopyArgSquare: return "CopyArgSquare";
    case CopyArgSharp: return "CopyArgSharp";
    case CopyArgComment: return "CopyArgComment";
    case CopyArgCommentLine: return "CopyArgCommentLine";
    case CopyArgVerbatim: return "CopyArgVerbatim";
    case HereDoc: return "HereDoc";
    case HereDocEnd: return "HereDocEnd";
    case CopyHereDoc: return "CopyHereDoc";
    case CopyHereDocEnd: return "CopyHereDocEnd";
    case RawString: return "RawString";
    case RawGString: return "RawGString";
    case CSString: return "CSString";
    case IDLAttribute: return "IDLAttribute";
    case IDLProp: return "IDLProp";
    case IDLPropName: return "IDLPropName";
    case SliceOptional: return "SliceOptional";
    case SliceMetadata: return "SliceMetadata";
    case SliceSequence: return "SliceSequence";
    case SliceSequenceName: return "SliceSequenceName";
    case SliceDictionary: return "SliceDictionary";
    case SliceDictionaryName: return "SliceDictionaryName";
    case Prototype: return "Prototype";
    case PrototypePtr: return "PrototypePtr";
    case PrototypeQual: return "PrototypeQual";
    case PrototypeExc: return "PrototypeExc";
    case PrototypeSkipLine: return "PrototypeSkipLine";
    case DocLine: return "DocLine";
    case DocBlock: return "DocBlock";
    case DocCopyBlock: return "DocCopyBlock";
    case RequiresClause: return "RequiresClause";
    case RequiresExpression: return "RequiresExpression";
    case ConceptName: return "ConceptName";
    case Deprecated_round: return "Deprecated_round";
  }
  return "Unknown";
}
