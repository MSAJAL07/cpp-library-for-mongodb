# cpp-library-for-mongodb
Using this library you can perform CRUD operations on the MongoDB database in c++

if you want to build mongo driver visit this link :

https://github.com/linux-on-ibm-z/docs/wiki/Building-C-MongoDB-Driver

Using my library you can write code like this :

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


