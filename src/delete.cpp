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
}
