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


