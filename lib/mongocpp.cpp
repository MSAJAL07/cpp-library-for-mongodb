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




