//===--- Formatting.h - Swift code indenting --------------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_IDE_CODE_FORMATTING_H
#define SWIFT_IDE_CODE_FORMATTING_H

#include "swift/Basic/SourceManager.h"
#include "swift/AST/Module.h"

namespace {
  class FormatContext;
}

namespace swift {
  namespace ide {

class LineRange {
  unsigned StartLine;
  unsigned Length;

public:
  LineRange()
    :StartLine(0), Length(0) { }
  LineRange(unsigned StartLine, unsigned Length)
    :StartLine(StartLine), Length(Length) { }
  LineRange(const LineRange &Other)
    :StartLine(Other.StartLine), Length(Other.Length) { }

  bool isValid() const {
    return Length != 0;
  }

  unsigned startLine() const {
    return StartLine;
  }

  unsigned endLine() const {
    return isValid() ? StartLine + Length - 1 : 0;
  }

  unsigned lineCount() const {
    return Length;
  }

  void setRange(unsigned NewStartLine, unsigned NewLength) {
    StartLine = NewStartLine;
    Length = NewLength;
  }

  void extendToIncludeLine(unsigned Line) {
    if (!isValid()) {
      StartLine = Line;
      Length = 1;
    }
    else if (Line >= StartLine + Length) {
      Length = Line - StartLine + 1;
    }
  }

};

class CodeFormatter {
public:
  struct Options {
    bool UseTabs = false;
    unsigned IndentWidth = 4;
    unsigned TabWidth = 4;
  };

private:
  const Options &FmtOptions;
  SourceManager &SM;
  SourceFile &SF;

  size_t getTrimmedLineOffset(unsigned LineIndex);
  size_t getExpandedIndentForLine(unsigned LineIndex);
  StringRef getTrimmedTextForLine(unsigned LineIndex);

  std::pair<LineRange,std::string> indent(unsigned LineIndex, FormatContext &FC);

public:
  CodeFormatter(const Options &FmtOptions, SourceFile &SF, SourceManager &SM)
    : FmtOptions(FmtOptions), SM(SM), SF(SF) { }

  StringRef getText(void);
  size_t getLineOffset(unsigned LineIndex);

  //===----------------------------------------------------------------------===//
  // Reformat
  //===----------------------------------------------------------------------===//
  /// Request a reformatting of the \p Range, using \p Options to determine the
  /// how the format should be applied to \p SF. \p SM is required to provide
  /// an ASTContext and other helper data.
  /// \returns a pair containing which line ranges where updated and a string
  /// containing the applied edits.
  std::pair<LineRange,std::string> reformat( LineRange Range );

};

  }
}

#endif // SWIFT_IDE_CODE_FORMATTING_H
