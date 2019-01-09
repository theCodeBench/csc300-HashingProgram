#include "Hash.h"
/*#define mix(a,b,c) \
{ \
    a -= b; a -= c; a ^= (c >> 13); \
    b -= c; b -= a; b ^= (a << 8); \
    c -= a; c -= b; c ^= (b >> 13); \
    a -= b; a -= c; a ^= (c >> 12); \
    b -= c; b -= a; b ^= (a << 16); \
    c -= a; c -= b; c ^= (b >> 5); \
    a -= b; a -= c; a ^= (c >> 3); \
    b -= c; b -= a; b ^= (a << 10); \
    c -= a; c -= b; c ^= (b >> 15); \
}*/

template<typename K, typename V>
Hash<K,V> :: Hash()
{
	static int i = 0;
	
	myhash.resize(primes[i]);
	
	Tablesize = primes[i];
	
	i++;
	
	return;
}

template<typename K, typename V>
Hash<K,V> :: Hash(int init_size)
{
	static int i = 0;

	myhash.resize(init_size);
}

template<typename K, typename V>
Hash<K,V> :: ~Hash()
{
	myhash.clear();
}	

template<typename K, typename V>
void Hash<K,V> :: rehash()
{
	int Tablesize = size();
	int i = 0;
	K key;
	V value;
	V& success;
	vector<bucket> newhash;
	
	newhash = myhash;
	
	while(primes[i] < Tablesize)
	i++;
	
	Hash(primes[i]);
	
	for(i =0; i<newhash.size();i++)
	{
		key = newhash[i].key;
		value = newhash[i].value;
		
		success = myhash.insert(key, value);
	}

	return;
}

template<typename K, typename V>
int Hash<K,V> :: size()
{
	return Tablesize;
}

template<typename K, typename V>
double Hash<K,V> :: loadFactor()
{
	static int numentries = 0;
	double load;
	numentries++;
	
	long long tsize = size();
	
	load = (double)numentries/(double)tsize;
	
	return load;
}

template<typename K, typename V>
V& Hash<K,V>::insert(const K& Key, V& Value)
{
    int pos, step;
    double load;

    load = loadFactor();
    if( load > 0.75 )
    {
        rehash();
    }

    pos = hashcode(Key);
    step = hash2(Key);

    if( myhash(pos).info != empty || myhash(pos).info != deleted )
    {
        myhash(pos).timesprobed++;
        pos = pos + step;
        pos = pos % size();
    }

    else
    {
        myhash(pos).value = Value;
        myhash(pos).info = active;
        myhash(pos).timesprobed++;
        myhash(pos).probed = true;
    }

    return myhash(pos).value;
}

template<typename K, typename V>
V Hash<K,V>::find(const K& Key)
{
    int pos, step;
    pos = hashcode(Key);
    step = hash2(Key);

    while ( myhash(pos).info != empty || myhash(pos).info != deleted )
    {
        if ( myhash(pos).key == Key )
            return myhash(pos).value;

        pos = pos + step;
        pos = pos % size();
    }
    
    cout << "Key not found" << endl;
    return 0;
}

template<typename K, typename V>
V& Hash<K,V>::operator[](const K& Key)
{
    V val = find(Key);
    
    return val;
}

template<typename K, typename V>
void Hash<K,V>::remove(const K& Key)
{
    int pos, step;
    pos = hashcode(Key);
    step = hash2(Key);

    while ( myhash(pos).info != empty || myhash(pos).info != deleted )
    {
        if ( myhash(pos).key == Key )
            myhash(pos).info = deleted;
        
        pos = pos + step;
        pos = pos % size();
    }
}

template<typename K, typename V>
int Hash<K,V>:: hashcode(int k)
{
  	int a = 0x9e3779b9;
  	int b, c;
  	
  	int len;

    a += (k + ((unsigned)k << 8) + ((unsigned)k<< 16) + ((unsigned)k<< 24));
	
	c = a % 13 + ((unsigned)k << 3) + a * 179;
	
	b = c;
	
    c += 13;
    
    len = c % 11;
    len++;

    switch (len)
    {
    case 11: c += ((unsigned)k<< 24);
    case 10: c += ((unsigned)k<< 16);
    case 9: c += ((unsigned)k<< 8);
        /* First byte of c reserved for length */
    case 8: b += ((unsigned)k<< 24);
    case 7: b += ((unsigned)k<< 16);
    case 6: b += ((unsigned)k<< 8);
    case 5: b += k;
    case 4: a += ((unsigned)k<< 24);
    case 3: a += ((unsigned)k<< 16);
    case 2: a += ((unsigned)k<< 8);
    case 1: a += k;
    }

    //mix(a, b, c);
	
	c = c % size();	
	
    return c;
}


template<typename K, typename V>
int Hash<K,V>:: hash2(char const *k)
{
	int index = 0;
	
	for(int i = 0; i < strlen(k); i++)
	{
	if(i % 2 == 0)
	index += k[i]<<13;
	
	if(i % 3 == 0)
	index += k[i] * 37;
	
	if(i % 5 == 0)
	index -= k[i] % 11;	
	}
	
	index = index % size();
	
	return index;

}

template<typename K, typename V>
int Hash<K,V>:: hash2(int k)
{
	int index = (k>>11)- (k* 6173) - (k % 3467);
	index += (k<<7);
	
	index = index % size();
	
	return index;
}
      
