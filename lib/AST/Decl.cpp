//===--- Decl.cpp - Swift Language Decl ASTs ------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file implements the Decl class and subclasses.
//
//===----------------------------------------------------------------------===//

#include "swift/AST/Decl.h"
#include "swift/AST/ASTContext.h"
#include "swift/AST/Expr.h"
#include "swift/AST/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
using namespace swift;
using llvm::cast;

// Only allow allocation of Decls using the allocator in ASTContext.
void *Decl::operator new(size_t Bytes, ASTContext &C,
                         unsigned Alignment) throw() {
  return C.Allocate(Bytes, Alignment);
}


llvm::SMLoc NamedDecl::getLocStart() const {
  switch (getKind()) {
  case OneOfDeclKind:       return cast<OneOfDecl>(this)->getLocStart();
  case VarDeclKind:        return cast<VarDecl>(this)->getLocStart();
  case FuncDeclKind:       return cast<FuncDecl>(this)->getLocStart();
  case OneOfElementDeclKind:return cast<OneOfElementDecl>(this)->getLocStart();
  case ArgDeclKind:        return cast<ArgDecl>(this)->getLocStart();
  case AnonDeclKind:       return cast<AnonDecl>(this)->getLocStart();
  case ElementRefDeclKind: return cast<ElementRefDecl>(this)->getLocStart();
  }

  assert(0 && "Unknown decl kind");
  return llvm::SMLoc();
}

OneOfElementDecl *OneOfDecl::getElement(Identifier Name) const {
  // FIXME: Linear search is not great for large oneof decls.
  for (unsigned i = 0, e = NumElements; i != e; ++i)
    if (Elements[i]->Name == Name)
      return Elements[i];
  return 0;
}



void Decl::dump() const { print(llvm::errs()); llvm::errs() << '\n'; }

void Decl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  switch (getKind()) {
  case OneOfDeclKind:       return cast<OneOfDecl>(this)->print(OS, Indent);
  case VarDeclKind:        return cast<VarDecl>(this)->print(OS, Indent);
  case FuncDeclKind:       return cast<FuncDecl>(this)->print(OS, Indent);
  case OneOfElementDeclKind:return cast<OneOfElementDecl>(this)->print(OS,Indent);
  case ArgDeclKind:        return cast<ArgDecl>(this)->print(OS, Indent);
  case AnonDeclKind:       return cast<AnonDecl>(this)->print(OS, Indent);
  case ElementRefDeclKind: return cast<ElementRefDecl>(this)->print(OS, Indent);
  }
}

void NamedDecl::printCommon(llvm::raw_ostream &OS, unsigned Indent) const {
  OS << "'" << Name << "'";
}

void OneOfDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(oneofdecl ";
  printCommon(OS, Indent);
  
  for (unsigned i = 0, e = NumElements; i != e; ++i) {
    OS << '\n';
    Elements[i]->print(OS, Indent+2);
  }
  
  OS << ')';
}


void ValueDecl::printCommon(llvm::raw_ostream &OS, unsigned Indent) const {
  NamedDecl::printCommon(OS, Indent);
  OS << " type='";
  Ty->print(OS);
  OS << "'";
  
  if (Init) {
    OS << '\n';
    Init->print(OS, Indent+2);
  }
  OS << ')';
}

void VarDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(vardecl ";
  printCommon(OS, Indent);
}

void FuncDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(funcdecl ";
  printCommon(OS, Indent);
}

void OneOfElementDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(oneofelementdecl ";
  printCommon(OS, Indent);
}

void ArgDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(argdecl ";
  printCommon(OS, Indent);
}

void AnonDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(anondecl ";
  printCommon(OS, Indent);
}

void ElementRefDecl::print(llvm::raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << "(elementrefdecl ";
  printCommon(OS, Indent);
}

