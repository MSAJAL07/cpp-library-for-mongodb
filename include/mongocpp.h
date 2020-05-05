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







