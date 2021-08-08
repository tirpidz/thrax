#include <string>
#include <vector>

#include <fst/fstlib.h>
#include <fst/string.h>
#include <thrax/symbols.h>

namespace thrax {
namespace function {

// Needed for symbol table functionality built into symbols.h

SymbolTableBuilder kSymbolTableBuilder;

fst::SymbolTable* GetByteSymbolTable() {
  return kSymbolTableBuilder.GetByteSymbolTable();
}

fst::SymbolTable* GetUtf8SymbolTable() {
  return kSymbolTableBuilder.GetUtf8SymbolTable();
}

void AddToByteSymbolTable(string symbol, int64 label) {
  kSymbolTableBuilder.AddToByteSymbolTable(symbol, label);
}

void AddToUtf8SymbolTable(string symbol, int64 label) {
  kSymbolTableBuilder.AddToUtf8SymbolTable(symbol, label);
}

inline bool IsUnicodeSpaceOrControl(int c) {
  // Replicates functionality in the ICU library for determining whether the
  // character type is control or whitespace. Specifically:
  //
  // if (u_charType(c) == U_CONTROL_CHAR ||
  //     u_hasBinaryProperty(c, UCHAR_WHITE_SPACE) ||
  //     u_hasBinaryProperty(c, UCHAR_POSIX_BLANK))
  //
  switch (c) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 127:
    case 128:
    case 129:
    case 130:
    case 131:
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 137:
    case 138:
    case 139:
    case 140:
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 146:
    case 147:
    case 148:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 5760:
    case 6158:
    case 8192:
    case 8193:
    case 8194:
    case 8195:
    case 8196:
    case 8197:
    case 8198:
    case 8199:
    case 8200:
    case 8201:
    case 8202:
    case 8232:
    case 8233:
    case 8239:
    case 8287:
    case 12288:
      return true;
    default:
      return false;
  }
}

SymbolTableBuilder::SymbolTableBuilder()
    : byte_symbols_(nullptr), utf8_symbols_(nullptr) {}

SymbolTableBuilder::~SymbolTableBuilder() {
  delete byte_symbols_;
  delete utf8_symbols_;
}

fst::SymbolTable* SymbolTableBuilder::GetByteSymbolTable() {
  if (!byte_symbols_) GenerateByteSymbolTable();
  return byte_symbols_;
}

fst::SymbolTable* SymbolTableBuilder::GetUtf8SymbolTable() {
  if (!utf8_symbols_) GenerateUtf8SymbolTable();
  return utf8_symbols_;
}

void SymbolTableBuilder::AddToByteSymbolTable(string symbol, int64 label) {
  if (!byte_symbols_) return;
  byte_symbols_->AddSymbol(symbol, label);
}

void SymbolTableBuilder::AddToUtf8SymbolTable(string symbol, int64 label) {
  if (!utf8_symbols_) return;
  utf8_symbols_->AddSymbol(symbol, label);
}

void SymbolTableBuilder::GenerateByteSymbolTable() {
  fst::MutexLock lock(&map_mutex_);
  byte_symbols_ = new fst::SymbolTable(kByteSymbolTableName);
  byte_symbols_->AddSymbol("<epsilon>", 0);
  char c_str[5];
  for (int c = 1; c < 256; ++c) {
    if (c > 32 && c < 127) {
      c_str[0] = static_cast<char>(c);
      c_str[1] = '\0';
    } else {
      snprintf(c_str, sizeof(c_str), "0x%02x", c);
    }
    byte_symbols_->AddSymbol(string(c_str), c);
  }
}

void SymbolTableBuilder::GenerateUtf8SymbolTable() {
  fst::MutexLock lock(&map_mutex_);
  utf8_symbols_ = new fst::SymbolTable(kUtf8SymbolTableName);
  utf8_symbols_->AddSymbol("<epsilon>", 0);
  for (int c = 1; c < 0x10000; ++c) {
    std::vector<int> labels;
    labels.push_back(c);
    string utf8_label;
    if (fst::LabelsToUTF8String(labels, &utf8_label)) {
      if (IsUnicodeSpaceOrControl(c)) {
        char c_str[7];
        snprintf(c_str, sizeof(c_str), "0x%04x", c);
        utf8_symbols_->AddSymbol(string(c_str), c);
      } else {
        utf8_symbols_->AddSymbol(utf8_label, c);
      }
    }
  }
}

}  // namespace function
}  // namespace thrax
