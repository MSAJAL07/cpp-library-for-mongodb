# cpp-library-for-mongodb
Using this library you can perform CRUD operations on the MongoDB database in c++

if you want to build mongo driver visit this link :

https://github.com/linux-on-ibm-z/docs/wiki/Building-C-MongoDB-Driver

Using my library you can write code like this :
 
    includes/mongocpp.h
```
 #include<mongoc.h>
class BsonObject
{
private :
bson_t * doc;
public:
BsonObject();
void setBson(bson_t * doc);
bson_t * get();
void close();
};

class BsonObjectBuilder
{
private:
BsonObject * bsonObject;
bson_t * doc;
public:
BsonObjectBuilder();
void append(const char * key,int value);
void append(const char * key,const char * value);
void append(const char * key,bool value);
void append(const char * key,double value);
void append(const char * key,BsonObject * value);
BsonObject * getBsonObject();
};

class DBCursor
{
private:
mongoc_cursor_t * cursor;
const bson_t * doc;
bson_iter_t iterator;
public:
DBCursor();
DBCursor(mongoc_cursor_t * cursor);
void setCursor(mongoc_cursor_t *cursor);
bool hasNext();
char * next();
int getInt(const char * k);
const char * getString(const char * k);
bool getBool(const char * k);
double getDouble(const char * k);
void close();
};

class DBCollection
{
private:
mongoc_collection_t *collection;
public:
DBCollection(mongoc_client_t * client, const char * db, const char * collection);
void insert(BsonObject * bsonObject);
void update(BsonObject * selector,BsonObject * update);
void remove(BsonObject * selector);
DBCursor * find();
DBCursor * find(BsonObject * query);
DBCursor * find(BsonObject * query,BsonObject * opt);
void close();
};

class MongoClient
{
private:
mongoc_client_t *client;
DBCollection * collection;
public:
MongoClient(const char * url);
DBCollection * getCollection(const char * db, const char * collection);
void close();
};
 ```

   src/insert.cpp
   

```
#include<mongocpp.h>
#include<iostream>
using namespace std;
int main()
{
MongoClient mc("mongodb://localhost:27017");
DBCollection * collection=mc.getCollection("tmdb","students");
BsonObjectBuilder *bob= new BsonObjectBuilder();
bob->append("english",82);
bob->append("hindi",65);
bob->append("science",80);

BsonObject * bsonObject =bob->getBsonObject();

bob= new BsonObjectBuilder();
bob->append("_id",1001);
bob->append("firstName","sajal");
bob->append("lastName","mishra");
bob->append("markes",bsonObject);
bsonObject =bob->getBsonObject();
collection->insert(bsonObject);
bsonObject->close();
collection->close();
mc.close();
cout<<"done";
return 0;
}
```


   src/delete.cpp
   
```

#include<mongocpp.h>
#include<iostream>
using namespace std;
int main()
{
MongoClient mc("mongodb://localhost:27017");
DBCollection * collection=mc.getCollection("tmdb","students");
BsonObjectBuilder *bob= new BsonObjectBuilder();
bob->append("_id",1001);
BsonObject * selector =bob->getBsonObject();
bob= new BsonObjectBuilder();
collection->remove(selector);
selector->close();
collection->close();
mc.close();
cout<<"done";
return 0;
}find



```
    src/find.cpp
    
    
