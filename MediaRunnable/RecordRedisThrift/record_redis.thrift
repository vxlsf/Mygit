struct reqinfo {
		1: required string key,
        2: required string value,
        3: required string type,
}

struct resinfo {
		1: optional map<string,string> keyValueMap,
}

service recordService {
	string Update(
		1: reqinfo request,
	)

    resinfo GetAllKeyValue(1: string type)

    string CheckOneRecord(1: reqinfo request)
    
    string DelOneRecord(1: reqinfo request)

    string PushRunnableType(1: string type)

    string DelRunnableType(1: string type)

}
