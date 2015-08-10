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

#ifndef simplemap_h
#define simplemap_h

//this is a super simple quick and dirty map. 
template<typename K,typename V> class simplemap{
    private:
        int count;

        template<typename pK,typename pV> struct pair{
            pK key;
            pV value;
        };
        int listSize;
        pair<K,V> *list;
        
        void increaseSize(int count){
            pair<K,V> *newList = new pair<K,V>[listSize+count];
            for (int i =0; i< listSize; i++){
                newList[i] = list[i];
            }
            if(list != 0){
                delete[] list;
            }
            list = newList;
            listSize += count;
        }
        
        void decreaseSize(int count){
            if(listSize > 0){
                if(count > listSize){
                    count = listSize;
                }
                pair<K,V> *newList = new pair<K,V>[listSize-count];
                for (int i =0; i< listSize-count; i++){
                    newList[i] = list[i];
                }
                if(list != 0){
                    delete[] list;
                }
                list = newList;
                listSize -= count;
            }
        }
    public:
        void set(K key,V value){
            for(int i = 0; i< count; i++){
                if(list[i].key == key){
                    list[i].value = value;
                    return;
                }
            }
            if(count >= listSize){
                increaseSize(5);
            }
            list[count].key = key;
            list[count++].value = value;
        }
        
        V get(K key){
            for(int i = 0; i< count; i++){
                if(list[i].key == key){
                    return list[i].value;
                }
            }
            return (V)0;
        }
};

#endif