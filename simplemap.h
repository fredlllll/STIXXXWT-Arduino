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