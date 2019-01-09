/***************************************************************************//**
 * @file
 *
 * @mainpage Hash Class
 *
 * @section course_section Course Information
 *
 * @author Ryan Hinrichs, Reed Harmon, Caleb Kaselnak
 *
 * @par Course:
 *      CSC 300 - M001 - 10:00am
 *
 * @par Location:
 *      McLaury - 205
 *
 * @details 
 * This program reads in a file with data and then takes the data and splits
 * it into keys and values and then stores it inside a hash table and keeps
 * track of multiple aspects of the key and value pair that has been inserted
 * such as if the bucket is empty, deleted or active, how many times the specific
 * bucket has been probed by the insert, and what the value and key are for the
 * bucket in the table.
 *
 * To do this we generate a Hash of a vector of type bucket which is a struct
 * holding different types of data. We then have multiple functions that the
 * Hash can utilize such as an insert, remove, find and rehash which are the 
 * the main ones that are utilized. The size of the vector can either be put 
 * in by the user or runs through an array of different set primes for rehashing
 * purposes. The insert reads in a key and a value from the input file and then
 * generates a hashcode to find the bucket it wants to insert it in and then tests
 * to see if the bucket is empty, deleted or active and reacts appropriately. If
 * the first hashcode isn't successful it generates a second hashcode and tests
 * the insert with that and if that fails just continually linearly probes until
 * key and value an empty or deleted bucket is found in the vector and inserts 
 * there. Once a is inserted it chages the properties of the bucket to be active,  
 * increments timesprobed and saves the key and value. If a bucket is active and 
 * it increments the timesprobed value. It can then search and find or remove 
 * keys. If the key is removed the buckets status is changed to deleted so when
 * probed again it acts like a empty bucket and writes over the previous data.
 * 
 *
 ******************************************************************************/

#ifndef _HASH_H_
#define _HASH_H_

#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;

template<typename K, typename V>
class Hash
{
   public:
      Hash();                                    // zero parameter constructor
      Hash(int init_size);                    // one parameter constructor
     ~Hash();                                    // destructor

      V& operator[](const K& Key);               // overloaded []
      V& insert(const K& Key, V& Value);         // insert
      V find(const K& Key);                      // find a key
      int size();                             // current size
      double loadFactor(bool rehash);                       // load factor
      int hashCodeGenerated(int hashCode);       // count of time hashCode was generated
      int* bucketProbed();                       // frequency of a hash code
      void remove(const K& Key);                 // remove a key from hash
      int hashcode(string k);
      int hashcode(int k);
      int hash2(string k);
      int hash2(int k);
      enum status { empty, active, deleted };   //implementing state that bucket is in
      void rehash();  
      int findMax();
      int timesprobed(int index);
      string getkey(int index); 
      void rectSize(float &x, float &y);       

   private:
   	  struct bucket
   	  {
      K key;							     // implementation dependent
      V value;
      status info = empty;
      int timesprobed = 0;
      };
      
      long long primes[20]={521, 1279, 3217, 9689, 19937, 44497, 
      110503, 216091, 756839, 1398269, 3021377, 6972593, 13466917, 
      30402457, 74207281, 160481219, 334214467, 674506111, 
      1400305337, 2902958801}; 
      long long Tablesize;
      
      vector<bucket> myhash;
      vector<int>generated;
      
};

#endif

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Constructor that instantiates the table at the size of the first value in the
 * array holding given hash table sizes.
 *  
 *
 *****************************************************************************/
