/*
    Copyright 2010 Duane R. Bailey
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
    UString is a standard, immutable string of unichars.
*/

#ifndef __INCLUDE_USTRING_H__
#define __INCLUDE_USTRING_H__

#include <Unicode/Unicode.h>
#include <stdio.h>

namespace tex {

class UString {
public:
    UString(unichar *uchar, size_t len) {
        assert(uchar != NULL);
        length = len;
        raw = new unichar[length];
        memcpy(raw, uchar, length * sizeof(unichar));
    }
    
    static UString *FromCString(const char *cstr);
    
    ~UString() {
        delete raw;
    }

    unichar get(size_t i) {
        if (i >= length)
            return 0x0;
        return raw[i];
    }

    unichar *getRaw() {return raw;}
    size_t getAllocated() {return allocated * sizeof(unichar);}
    size_t getLength() {return length;}
protected:
    UString() {}
    unichar *raw;
    size_t length, size, allocated;
};

class MutableUString : public UString {
public:
    MutableUString() {
        allocated = 16;
        raw = new unichar[allocated];
        length = 0;
    }
    explicit MutableUString(UString *string) {
        // copy the data
        allocated = length = string->getLength();
        raw = new unichar[length];
        memcpy(raw, string->getRaw(), allocated * sizeof(unichar));
    }
    
    explicit MutableUString(size_t initialAllocation) {
        allocated = initialAllocation;
        raw = new unichar[allocated];
        length = 0;
    }
       
    void set(size_t index, unichar uchar) {
        if (index >= length)
            return;
        raw[index] = uchar;
    }
    
    void add(unichar uchar) {
        if (length >= allocated) {
            unichar *temp = new unichar[allocated * 2];
            memcpy(temp, raw, allocated * sizeof(unichar));
            delete raw;
            raw = temp;
            allocated = allocated * 2;
        }
        raw[length++] = uchar;
    }
};

};  // namespace tex

#endif  // __INCLUDE_USTRING_H__