```

#include<mongocpp.h>
#include<iostream>
using namespace std;
int main()
{
MongoClient mc("mongodb://localhost:27017");
DBCollection * collection=mc.getCollection("tmdb","students");
DBCursor * cursor = collection->find();
while(cursor->hasNext())
{
printf("%s\n",cursor->next());
printf("id : %d  ",cursor->getInt("_id"));
printf("last name : %s   ",cursor->getString("lastName"));
printf("first name : %s   \n",cursor->getString("firstName"));
}
cursor->close();
printf("***************************************************************************************\n");

BsonObjectBuilder *bob = new BsonObjectBuilder();
bob->append("lastName","mishra");
BsonObject * selector =bob->getBsonObject();
cursor = collection->find(selector);

while(cursor->hasNext())
{
printf("%s\n",cursor->next());
printf("id : %d  ",cursor->getInt("_id"));
printf("first name : %s   ",cursor->getString("firstName"));
printf("last name : %s   \n",cursor->getString("lastName"));
}
cursor->close();
printf("***************************************************************************************\n");
bob= new BsonObjectBuilder();
bob->append("firstName",true);
bob->append("_id",false);
BsonObject * projection =bob->getBsonObject();
bob= new BsonObjectBuilder();
bob->append("firstName", -1);
BsonObject * sort =bob->getBsonObject();
bob= new BsonObjectBuilder();
bob->append("projection",projection);
bob->append("limit",5);
bob->append("sort",sort);
BsonObject * opt =bob->getBsonObject();
/*
opt={ 
	"projection":
		{
		"firstName":"BCON_BOOL(true),
		"_id":BCON_BOOL(false)
		},
	"limit": BCON_INT64 (5), 
	"sort":
		{
		"firstName":
		"BCON_INT32 (-1)		
		}
*/
//cout<<bson_as_json(opt->get(),NULL)<<'\n';
cursor = collection->find(selector,opt);
while(cursor->hasNext())
{
printf("%s\n",cursor->next());
}
cursor->close();
printf("***************************************************************************************\n");
cursor = collection->find(NULL,opt);
while(cursor->hasNext())
{
printf("%s\n",cursor->next());
}
printf("***************************************************************************************\n");
projection->close();
sort->close();
opt->close();
selector->close();
cursor->close();
collection->close();
mc.close();
cout<<"done";
return 0;
}



```

    src/update.cpp
    
    
```

#include<mongocpp.h>
#include<iostream>
using namespace std;
int main()
{
MongoClient mc("mongodb://localhost:27017");
DBCollection * collection=mc.getCollection("tmdb","students");
BsonObjectBuilder *bob= new BsonObjectBuilder();
bob->append("_id",1001);
BsonObject * selector =bob->getBsonObject();
bob= new BsonObjectBuilder();
bob->append("_id",1001);
bob->append("firstName","sajal");
bob->append("lastName","mishra");
bob->append("markes","fail");
BsonObject * update =bob->getBsonObject();
collection->update(selector,update);
collection->close();
mc.close();
cout<<"done";
return 0;
}


```

  lib/mongocpp.cpp
  
  
