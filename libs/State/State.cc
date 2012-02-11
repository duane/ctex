#include <State/State.h>

using namespace tex;

State::State() {
  // initialize the CS table.
  cs_entries = 0;
  cs_size = CS_TABLE_SIZE;
  cs_table = new CommandSequenceEntry*[cs_size];
  // initialize empty entries.
  bzero(cs_table, cs_size * sizeof(CommandSequence*));
  
  // first initialize ccode to all "CC_OTHER_CHAR" as the default code.
  for (unsigned i = 0; i < 128; i++) {
    ccode[i] = CC_OTHER_CHAR;
  }
  // now initialize letters.
  for (unsigned i = 0; i < 26; i++) {
    ccode['A' + i] = CC_LETTER;
    ccode['a' + i] = CC_LETTER;
  }
  
  // now misc. initializations.
  ccode[0x00] = CC_IGNORE;
  ccode[0x20] = CC_SPACER; // ' ' 
  ccode[0x5C] = CC_ESCAPE; // '\\'
  ccode[0x25] = CC_COMMENT; // '%'
  ccode[0x7F] = CC_INVALID;
  ccode[0x0A] = CC_CAR_RET; // '\n'; technically deviates from tex.
  ccode[0x0D] = CC_CAR_RET; // '\r'
}

CommandSequenceEntry *State::find_cs(UString &string, CommandSequenceEntry ***referenced_by) const {
  uint32_t hash = string.hash();
  uint32_t bucket = hash % cs_size;
  CommandSequenceEntry **prev = &cs_table[bucket], *entry;
  while ((entry = *prev)) {
    if (string.equalq(entry->cs.string)) {
      *referenced_by = prev;
      return entry;
    }
    prev = &entry->next;
  }
  return NULL;
}

void State::set(CommandSequence &cs) {
  CommandSequenceEntry **reference;
  CommandSequenceEntry *entry = find_cs(cs.string, &reference);

  if (entry) {
    entry->cs = cs;
    return;
  }
  
  // nothing's there, we need to add one.
  cs_entries += 1;
  entry = new CommandSequenceEntry;
  entry->cs = cs;
  entry->next = NULL;
  *reference = entry;
}

CommandSequence *State::get(UString &string) {
  CommandSequenceEntry **reference;
  CommandSequenceEntry *entry = find_cs(string, &reference);
  if (entry)
    return &entry->cs;
  return NULL;
}

State::~State() {
  // free the hash table.
  for (size_t i = 0; i < cs_size; i++) {
    CommandSequenceEntry *entry = cs_table[i];
    while (entry) {
      CommandSequenceEntry *next = entry->next;
      delete entry;
      entry = next;
    }
  }
  delete cs_table;
}