template<typename K, typename V>
Hash<K,V> :: Hash()
{
	static int i = 0;
	//changes hash size to the value in an index in primes array
	//changes tablesize to that same value too
	myhash.resize(primes[i]);
	generated.resize(primes[i], 0);
	
	Tablesize = primes[i];
	
	return;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Constructor that takes the desired size of the hash table and makes the table
 * that size.
 *
 * @param[in] init_size - desired initial size of the hash table
 *  
 *
 *****************************************************************************/
template<typename K, typename V>
Hash<K,V> :: Hash(int init_size)
{
	//resize hash to desired size and set Tablesize to that value also
	myhash.resize(init_size);
	generated.resize(init_size, 0);
	Tablesize = init_size;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Destructor that clears the hash table of all of its contents.
 *  
 *
 *****************************************************************************/
template<typename K, typename V>
Hash<K,V> :: ~Hash()
{
	myhash.clear();
}
	
/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Finds the number of times that a specific Hash Code has been generated
 *  
 *
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V> :: hashCodeGenerated(int hashCode)
{
	generated[hashCode]++;
	return generated[hashCode];

}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Function that goes through the hash and finds the bucket that has been probed
 * the most. This is done by checking the timesprobed value of each bucket. 
 *  
 * @param[out] index - index of bucket that has the most probes
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V> :: findMax()
{
    unsigned int i;
    int max =0;
    int index;
    
    for( i=0; i<myhash.capacity(); i++ )
    {
        if ( myhash[i].timesprobed > max )
        {
            max = myhash[i].timesprobed;
            index = i;
        }
     //   cout << "Index " << i << " Timesprobbed " << myhash[i].timesprobed << " Max is " <<max << endl;
    }
    
    return index;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Rehash function that resizes the current hash table. This is done by taking 
 * the current table and moving it to a temp hash table and resizing the original
 * hash table. Then, the values from the temp table are put back into the original
 * hash table
 *  
 * @returns void
 *****************************************************************************/
template<typename K, typename V>
void Hash<K,V> :: rehash()
{
	int Tablesize = size();
	unsigned int i = 0;
	K key;
	V value;
	loadFactor(true);
	vector<bucket> temphash;
	

	//make temp hash to hold original hash and clear my hahs
	temphash.reserve(Tablesize);
    	temphash = myhash;
    	myhash.clear();


	//resize original hash table
	while(primes[i] <= Tablesize)
	{
	i++;
	}
	myhash.resize(primes[i]);


	//set all values in resized hash to default
	for(int j = 0; j< size(); j++)
	{
      	    this->myhash[j].info = empty;
      	    this->myhash[j].timesprobed = 0;
	}

	cout << "Rehashing Table to size " << size() << endl;

	//put values from temp hash back into resized original hash
	for(i =0; i<temphash.size();i++)
	{
		key = temphash[i].key;
		value = temphash[i].value;
		insert(key, value);
	}
	temphash.clear();

	return;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Keeps track of the size of the table.
 *  
 * @returns Tablesize - size of hash table
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V> :: size()
{
	return myhash.capacity();
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Counts the amount of entries there are inside the table and then tests
 * that against the actual tablesize to discover how full it is.
 *  
 * @returns load - percentage of how full table is
 *****************************************************************************/
template<typename K, typename V>
double Hash<K,V> :: loadFactor(bool rehash)
{
	static int numentries = 0;
	double load;
	long long tsize = size();


	//incrememnt number of entries in hash
	numentries++;
	
	//if hash gets rehashed, reset number of entries
	if(rehash)
		numentries = 0;
	
	//calculate loadfactor
	load = (double)numentries/(double)tsize;
	
	
	return load;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Reads in a key and a value and then tests what the loadfactor is and if
 * it's greater than .75 then it calls the rehashing function before
 * continuing. Once through the load testing it then using the key in first hash
 * to find the first index to test and then runs the key through the second
 * hash to find the step size for traversing the table if first searched 
 * bucket is not empty. If the bucket is not registered empty or deleted then
 * the timesprobed count of the bucket is incremented and the index is moved to
 * the next location and retried until empty or deleted is found. Once an empty
 * or deleted bucket is found then the bucket is updated with the value, is probed,
 * as active and timesprobbed incremented and then the value is returned.
 *
 * @param[in] Key - key being used to find index location of hash table for insert
 * @param[in] Value - item being inserted into the bucket in hash table
 *
 * @returns myhash(pos).value - value that was inserted into hash table
 *****************************************************************************/
template<typename K, typename V>
V& Hash<K,V>::insert(const K& Key, V& Value)
{
    int pos, step;
    double load;
    bucket temp;
    static string blank = "Space";

    //get and check the load factor to see if it needs to be rehashed
    load = loadFactor(false);
    if( load > 0.75 )
    {
        rehash();
    }


    //get indicies for double hashing
    pos = hashcode(Key);
    step = hash2(Key);
    

    //go through first hash, check if able ot insert
    //go through second hash in first full, check if able to insert
    //linear probe first hash if all else fails and insert
    if(pos == -1)
    {
    	return blank;
    }
    else if( myhash[pos].info == empty || myhash[pos].info == deleted 
    		|| myhash[pos].key == Key )
    {    		
    	myhash[pos].timesprobed++;
		myhash[pos].key = Key;
  		myhash[pos].value = Value;
  	 	myhash[pos].info = active;
		
   	}
   	else if( myhash[step].info == empty || myhash[step].info == deleted 
    		|| myhash[step].key == Key )
    {
       	myhash[step].timesprobed++;
		myhash[step].key = Key;
  		myhash[step].value = Value;
  	 	myhash[step].info = active;
		
   	}
   	else
   	{
   		while( myhash[pos].info != empty && myhash[pos].info != deleted 
    		&& myhash[pos].key != Key )
    		{
    		myhash[pos].timesprobed++;
    		pos ++;
    		pos = pos % size();
    		}
    		
   			myhash[pos].key = Key;
  			myhash[pos].value = Value;
  	 		myhash[pos].info = active;
   	}

    return myhash[pos].value;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Function takes the given key and checks to see if there is an index inside
 * the hash table with a matching key. This is done by taking the given key, 
 * running through the hash code and the second hash code to generate the needed
 * indices, and searching through the hash table. As long as the current bucket
 * is not empty or deleted, check if the key in the bucket matches the given key.
 * Once there are no more active buckets, the function will return 0 and states the
 * key was not found.
 *
 * @param[in] Key - key being used to find index location in hash table
 *
 * @returns myhash(pos).value - value of bucket with matching key
 * @returns 0	- index could not be found inside of the table
 *****************************************************************************/
template<typename K, typename V>
V Hash<K,V>::find(const K& Key)
{
    int pos, step;

    //get indicies for double hashing
    pos = hashcode(Key);
    step = hash2(Key);
    
    //go through first hash, see if bucket is there
    //otherwise go through next hash and check
    //finally linear probe through first hash and check
    if( myhash[pos].info == active && myhash[pos].key == Key )
    {    		
    	return myhash[pos].value;
    }
    else if( myhash[step].info == active && myhash[step].key == Key )
    {
       	return myhash[step].value;
    }
    else
    {
   	while( myhash[pos].info != empty && 
   		!(myhash[pos].info == active && myhash[pos].key == Key))
    	{
    		pos++;
    		pos = pos % size();
    	}
    		
	if(myhash[pos].info != empty)
	{
   		return (myhash[pos].value);
	}
    }
    

    //if not found
    cout << "Key not found" << endl;
    return 0;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Overloaded [] operator function that calls the find function and returns
 * the value of the bucket with the matching key to the given key. 
 *
 * @param[in] Key - key being used to find index location in hash table
 *
 * @returns myhash(pos).value - value of bucket with matching key
 *****************************************************************************/
template<typename K, typename V>
V& Hash<K,V>::operator[](const K& Key)
{
    //call find and return value
    V val = find(Key);
    
    return val;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Function takes the given key and checks to see if there is an index inside
 * the hash table with a matching key and deletes the bucket. This is done by 
 * taking the given key, run it through the hash codes, check each index that 
 * is considered filled, and check if the key at the index matches the given key.
 * if the keys match, then the bucket is changed to empty, but the value in the
 * index is not removed.
 *
 * @param[in] Key - key being used to find index location in hash table
 *
 * @returns void
 *****************************************************************************/
template<typename K, typename V>
void Hash<K,V>::remove(const K& Key)
{
    int pos, step;
    pos = hashcode(Key);
    step = hash2(Key);

    if( myhash[pos].info == active && myhash[pos].key == Key )
    {    		
    	myhash[pos].info = deleted;
    	return;
   	}
   	else if( myhash[step].info == active && myhash[step].key == Key )
    {
       	myhash[step].info = deleted;
   	}
   	else
   	{
   		while( myhash[pos].info != empty && 
   			!(myhash[pos].info == active && myhash[pos].key == Key))
    		{
    		pos++;
    		pos = pos % size();
    		}
    		
		if(myhash[pos].info != empty)
		{
   			myhash[pos].info = deleted;
		}
   	}
}



/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * This function returns a vector of ints that contains the values of the 
 * times probed of each bucket in the heap. This is done by looping through the 
 * hash and checking each bucket and recording its times 
 *  
 * @returns ProbedStuff - vector containg ints of timesprobed values
 *****************************************************************************/
template<typename K, typename V>
int*Hash<K,V> :: bucketProbed()
{
	vector<int> ProbedStuff;
	
	for(int i = 0; i < this->size(); i++)
	{
		ProbedStuff[i] = myhash[i].timesprobed;
	}

    return ProbedStuff;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description: Hashcode found online based around the idea of shifting
 * a variety of spaces to creat a unique code, utilizing an integer key.
 * 
 * @param[in] m - key for generating hashcode
 *
 * @returns hash - hashcode generated
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V>:: hashcode(int m)
{
	unsigned long hash = 5441;
	const char *k;
	string l;
	int c = 0;	
	l = to_string(m);
	k = l.c_str();

    while (c < l.size())
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
        c++;
    }

   	hash = hash % size();
   	
    if(l.size() == 0)
	hash = -1;	
  
   return hash;

}

/**************************************************************************//**
 * @author 
 *
 * @par Description: Hashcode found online based around the idea of shifting
 * a variety of spaces to creat a unique code, utilizing a string key.
 * 
 * @param[in] l - key for generating hashcode
 *
 * @returns hash - hashcode generated
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V>:: hashcode(string l)
{
	unsigned long hash = 11;
	unsigned int c = 0;	

    while (c < l.size())
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
        c++;
    }

   	hash = hash % size();
   	
    if(l.size() == 0)
	hash = -1;	
  
   return hash;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Reads in a key and then runs it through the hashcode to generate an index
 * which is then returned to be used to as the amount of buckets ahead you step
 * through the hashtable while traversing.
 *  
 * @param[in] l - key to be hashed
 * 
 * @returns hash - value to be utilized for placing bucket
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V>:: hash2(string l)
{
	unsigned long hash = 5381;
	unsigned int c = 0;	


    while (c < l.size())
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c++;
    }
        
    hash = hash % size();	

    return hash;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Reads in a key and then runs it through the hashcode to generate an index
 * which is then returned to be used to as the amount of buckets ahead you step
 * through the hashtable while traversing.
 *  
 * @param[in] m - key for generating hashcode
 *
 * @returns hash - hashcode generated
 *****************************************************************************/
template<typename K, typename V>
int Hash<K,V>:: hash2(int m)
{
	unsigned long hash = 3271;
	const char *k;
	string l;
	int c = 0;	
	l = to_string(m);
	k = l.c_str();

    while (c < l.size())
    {
         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c++;
    }

   	hash = hash % size();
   	
    if(l.size() == 0)
	hash = -1;	
  
   return hash;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Takes in an index given to find the number of times probed for the specific
 * location.
 *  
 * @param[in] index - location of the bucket in question
 *
 * @returns probe - value of times probed for specific bucket
 *****************************************************************************/
template<typename K, typename V>  
int Hash<K,V>:: timesprobed(int index)
{
	int probe;
	probe = myhash[index].timesprobed;
	return probe;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Takes in an index given to find the key for the specific location.
 *  
 * @param[in] index - location of the bucket in question
 *
 * @returns probe - key for specific bucket
 *****************************************************************************/
template<typename K, typename V>  
string Hash<K,V>:: getkey(int index)
{
	string probe;
	probe = myhash[index].key;
	return probe;
}

/**************************************************************************//**
 * @author 
 *
 * @par Description:
 * Function that calculates the size of our gridded rectangle to be used by
 * the visualizer.
 *  
 * @param[in] x - x-coordinate to manipulate for columns.
 * @param[in] y - y-coordinate to manipulate for rows.
 *
 * @returns void
 *****************************************************************************/
template<typename K, typename V>
void Hash<K,V>::rectSize(float &x, float &y)
{
    int cap = 521;
    int h = 500;
    int w = 1000;    
    int cols, rows;

    cols = cap / 10;
    rows = cap / cols + 1;

    x = (float) (w / cols);
    y = (float) (h / rows);
    
    return;
}
