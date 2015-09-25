/**********************************************************************************************
 * Arduino STIXXXWT Library - Version 1.0.0
 * by Frederik Gelder <frederik.gelder@freenet.de>
 *
 * Copyright 2015 Frederik Gelder
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **********************************************************************************************/
 
#ifndef TEXT_h
#define TEXT_h
 
int8_t convertFromUTF8ToUTF16Step(char * &source, uint16_t *destination){ //returns bytes that were written to destination (BYTES not WORDS)
    char a = *(source++);
    uint32_t codepoint;
    if(a == 0){ //0 terminated
        return 0;
    }else if((a&0x80)==0){ //1 byte
        codepoint = a;
    }else if((a&0xE0)==0xC0){ //2 byte
        codepoint = (a&0x1F)<<6;
        a= *(source++);
        codepoint |= (a&0x3F);
    }else if((a&0xF0)==0xE0){ //3 byte
        codepoint = (a&0x0F)<<12;
        a= *(source++);
        codepoint |= (a&0x3F)<<6;
        a= *(source++);
        codepoint |= (a&0x3F);
    }else if((a&0xF8)==0xF0){ //4 byte
        codepoint = (a&0x07)<<18;
        a= *(source++);
        codepoint |= (a&0x3F)<<12;
        a= *(source++);
        codepoint |= (a&0x3F)<<6;
        a= *(source++);
        codepoint |= (a&0x3F);
    }else{
        return -1;//invalid byte. this shouldnt happen with compiler generated utf8
    }
    //convert to utf16;
    if(codepoint <= 0xD7FF){
        *destination = codepoint;
        return 2;
    }else if(codepoint >= 0x10000){
        *destination = (codepoint >>10)+0xD800;
        destination++;
        *destination = (codepoint & 0x3FF)+0xDC00;
        return 4;
    }
    return -2; //no clue if possible to get here, but here u have your errorcode if you do. means that utf8 could be parsed, but not converted to utf16
}
 
void convertFromUTF8ToUTF16(char *source, uint16_t *destination, uint32_t *destinationByteLen){
    uint32_t desti = 0;
    uint32_t destinationMaxLen = *destinationByteLen;
    while(*source && desti< destinationMaxLen){
    //for(int i =0; desti < destinationMaxLen; i++){
        char a = *source;//source[i];
        uint32_t codepoint;
        if(a == 0){
            break; //0 terminated (i hope?)
        }else if((a&0x80)==0){ //1 byte
            codepoint = a;
        }else if((a&0xE0)==0xC0){ //2 byte
            codepoint = (a&0x1F)<<6;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F);
        }else if((a&0xF0)==0xE0){ //3 byte
            codepoint = (a&0x0F)<<12;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F)<<6;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F);
        }else if((a&0xF8)==0xF0){ //4 byte
            codepoint = (a&0x07)<<18;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F)<<12;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F)<<6;
            //a = source[++i];
            a= *(source++);
            codepoint |= (a&0x3F);
        }else{
            //invalid byte. just ignore. even though this shouldnt happen with compiler generated utf8
            continue;
        }
        //convert to utf16 and put in destination
        uint16_t *dst = (uint16_t*)((char*)destination)+desti;
        if(codepoint <= 0xD7FF){
            *dst = codepoint;
            desti+=2;
        }else if(codepoint >= 0x10000){
            codepoint -= 0x10000;
            uint16_t first = (codepoint >>10)+0xD800;
            uint16_t second = (codepoint & 0x3FF)+0xDC00;
            *dst = first;
            desti+=2;
            if(desti >= destinationMaxLen){ //dont write out only one half, when next unit doesnt fit
                *dst = 0;
                desti-=2;
                break;
            }
            dst++;
            *dst = second;
            desti+=2;
        }
    }
    *destinationByteLen = desti;
}

uint32_t strlenUTF8(char* str){
    uint32_t len = 0;
    while(*str){
        char a = *str;
        if((a&0x80)==0){ //1 byte
            len++;
            str++;
        }else if((a&0xE0)==0xC0){ //2 byte
            len++;
            str+=2;
        }else if((a&0xF0)==0xE0){ //3 byte
            len++;
            str+=3;
        }else if((a&0xF8)==0xF0){ //4 byte
            len++;
            str+=4;
        }else{
            //invalid byte. just ignore. even though this shouldnt happen with compiler generated utf8
            str++;
            continue;
        }
    }
    return len;
}

uint32_t UTF16BytesNeededForUTF8String(char* utf8str){
    uint32_t needed = 2;//2 for 16 bit null termination
    while(*utf8str){
        char a = *utf8str;
        uint32_t codepoint;
        if((a&0x80)==0){ //1 byte
            codepoint = a;
        }else if((a&0xE0)==0xC0){ //2 byte
            codepoint = (a&0x1F)<<6;
            a= *(utf8str++);
            codepoint |= (a&0x3F);
        }else if((a&0xF0)==0xE0){ //3 byte
            codepoint = (a&0x0F)<<12;
            a= *(utf8str++);
            codepoint |= (a&0x3F)<<6;
            a= *(utf8str++);
            codepoint |= (a&0x3F);
        }else if((a&0xF8)==0xF0){ //4 byte
            codepoint = (a&0x07)<<18;
            a= *(utf8str++);
            codepoint |= (a&0x3F)<<12;
            a= *(utf8str++);
            codepoint |= (a&0x3F)<<6;
            a= *(utf8str++);
            codepoint |= (a&0x3F);
        }else{
            //invalid byte. just ignore. even though this shouldnt happen with compiler generated utf8
            continue;
        }
        if(codepoint <= 0xD7FF){
            needed+=2;
        }else if(codepoint >= 0x10000){
            needed+=4;
        }
    }
    return needed;
}

#endif