```

#include<mongocpp.h>

BsonObject::BsonObject() {}
void BsonObject::setBson(bson_t * doc)
{
this->doc = doc;
}
bson_t * BsonObject::get()
{
return doc;
}
void BsonObject::close()
{
bson_destroy(doc);
}




BsonObjectBuilder::BsonObjectBuilder()
{
this->doc = bson_new(); 
this->bsonObject= new BsonObject();
}
void BsonObjectBuilder::append(const char * key,int value)
{
BSON_APPEND_INT32(doc,key,value);
}
void BsonObjectBuilder::append(const char * key,const char * value)
{
BSON_APPEND_UTF8(doc,key,value);
}
void BsonObjectBuilder::append(const char * key,bool value)
{
BSON_APPEND_BOOL(doc,key,value);
}
void BsonObjectBuilder::append(const char * key,double value)
{
BSON_APPEND_DOUBLE(doc,key,value);
}
void BsonObjectBuilder::append(const char * key,BsonObject * value)
{
BSON_APPEND_DOCUMENT(doc,key,value->get());
}

BsonObject * BsonObjectBuilder::getBsonObject()
{
bsonObject->setBson(doc);
return bsonObject;
}



DBCursor::DBCursor() {}

DBCursor::DBCursor(mongoc_cursor_t * cursor)
{
this->cursor=cursor;
}
void DBCursor::setCursor(mongoc_cursor_t *cursor)
{
this->cursor=cursor;
}
bool DBCursor::hasNext()
{
if(mongoc_cursor_next(cursor,&doc)) 
{
return true;
}
return false;
}
char * DBCursor::next()
{
return bson_as_json(doc,NULL);
}

int DBCursor::getInt(const char * k)
{
if(bson_iter_init(&iterator,doc))
{
while(bson_iter_next(&iterator))
{
const char * key=bson_iter_key(&iterator);
if(!strcmp(key,k)) return bson_iter_int32(&iterator);
}
}
}

const char * DBCursor::getString(const char * k)
{
if(bson_iter_init(&iterator,doc))
{
while(bson_iter_next(&iterator))
{
const char * key=bson_iter_key(&iterator);
if(!strcmp(key,k)) return bson_iter_utf8(&iterator,NULL);
}
}
}

bool DBCursor::getBool(const char * k)
{
if(bson_iter_init(&iterator,doc))
{
while(bson_iter_next(&iterator))
{
const char * key=bson_iter_key(&iterator);
if(!strcmp(key,k)) return bson_iter_bool (&iterator);
}
}
}

double DBCursor::getDouble(const char * k)
{
if(bson_iter_init(&iterator,doc))
{
while(bson_iter_next(&iterator))
{
const char * key=bson_iter_key(&iterator);
if(!strcmp(key,k)) return bson_iter_double (&iterator);
}
}
}

void DBCursor::close()
{
mongoc_cursor_destroy (cursor);
}



DBCollection::DBCollection(mongoc_client_t * client, const char * db, const char * collection)
{
this->collection = mongoc_client_get_collection(client,db,collection);
}
void DBCollection::insert(BsonObject * bsonObject)
{
bson_t * doc = bsonObject->get();
bson_error_t error;
if(!mongoc_collection_insert_one(collection,doc,NULL,NULL,&error)) printf("%s\n",error.message);
}
void DBCollection::update(BsonObject * selector,BsonObject * update)
{
bson_error_t error;
if(!mongoc_collection_update(collection,MONGOC_UPDATE_NONE,selector->get(),update->get(),NULL,&error)) printf("%s\n",error.message);
}

void DBCollection::remove(BsonObject * selector)
{
bson_error_t error;
if(!mongoc_collection_remove(collection,MONGOC_REMOVE_NONE,selector->get(),NULL,&error)) printf("%s\n",error.message);
}

DBCursor * DBCollection::find()
{
mongoc_cursor_t *cursor;
bson_t query;
bson_init(&query);
cursor = mongoc_collection_find_with_opts(collection,&query,NULL,NULL);
DBCursor * dbCursor = new DBCursor(cursor);
bson_destroy(&query);
return dbCursor;
}

DBCursor * DBCollection::find(BsonObject * query)
{
mongoc_cursor_t *cursor;
cursor = mongoc_collection_find_with_opts(collection,query->get(),NULL,NULL);
DBCursor * dbCursor = new DBCursor(cursor);
return dbCursor;
}

DBCursor * DBCollection::find(BsonObject * query,BsonObject * opt)
{
mongoc_cursor_t *cursor;
if(query==NULL)
{
bson_t query;
bson_init(&query);
cursor = mongoc_collection_find_with_opts(collection,&query,opt->get(),NULL);
bson_destroy(&query);
}else{
cursor = mongoc_collection_find_with_opts(collection,query->get(),opt->get(),NULL);
}
DBCursor * dbCursor = new DBCursor(cursor);
return dbCursor;
}


void DBCollection::close()
{
mongoc_collection_destroy(this->collection);
}





MongoClient::MongoClient(const char * url)
{
mongoc_init();
this->client = mongoc_client_new(url);
}

DBCollection * MongoClient::getCollection(const char * db, const char * collection)
{
this->collection = new DBCollection(client,db,collection);
return this->collection;
}
void MongoClient::close()
{
mongoc_client_destroy(this->client);
mongoc_cleanup();
}







```


