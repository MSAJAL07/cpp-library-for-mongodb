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
cursor = collection->find(NULL,opt);
while(cursor->hasNext())
{
printf("%s\n",cursor->next());
}
projection->close();
sort->close();
opt->close();
selector->close();
cursor->close();
collection->close();
mc.close();
return 0;
}

