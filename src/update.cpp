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
return 0;
}
